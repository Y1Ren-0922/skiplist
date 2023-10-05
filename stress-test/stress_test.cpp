#include <thread>

#include "../skiplist.hpp"

#define NUM_THREADS 1
#define TEST_COUNT 1000000

skiplist<int, std::string> skip_list(16);

int main() {
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