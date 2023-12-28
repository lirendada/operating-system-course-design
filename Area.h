#ifndef __MY_AREA__
#define __MY_AREA__
#include <iostream>
#include <string>

// 分区结构体
class Area
{
public:
    Area(size_t id, size_t start = 0, size_t length = 0, bool used = false)
        : _id(id)
        , _start(start)
        , _length(length)
        , _used(used)
    {}
public:
    size_t _id;        // 分区号
    size_t _start;     // 起始地址
    size_t _length;    // 分区长度
    bool _used;        // 是否被分配，true表示已被分配，默认为false
    std::string _name; // 占用该分区的作业名称

    Area* _prev = nullptr;
    Area* _next = nullptr;
};

#endif