#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__
#include "FixedArea.hpp"
#include "VariableArea.hpp"

class MemoryManager
{
public:
    void FixAreaAllocation(); // 定长分区的分配算法
    void FixAreaRelease();    // 定长分区的释放算法

    void VariableAreaAllocation(); // 变长分区的分配算法：采用最优分配算法
    void VariableAreaRelease();    // 变长分区的释放算法
public:
    FixedArea _fix;
    VariableArea _var;
};

#endif 