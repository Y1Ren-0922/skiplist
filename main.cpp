#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

#include "skiplist.hpp"
#include "type.h"
#include "shell_command.h"

#define NUM_THREADS 1
#define TEST_COUNT 100000

int command_dispatch(sptr_sss skip_list, std::string_view command, const std::vector<std::string> &args) {
    struct {
        std::string_view name;
        shell_cmd* func{};
    } commands[] {
            {"insert", cmd_insert},
            {"delete", cmd_delete},
            {"search", cmd_search},
            {"show", cmd_show},
            {"size", cmd_size},
            {"loadfile", cmd_loadfile},
            {"dumpfile", cmd_dumpfile},
    };

    for (auto & i : commands) {
        if (command == i.name) {
            return i.func(skip_list, args.size(), args);
        }
    }

    std::cout << "不存在命令" << command << "！" << std::endl;
    return -1;
}

void trim(std::string &s) {
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
}

void test();

int main() {
    std::shared_ptr<skiplist<int, std::string>> skip_list = std::make_shared<skiplist<int, std::string>>(16);

    std::string command;
    while(true) {
        std::cout << "kvStorage >> ";
        if (!std::getline(std::cin, command)) {
            return 0;
        }

        if (command.empty()) continue;
        trim(command);

        std::string::const_iterator iter, pre = command.cbegin();
        std::vector<std::string> args;
        while ((iter = std::find(pre, command.cend(), ' ')) != command.cend()) {
            args.push_back(command.substr(pre - command.cbegin(), iter - pre));
            pre = iter + 1;
        }

        if (pre != command.cend()) {
            args.push_back(command.substr(pre - command.cbegin(), iter - pre + 1));
        }

        if (!args.empty()) {
            if (args[0] == "exit") break;
            command_dispatch(skip_list, args[0], std::vector(args.begin() + 1, args.end()));
        }
    }

}

#ifdef TEST
void test() {
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; i ++) {

        std::thread t([]() {
            int tmp = TEST_COUNT / NUM_THREADS;
            for (int count = 0; count < tmp; count ++) {
                skip_list.insert_element(count, "a");
            }
        });

        threads.push_back(std::move(t));
    }

    for (auto &t1: threads) t1.join();

    auto finish = std::chrono::high_resolution_clock::now();
    auto dt = finish - start;

    int64_t sec = std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
    std::cout << "insert: " << sec << std::endl;

    threads.clear();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; i ++) {

        std::thread t([]() {
            int tmp = TEST_COUNT / NUM_THREADS;
            for (int count = 0; count < tmp; count ++) {
                skip_list.search_element(rand() % TEST_COUNT);
            }
        });
        threads.push_back(std::move(t));
    }

    for (auto &t1: threads) t1.join();

    finish = std::chrono::high_resolution_clock::now();
    dt = finish - start;

    sec = std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
    std::cout << "search: " << sec << std::endl;
}

#endif