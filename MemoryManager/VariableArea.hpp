#ifndef __VARIABLE_AREA__
#define __VARIABLE_AREA__
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "Area.h"
using std::cout;
using std::endl;
/*
    可变分区方式：此种方式根据每个作业要求的实际大小分割一块空间，回收时如果与其他空闲空间连接时须合并。优点是内存利用率高，缺点是实现复杂。
*/
class VariableArea
{
public:
    VariableArea()
    {
        _free_variable_area.push_back(new Area(_free_num++, 4, 124, false));
    }

    void show()
    {
        cout << " #未分配分区表：" << endl;
        if(_free_variable_area.size() > 0)
        {
            cout << "--------------------------------------------------------------" << endl;
            printf("|  分区号%10c起始地址%10c长度%10c占用标志  |\n", ' ', ' ', ' ');
            cout << "--------------------------------------------------------------" << endl;
            for(int i = 0; i < _free_variable_area.size(); ++i)
            {
                if(_free_variable_area[i]->_used == false)
                    printf("|%5u%12c%6u%10c%6u%11c 未占用   |\n", _free_variable_area[i]->_id, ' ', _free_variable_area[i]->_start, ' ', _free_variable_area[i]->_length, ' ');
                else
                    printf("|%5u%12c%6u%10c%6u%11c 已占用   |\n", _free_variable_area[i]->_id, ' ', _free_variable_area[i]->_start, ' ', _free_variable_area[i]->_length, ' ');
            }
            cout << "--------------------------------------------------------------" << endl;
        }
        else
            cout << "暂无未分配的分区！！！" << endl;

        cout << " #已分配分区表：" << endl;
        if(_used_variable_area.size() > 0)
        {
            cout << "--------------------------------------------------------------" << endl;
            printf("|  分区号%10c起始地址%10c长度%10c占用标志  |\n", ' ', ' ', ' ');
            cout << "--------------------------------------------------------------" << endl;
            for(int i = 0; i < _used_variable_area.size(); ++i)
            {
                if(_used_variable_area[i]->_used == false)
                    printf("|%5u%12c%6u%10c%6u%11c 未占用   |\n", _used_variable_area[i]->_id, ' ', _used_variable_area[i]->_start, ' ', _used_variable_area[i]->_length, ' ');
                else
                    printf("|%5u%12c%6u%10c%6u%11c 已占用   |\n", _used_variable_area[i]->_id, ' ', _used_variable_area[i]->_start, ' ', _used_variable_area[i]->_length, ' ');
            }
            cout << "--------------------------------------------------------------" << endl;
        }
        else
            cout << "暂无已分配的分区！！！" << endl << endl;
    }
public:
    std::vector<Area*> _free_variable_area; // 未分配分区表
    std::vector<Area*> _used_variable_area; // 已分配分区表
    size_t _free_num = 1; // 用于给未分配分区表计数id的
    size_t _used_num = 1; // 用于给已分配分区表计数id的
};

#endif