#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <ctime>

enum class State : char {
	Wait = 'W',
	Run = 'R',
	Finish = 'F'
};

class PCB
{
public:
	PCB(uint16_t pid, std::string name, uint16_t priority, time_t left_time)
		: _pid(pid)
		, _name(name)
		, _priority(priority)
		, _left_time(left_time)
		, _consume_time(left_time)
        , _used_time(0)
        , _state(State::Wait)
        , _start_time(-1)
		, _end_time(-1)
		, _arrive_time(time(nullptr))
	{
		std::cout << "创建PCB成功，id为：" << _pid << std::endl;
	}

	// 打印PCB的信息 
	void show()
	{ 
		char timebuffer[128];
		time_t timestamp = time(&_arrive_time);
		struct tm* timeinfo = localtime(&timestamp);
		strftime(timebuffer, sizeof(timebuffer), "%H:%M:%S", timeinfo);

        std::cout << "--------------------------------------------------------------------" << std::endl;
        printf("|进程号：%-24u  进程名：%-24s|\n", _pid, _name.c_str());
        printf("|最后优先级：%-20u  进程状态：%-22c|\n", _priority, (char)_state);
        printf("|到达时间：%-22s  已用CPU时间：%-19u|\n", timebuffer, _used_time);
        printf("|相对开始运行时间：%-14u  ", _start_time);
		if(_end_time == -1)
			printf("相对结束运行时间：未定          |\n");
		else
			printf("相对结束运行时间：%-14u|\n", _end_time);
        printf("|总共需运行的时长：%-14u  还需运行时长：%-18u|\n", _consume_time, _left_time);
        std::cout << "--------------------------------------------------------------------" << std::endl;
	}
public:
	uint16_t _pid;		  // 进程号
	std::string _name;    // 进程名称
	uint16_t _priority;   // 优先级
	time_t _arrive_time;  // 到达就绪队列的时间
	time_t _left_time;    // 剩余需要运行的时间
	time_t _consume_time; // 总共需要的运行时间
	time_t _used_time;    // 已用CPU时间
    time_t _start_time;   // 实际开始运行时间
    time_t _end_time;     // 结束运行时间
	State _state;		  // 进程状态
};