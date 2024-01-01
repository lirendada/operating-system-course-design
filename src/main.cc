#include "interface.hpp"
#include <pthread.h>

// 测试动态优先级算法的多线程函数
void* testDynamicPriority(void* args)
{
    PCBManager* pcbm = static_cast<PCBManager*>(args);
    for(int i = 0; i < 100; ++i)
    {
        if(i % 10 == 0)
            pcbm->Create(i, "noqi", i, i % 255);
    }
    return nullptr;
}

int main()
{
	// PCBManager pcbm;
    // pcbm.Create(1, "asd", 10, 20);
    // pcbm.Create(2, "qwe", 100, 50);
    // pcbm.Create(3, "zxc", 30, 5);
    // pcbm.Create(4, "bnm", 41, 78);
    // pcbm.Create(5, "jkl", 40, 67);
    // pcbm.Create(6, "uyh", 100, 40);

    // // pthread_t pid = 1;
    // // pthread_create(&pid, nullptr, testDynamicPriority, &pcbm);

    // // pcbm.FCFS();
    // // pcbm.SJF();
    // // pcbm.StaticPriority();
    // // pcbm.DynamicPriority();

    // // pthread_join(pid, nullptr);

    // pcbm.RR();

    // pcbm.CreateMultiQueue(1, "asd", 10, 20);
    // pcbm.CreateMultiQueue(2, "qwe", 100, 50);
    // pcbm.CreateMultiQueue(3, "zxc", 30, 5);
    // pcbm.CreateMultiQueue(4, "bnm", 41, 78);
    // pcbm.CreateMultiQueue(5, "jkl", 40, 67);
    // pcbm.CreateMultiQueue(6, "uyh", 100, 40);
    // pcbm.MultiLevelFeedbackQueue();

    // Interface face;
    // face.ProcessInterface();

    // MemoryManager mm;
    // mm.FixAreaAllocation();
    // mm.FixAreaRelease();
    // mm.FixAreaRelease();

    // MemoryManager mm;
    // mm.VariableAreaAllocation();
    // mm.VariableAreaAllocation();
    // mm.VariableAreaRelease();
    // mm.VariableAreaRelease();

    // Interface face;
    // face.MemoryInterface();

    Interface* f = Interface::GetInstance();
    f->Menu();
	return 0;
}