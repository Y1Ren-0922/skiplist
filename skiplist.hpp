#pragma once

#include <cstring>
#include <iostream>
#include <random>
#include <optional>
#include <fstream>
#include <sstream>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "utils/my_string.h"

// 读写锁
static std::shared_mutex m_mtx;

static std::random_device rnd;
static std::mt19937 rng(rnd());
static std::uniform_int_distribution<int> uni(0, 100);

template <typename K, typename V>
class Node {
public:
    Node() = default;
    Node(K key, V value, int level);
    ~Node();

    std::shared_ptr<Node<K, V>> *forward;
    int node_level = 0;

    K get_key() const;
    V get_value() const;

    void set_value(V v);

private:
    K key;
    V value;
};

template<typename K, typename V>
Node<K, V>::Node(K key, V value, int level): key(key), value(value), node_level(level) {
    forward = new std::shared_ptr<Node<K, V>>[level + 1];

    memset(forward, 0, sizeof(Node<K, V>*) * (level + 1));
}

template<typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
}

template<typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
}

template<typename K, typename V>
void Node<K, V>::set_value(V v) {
    value = v;
}

template<typename K, typename V>
Node<K, V>::~Node() {
    delete[] forward;
}


template <typename K, typename V>
class skiplist {
public:

    explicit skiplist(int max_level);

    std::shared_ptr<Node<K, V>> create_node(K key, V value, int level);
    int insert_element(K key, V value);
    void delete_element(K key);
    void display_list();
    std::optional<V> search_element(K key);
    void dump_file(const std::string &filename);
    void load_file(const std::string &filename);
    void show_list();
    int get_random_level();
    int size();

    K key_flag;
    V value_flag;

private:
    int _max_level;
    int _current_level;
    int _element_count;

    std::shared_ptr<Node<K, V>> _head;
};


template<typename K, typename V>
skiplist<K, V>::skiplist(int max_level): _max_level(max_level), _current_level(0), _element_count(0) {
    K k;
    V v;
    _head = std::make_shared<Node<K, V>>(k, v, _max_level);
}

template<typename K, typename V>
std::shared_ptr<Node<K, V>> skiplist<K, V>::create_node(K key, V value, int level) {
    return std::make_shared<Node<K, V>>(key, value, level);
}

template<typename K, typename V>
int skiplist<K, V>::insert_element(K key, V value) {
    // 插入，独占锁
    std::unique_lock grd(m_mtx);

    std::shared_ptr<Node<K, V>> current = _head;

    auto *update = new std::shared_ptr<Node<K, V>>[_max_level + 1];
    for (int i = _current_level; i >= 0; i --) {
        while (current->forward[i] && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];
    if (current && current->get_key() == key) {
        delete[] update;
        return 1;
    }

    if (!current || current->get_key() != key) {
        int random_level = get_random_level();
        if (random_level > _current_level) {
            for (int i = _current_level + 1; i <= random_level; i ++)
                update[i] = _head;
            _current_level = random_level;
        }

        std::shared_ptr<Node<K, V>> inserted_node = create_node(key, value, random_level);
        for (int i = 0; i <= random_level; i ++) {
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }

        _element_count ++;
    }
    delete[] update;
    return 0;
}

template<typename K, typename V>
int skiplist<K, V>::get_random_level() {
    int k = 0;
    while (uni(rng) % 2)
        k ++;

    return std::min(k, _max_level);
}

template<typename K, typename V>
void skiplist<K, V>::delete_element(K key) {
    std::unique_lock grd(m_mtx);

    std::shared_ptr<Node<K, V>> current = _head;
    auto *update = new std::shared_ptr<Node<K, V>>[_current_level + 1];

    for (int i = _current_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];

    if (current && current->get_key() != key) {
        delete[] update;
        return;
    }

    if (current->get_key() == key) {
        for (int i = _current_level; i >= 0; i --) {
            std::shared_ptr<Node<K, V>> m_forward = update[i]->forward[i];
            if (m_forward && m_forward->get_key() == key)
                update[i]->forward[i] = m_forward->forward[i];

            if (!_head->forward[i]) _current_level--;
        }
        --_element_count;
    }
    delete[] update;
}

template<typename K, typename V>
void skiplist<K, V>::display_list() {
    std::shared_lock grd(m_mtx);

    std::shared_ptr<Node<K, V>> current = _head;
    for (int i = _current_level; i >= 0; i --) {
        std::shared_ptr<Node<K, V>> tmp = current->forward[i];
        while (tmp) {
            std::cout << tmp->get_key() << "\t";
            tmp = tmp->forward[i];
        }
        std::cout << std::endl;
    }
}

template<typename K, typename V>
std::optional<V> skiplist<K, V>::search_element(K key) {
    std::shared_lock grd(m_mtx);

    std::shared_ptr<Node<K, V>> current = _head;
    for (int i = _current_level; i >= 0; i --) {
        while (current->forward[i] && current->forward[i]->get_key() < key)
            current = current->forward[i];
    }

    current = current->forward[0];
    if (current && current->get_key() == key) {
        return current->get_value();
    }

    return std::nullopt;
}

template<typename K, typename V>
void skiplist<K, V>::dump_file(const std::string &filename) {
    std::shared_ptr<Node<K, V>> current = _head;

    std::ofstream file(filename);

    while (current->forward[0]) {
        std::stringstream ss;
        ss << current->forward[0]->get_key() << ":" << current->forward[0]->get_value() << "\n";
        file << ss.str();

        current = current->forward[0];
    }

    file.close();
}

template<typename K, typename V>
void skiplist<K, V>::load_file(const std::string &filename) {

    std::ifstream in(filename);
    if (!in) {
        std::cout << "file doesnt exist!" << std::endl;
        return;
    }


    std::string line;

    while(getline(in, line)) {
        auto t = line.find(':');
        K key = switch_type<K>(line.substr(0, t));
        V value = switch_type<V>(line.substr(0, t));

        insert_element(key, value);
    }
}

template<typename K, typename V>
void skiplist<K, V>::show_list() {
    std::shared_lock grd(m_mtx);

    std::shared_ptr<Node<K, V>> current = _head;

    while (current->forward[0]) {
        std::cout << current->forward[0]->get_key() << ": " << current->forward[0]->get_value() << std::endl;
        current = current->forward[0];
    }
}

template<typename K, typename V>
int skiplist<K, V>::size() {
    return _element_count;
}



