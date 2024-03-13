#include <stdlib.h>
#include <time.h>
#include <memory>
#include <iostream>
#include <chrono> // 用于测量执行时间
#include "BPTree.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

int main(int argc, char const *argv[])
{
    auto tree = make_unique<BPTree<int, int>>(); // 使用智能指针自动管理内存

    const int num_operations = 1000; // 测试操作的数量
    srand(time(0)); // 生成随机数种子
    // 插入随机键及其值（假设键和值相同）
    auto start = high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        int key = rand() % 10000; // 0到9999之间的随机键
        tree->insert(key, key);
    }
    auto stop = high_resolution_clock::now();
    auto insert_duration = duration_cast<microseconds>(stop - start);
    cout << "插入 " << num_operations << " 个键花费了 " << insert_duration.count() << " 微秒。" << endl;
    
    // 在程序中的需要暂停的位置添加：
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // 扫描树中所有的值
    cout << "扫描所有值：" << endl;
    tree->scan();

    // 查找随机键并报告时间
    start = high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        int key = rand() % 10000;
        auto value = tree->find(key);
        // 若要查看查找结果，请取消以下注释：
        // cout << "查找键 " << key << ": " << (value ? *value : -1) << endl;
    }
    stop = high_resolution_clock::now();
    auto find_duration = duration_cast<microseconds>(stop - start);
    cout << "查找 " << num_operations << " 个随机键花费了 " << find_duration.count() << " 微秒。" << endl;

    // 如果需要，可以类似地实现和测试erase操作。

    return 0;
}