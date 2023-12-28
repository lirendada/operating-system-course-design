#ifndef __MY_INTERFACE__
#define __MY_INTERFACE__
#include "PCBManager.h"
#include "MemoryManager.h"

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
            cout << "输入你的选择：插入方式、调度算法>> ";
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

    // 进程管理模块菜单
    static void ProcessMenu()
    {
        std::cout << "-----------------------进程管理模块主菜单-----------------------" << std::endl;
        printf("| -_-先进行对应数据插入，然后选择以下某种调度算法开始运行：    |\n");
        printf("|       a. 插入数据            b. 插入数据(多级队列算法专用)   |\n");
        printf("|       1. 先来先服务算法      2. 短作业优先调度算法           |\n");
        printf("|       3. 静态优先级算法      4. 动态优先级算法               |\n");
        printf("|       5. 轮转调度算法        6. 多级反馈队列调度算法         |\n");
        printf("|       0. 退出                                                |\n");
        std::cout << "----------------------------------------------------------------" << std::endl;
    }

    // 存储器管理模块界面
    void MemoryInterface()
    {
        while(true)
        {
            MemoryMenu();
            cout << "输入需要的分区模式>> ";
            int option;
            cin >> option;
            
            // 过滤掉非法选择
            if(option == 0)
            {
                cout << "退出成功！！！" << endl;
                break;
            }
            else if(option < 0 || option > 2)
            {
                cout << "输入错误或无效，请重新输入！！！" << endl;
                continue;
            }

            // 判断处理
            MemoryManager mm;
            if(option == 1)
            {
                while(true)
                {
                    MemoryOperationMenu();
                    cout << "输入需要的操作选择>> ";
                    int option1;
                    cin >> option1;

                    // 过滤掉非法选择
                    if(option1 == 0)
                    {
                        cout << "退回主菜单！！！" << endl;
                        break;
                    }
                    else if(option1 < 0 || option1 > 2)
                    {
                        cout << "输入错误或无效，请重新输入！！！" << endl;
                        continue;
                    }

                    if(option1 == 1)
                        mm.FixAreaAllocation();
                    else    
                        mm.FixAreaRelease();
                }
            }
            else if(option == 2)
            {
                while(true)
                {
                    MemoryOperationMenu();
                    cout << "输入需要的操作选择>> ";
                    int option1;
                    cin >> option1;

                    // 过滤掉非法选择
                    if(option1 == 0)
                    {
                        cout << "退回主菜单！！！" << endl;
                        break;
                    }
                    else if(option1 < 0 || option1 > 2)
                    {
                        cout << "输入错误或无效，请重新输入！！！" << endl;
                        continue;
                    }

                    if(option1 == 1)
                        mm.VariableAreaAllocation();
                    else    
                        mm.VariableAreaRelease();
                }
            }
        }
    }

    // 存储器管理模块主菜单
    static void MemoryMenu()
    {
        std::cout << "----------------------存储器管理模块主菜单----------------------" << std::endl;
        printf("| -_-选择对应的分区模式：                                      |\n");
        printf("|       1. 固定分区方式        2. 可变分区方式                 |\n");
        printf("|       0. 退出                                                |\n");
        std::cout << "----------------------------------------------------------------" << std::endl;
    }

    // 存储器管理模块操作方式菜单
    static void MemoryOperationMenu()
    {
        std::cout << "---------------------存储器管理模块操作选择---------------------" << std::endl;
        printf("| -_-选择对应的操作：                                          |\n");
        printf("|       1. 分配空间            2. 释放空间                     |\n");
        printf("|       0. 返回主菜单                                          |\n");
        std::cout << "----------------------------------------------------------------" << std::endl;
    }
private:
    PCBManager pcbm; // 进程模块管理对象
};

#endif