#pragma once
#include "PCBManager.h"

/*
    这个类提供多个管理系统的页面以及选择功能
*/
class Interface
{
public:
    // 进程管理模块界面
    void ProcessInterface()
    {
        while(true)
        {
            ProcessMenu();
            cout << ">> ";
            char ins = 0;
            int algorithm = -1;
            cin >> ins >> algorithm;
            
            if(algorithm == 0)
            {
                cout << "退出成功！！！" << endl;
                break;
            }
            else if(algorithm < 0 || algorithm > 6)
            {
                cout << "输入错误或无效，请重新输入！！！" << endl;
                continue;
            }
            else if(algorithm == 6 && ins != 'b')
            {
                cout << "多级反馈队列调度算法应该选择b选项进行插入，请重新输入！！！" << endl;
                continue;
            }

            // 先处理插入数据
            if(ins == 'a' || ins == 'b')
            {
                cout << "输入要插入的进程个数>> ";
                int n = 0;
                cin >> n;
                if(n <= 0)
                {
                    cout << "输入错误或无效，请重新输入！！！" << endl;
                    continue;
                }

                while(n--)
                {
                    uint16_t pid;
                    std::string name;
                    uint16_t priority;
                    time_t left_time;
                    cout << "分别输入进程号、进程名、优先级、需要运行的时间>> ";
                    cin >> pid >> name >> priority >> left_time;

                    if(ins == 'a')
                        pcbm.Create(pid, name, priority, left_time);
                    else
                        pcbm.CreateMultiQueue(pid, name, priority, left_time);
                }
            }
            else
            {
                cout << "输入错误或无效，请重新输入！！！" << endl;
                continue;
            }

            // 再处理算法选择
            if(algorithm == 1)
                pcbm.FCFS();
            else if(algorithm == 2)
                pcbm.SJF();
            else if(algorithm == 3)
                pcbm.StaticPriority();
            else if(algorithm == 4)
                pcbm.DynamicPriority();
            else if(algorithm == 5)
                pcbm.RR();
            else if(algorithm == 6)
                pcbm.MultiLevelFeedbackQueue();
        }
    }

    static void ProcessMenu()
    {
        std::cout << "--------------------------进程管理模块--------------------------" << std::endl;
        printf("| -_-先进行对应数据插入，然后选择以下某种调度算法开始运行：    |\n");
        printf("|       a. 插入数据            b. 插入数据(多级队列算法专用)   |\n");
        printf("|       1. 先来先服务算法      2. 短作业优先调度算法           |\n");
        printf("|       3. 静态优先级算法      4. 动态优先级算法               |\n");
        printf("|       5. 轮转调度算法        6. 多级反馈队列调度算法         |\n");
        printf("|       0. 退出                                                |\n");
        std::cout << "----------------------------------------------------------------" << std::endl;
    }
private:
    PCBManager pcbm; // 进程模块管理对象
};