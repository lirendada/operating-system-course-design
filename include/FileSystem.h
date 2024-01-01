#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <algorithm>
using std::cout;
using std::endl;

class FileSystem
{
public:
    FileSystem(); // 构造函数完成序列的文件读入

    void show_result(std::vector<int>& ret, int sum); // 打印磁道服务顺序以及移动总道数
    void FCFS(); // 先来先服务算法
    void SSTF(); // 最短寻道优先算法
    void SCAN(); // 电梯（扫描）算法
public:
    std::vector<int> _sequence;
};

#endif