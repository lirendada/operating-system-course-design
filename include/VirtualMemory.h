#ifndef __VIRTUAL_MEMORY_H__
#define __VIRTUAL_MEMORY_H__
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <fstream>
using std::cout;
using std::endl;

class VirtualMemory
{
public:
    VirtualMemory(); // 构造函数完成序列的文件读入
    void FIFO(); // 先进先出页面置换算法
    void LRU();  // 最近最久未使用算法
    void OPT();  // 最佳页面置换算法
    
    void show_sequence();  // 打印页面序列
    void show_eliminate(std::vector<int>& eliminate, int sum, std::list<int>& queue); // 打印淘汰页面以及结果
    bool test_same_in_queue(std::list<int>& queue, int i, std::list<int>::iterator& out); // 检查是否当前队列中是否存在当前要调度的页面
private:
    std::vector<int> _sequence;
};

#endif