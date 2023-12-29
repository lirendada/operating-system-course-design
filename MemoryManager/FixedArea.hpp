#ifndef __FIXED_AREA__
#define __FIXED_AREA__
#include <iostream>
#include <vector>
#include <string>
#include "Area.h"
using std::cout;
using std::endl;

/*
    定长分区结构定义，以及定长表的打印
*/
static const int FIXED_AREA_NUM = 6; // 分区个数
static const size_t START_ADDRESS[FIXED_AREA_NUM] = { 4, 12, 28, 44, 68, 92 }; // 各分区起始地址
static const size_t AREA_LENGTH[FIXED_AREA_NUM] = { 8, 16, 16, 24, 24, 36 };   // 各分区长度
class FixedArea
{
public:
    FixedArea()
    {
        for(int i = 0; i < FIXED_AREA_NUM; ++i)
            _fixed_area.push_back(new Area(i + 1, START_ADDRESS[i], AREA_LENGTH[i], false));
    }

    void show()
    {
        cout << "固定分区表：" << endl;
        cout << "--------------------------------------------------------------" << endl;
        printf("|  分区号%10c起始地址%10c长度%10c占用标志  |\n", ' ', ' ', ' ');
        cout << "--------------------------------------------------------------" << endl;
        for(int i = 0; i < FIXED_AREA_NUM; ++i)
        {
            if(_fixed_area[i]->_used == false)
                printf("|%5u%12c%6u%10c%6u%11c 未占用   |\n", _fixed_area[i]->_id, ' ', _fixed_area[i]->_start, ' ', _fixed_area[i]->_length, ' ');
            else
                printf("|%5u%12c%6u%10c%6u%11c 已占用   |\n", _fixed_area[i]->_id, ' ', _fixed_area[i]->_start, ' ', _fixed_area[i]->_length, ' ');
        }
        cout << "--------------------------------------------------------------" << endl;
    }
public:
    std::vector<Area*> _fixed_area; // 固定分区对象的指针数组
};
#endif 