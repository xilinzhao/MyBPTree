#include <iostream>
#include <chrono>
#include <memory>
#include <random>
#include <thread>
#include <vector>
#include <mutex>
#include "MyBPTree.h" // 确保你的MyBPTree实现是线程安全的

using namespace std;
using namespace std::chrono;

mutex tree_mutex; // 全局互斥锁

void insertFunction(unique_ptr<MyBPTree<int, int>>& tree, int num_operations) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 10000000);

    for (int i = 0; i < num_operations; ++i) {
        int key = distrib(gen);
        {
            lock_guard<mutex> guard(tree_mutex);
            tree->insert(key, key);
        }
    }
}

void findFunction(unique_ptr<MyBPTree<int, int>>& tree, int num_operations) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 10000000);

    for (int i = 0; i < num_operations; ++i) {
        int key = distrib(gen);
        lock_guard<mutex> guard(tree_mutex);
        tree->find(key);
        
    }
}

int main() {
    auto tree = make_unique<MyBPTree<int, int>>();
    const int num_operations = 100000; // 减少操作数量以避免长时间运行

    // 插入操作的多线程
    auto start = high_resolution_clock::now();
    thread insertThread1(insertFunction, ref(tree), num_operations / 2);
    thread insertThread2(insertFunction, ref(tree), num_operations / 2);
    insertThread1.join();
    insertThread2.join();
    auto stop = high_resolution_clock::now();
    auto insert_duration = duration_cast<microseconds>(stop - start);
    cout << "并发插入操作花费了 " << insert_duration.count() << " 微秒。" << endl;

    // 查找操作的多线程
    start = high_resolution_clock::now();
    thread findThread1(findFunction, ref(tree), num_operations / 2);
    thread findThread2(findFunction, ref(tree), num_operations / 2);
    findThread1.join();
    findThread2.join();
    stop = high_resolution_clock::now();
    auto find_duration = duration_cast<microseconds>(stop - start);
    cout << "并发查找操作花费了 " << find_duration.count() << " 微秒。" << endl;

    return 0;
}