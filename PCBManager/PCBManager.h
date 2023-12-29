#ifndef __PCBMANAGER_HPP__
#define __PCBMANAGER_HPP__
#include "PCB.hpp"
#include <list>
#include <vector>
#include <algorithm>
#include <mutex>
#include <unistd.h>
using std::cout;
using std::endl;
using std::cin;

static const int PRI_ADD = 3;         // 优先级增长大小
static const int TIMER_CHIP = 20;     // 时间片大小

static const int MULTI_QUEUE_NUM = 3; // 多级队列的数量
static int MULTI_TIMER_CHIP[MULTI_QUEUE_NUM] = { 20, 40, 60 }; // 多级队列每层队列的时间片大小

class PCBManager
{
public:
    PCBManager(int multi_queue_num = MULTI_QUEUE_NUM)
        : _multi_list(multi_queue_num)
    {}

    void Create(uint16_t pid, std::string name, uint16_t priority, time_t left_time); // 创建PCB
    void FCFS();                    // 先来先服务算法
    void SJF();                     // 短作业优先调度算法
    void StaticPriority();          // 静态优先级算法
    void DynamicPriority();         // 动态优先级算法
    void RR();                      // 轮转调度算法
    void MultiLevelFeedbackQueue(); // 多级反馈队列调度算法
    void CreateMultiQueue(uint16_t pid, std::string name, uint16_t priority, time_t left_time); // 创建多级队列中的PCB，其实就是换成插入到多级队列中
private:
	std::list<PCB*> _list; // 存放PCB的队列
    std::vector<std::list<PCB*>> _multi_list; // 多级队列
    int process_num = 0;
};

#endif