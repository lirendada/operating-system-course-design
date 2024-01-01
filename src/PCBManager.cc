#include "../include/PCBManager.h"

// 创建PCB
void PCBManager::Create(uint16_t pid, std::string name, uint16_t priority, time_t left_time)
{
    PCB* ptr = new PCB(pid, name, priority, left_time);
    if(ptr == nullptr)
    {
        std::cout << "创建PCB失败" << std::endl;
        return;
    }
    _list.push_back(ptr);
}

// 先来先服务算法
void PCBManager::FCFS()
{
    /* 
        FCFS是最简单的CPU调度算法，即按进程到来的先后次序进行调度，
        这样在系统中等待时间最长的进程被优先调度，而不管其所需运行时间的长短。
    */
    cout << "*********************************************" << endl;
    cout << "FCFS调度算法：";
    std::vector<PCB*> tmp;
    auto it = _list.begin();
    int current_time = 0;
    while (it != _list.end())
    {
        (*it)->_used_time = (*it)->_left_time; // 使用时间为实际运行时间
        (*it)->_left_time = 0; // 剩余时间设置为0，因为运行到结束为止
        (*it)->_start_time = current_time; // 开始时间为当前时间
        (*it)->_end_time = (*it)->_start_time + (*it)->_used_time;
        (*it)->_state = State::Finish; // 别忘了设置状态
        
        cout << (*it)->_pid << "(当前时间：" << current_time << ")";
        current_time += (*it)->_used_time; // 当前时间加上实际运行时间
        tmp.push_back(*it);
        if ((++it) != _list.end())
            cout << " -> ";
    }
    cout << endl;

    cout << "当前所有进程的状态：" << endl;
    for(auto& e : tmp)
        e->show();
    cout << "*********************************************" << endl;
}

// 短作业优先调度算法
void PCBManager::SJF()
{
    /*
        指当CPU可供使用时SJF算法把CPU分给需要运行时间最短的进程。
    */
    cout << "*********************************************" << endl;
    cout << "SJF调度算法：";
    std::vector<PCB*> tmp(_list.begin(), _list.end());
    std::sort(tmp.begin(), tmp.end(), [&](const PCB* p1, const PCB* p2)->bool{
        return p1->_consume_time <= p2->_consume_time;
    });

    int current_time = 0;
    int n = tmp.size();
    for(int i = 0; i < n; ++i)
    {
        tmp[i]->_used_time = tmp[i]->_left_time; // 使用时间为实际运行时间
        tmp[i]->_left_time = 0; // 剩余时间设置为0，因为运行到结束为止
        tmp[i]->_start_time = current_time; // 开始时间为当前时间
        tmp[i]->_end_time = tmp[i]->_start_time + tmp[i]->_used_time;
        tmp[i]->_state = State::Finish; // 别忘了设置状态
        
        cout << tmp[i]->_pid << "(当前时间：" << current_time << ")";
        current_time += tmp[i]->_used_time; // 当前时间加上实际运行时间
        if (i != n - 1)
            cout << " -> ";
    }
    cout << endl;

    cout << "当前所有进程的状态：" << endl;
    for(auto& e : tmp)
        e->show();
    cout << "*********************************************" << endl;
}

// 静态优先级算法
void PCBManager::StaticPriority()         
{
    /*
        静态优先数是指在进程开始运行之前便根据某种或某些
        因素(如估计运行时间、主存需求量、打开文件个数、所付经费多少等)算定，
        而且该优先数在进程的整个生命周期内一直不变。
        静态优先数方法虽然简单，但有可能导致某些低优先级的进程无限期地等待。
        尤其在高优先级的进程不断进入就绪队列的情况下，
        使等待CPU的低优先级进程更多，等待时间更长。
    */
    cout << "*********************************************" << endl;
    cout << "静态优先级调度算法：";
    std::vector<PCB*> tmp(_list.begin(), _list.end());
    std::sort(tmp.begin(), tmp.end(), [&](const PCB* p1, const PCB* p2)->bool{
        return p1->_priority >= p2->_priority;
    });

    int current_time = 0;
    int n = tmp.size();
    for(int i = 0; i < n; ++i)
    {
        tmp[i]->_used_time = tmp[i]->_left_time; // 使用时间为实际运行时间
        tmp[i]->_left_time = 0; // 剩余时间设置为0，因为运行到结束为止
        tmp[i]->_start_time = current_time; // 开始时间为当前时间
        tmp[i]->_end_time = tmp[i]->_start_time + tmp[i]->_used_time;
        tmp[i]->_state = State::Finish; // 别忘了设置状态
        
        cout << tmp[i]->_pid << "(当前时间：" << current_time << ")";
        current_time += tmp[i]->_used_time; // 当前时间加上实际运行时间
        if (i != n - 1)
            cout << " -> ";
    }
    cout << endl;

    cout << "当前所有进程的状态：" << endl;
    for(auto& e : tmp)
        e->show();
    cout << "*********************************************" << endl;
}

// 动态优先级算法
void PCBManager::DynamicPriority()         
{
    /*
        动态优先数方法是按照某种原则使各进程的优先级随着时间而改变。
        例如随等待时间增大优先级也跟着提高、随着使用CPU时间的增长优先级跟着下降，
        就是一种较好的策略。等待了较长时间的进程，总会因其优先级不断地提高而被调度运行。

        这里实现方式为：随着时间增长，每执行完一个任务之后就让其它任务的优先级增加，重新进行排序！
    */
    cout << "*********************************************" << endl;
    cout << "动态优先级调度算法：";
    std::vector<PCB*> ret;
    _list.sort([&](const PCB* p1, const PCB* p2)->bool{
        // 先按照优先级排序，优先级相等则按照FCFS思想排序，先来先处理，由_arrive_time大小决定
        if(p1->_priority > p2->_priority) 
            return true;
        else if(p1->_priority < p2->_priority)
            return false;
        else   
            return p1->_arrive_time > p2->_arrive_time;
    });
    
    int current_time = 0;
    auto it = _list.begin();
    int pre_size = _list.size(); // 记录下先前的链表节点个数
    while(it != _list.end())
    {
        PCB* front = _list.front();
        _list.pop_front();
        
        front->_used_time += front->_left_time; // 使用时间为实际运行时间
        front->_left_time = 0;
        front->_start_time = current_time; // 开始时间和到达时间为当前时间
        front->_end_time = front->_start_time + front->_used_time;
        front->_state = State::Finish; // 别忘了设置状态
        
        ret.push_back(front); // 插入结果集
        pre_size--;
        
        cout << front->_pid << "(当前时间：" << current_time << ")";
        // 注意迭代器失效问题（因为上面删除了节点）
        it = _list.begin();
        if(it != _list.end())
            cout << " -> ";

        // 为其它节点更新优先级，这是必须做的，不然就起不到动态优先级的效果
        auto tmp = _list.begin();
        while(tmp != _list.end())
        {
            (*tmp)->_priority += PRI_ADD;
            ++tmp;
        }

        // 只有出现节点增长，才需要重新排序更新优先级，因为此时引入了新进程（其它线程或进程中）
        if(_list.size() > pre_size)
        {
            // 此处排序和上面一样
            _list.sort([&](const PCB* p1, const PCB* p2)->bool{
                if(p1->_priority > p2->_priority) 
                    return true;
                else if(p1->_priority < p2->_priority)
                    return false;
                else   
                    return p1->_arrive_time <= p2->_arrive_time;
            });
        }

        // 别忘了更新当前时间
        current_time += front->_consume_time;
        // sleep(1);
    }
    cout << endl;

    cout << "当前所有进程的状态：" << endl;
    for(auto& e : ret)
        e->show();
    cout << "*********************************************" << endl;
}

// 轮转调度算法
void PCBManager::RR()
{
    /*
        轮转调度的核心就是时间片，一般取10~100ms为单位进行就绪队列中任务的执行，
        如果有任务在时间片内执行完毕，则将其从就绪队列中删掉。
        就绪队列本质上是一个环形队列，直到队列为空之前，我们都要一直循环处理该队列
        （默认一开始处理顺序为就绪队列的顺序进行处理）
    */
    cout << "*********************************************" << endl;
    cout << "轮转调度算法：" << endl;
    std::vector<PCB*> ret;

    int current_time = 0;
    while(!_list.empty())
    {
        // 开始从头遍历链表每个PCB，每个PCB获得一个时间片也就是TIMER_CHIP的运行时间
        auto it = _list.begin();
        while(it != _list.end())
        {
            // 判断时间片和该PCB剩余执行时长的关系
            if((*it)->_left_time > TIMER_CHIP)
            {
                // 如果大于时间片的话，说明还不能执行完，则简单的更新时间就行
                if((*it)->_start_time == -1) 
                    (*it)->_start_time = current_time; // 第一次运行该进程则设置其实际开始运行时间
                (*it)->_used_time += TIMER_CHIP;
                (*it)->_left_time -= TIMER_CHIP;
                (*it)->_state = State::Run;

                cout << "当前执行进程为：" << (*it)->_pid << "  相对运行时间为：" << current_time << "  该进程还没运行完，其属性如下所示：" << endl;
                (*it)->show();
                current_time += TIMER_CHIP;
                (*it)->_state = State::Wait; // 打印完再设置为等待状态
                ++it;
            }
            else
            {
                // 如果小于等于时间片的话，说明可以执行完毕，此时需要进行插入、删除、更新操作
                // 将该节点从就绪队列中删除
                PCB* pcb = *it;
                it = _list.erase(it); // 注意迭代器失效问题，要让it重新赋值

                // 更新该节点的属性
                int tmp_left_time = pcb->_left_time; // 先记录下剩余时间，下面要用
                pcb->_used_time += pcb->_left_time;
                pcb->_left_time = 0; 
                if(pcb->_start_time == -1) 
                    pcb->_start_time = current_time; // 第一次运行该进程则设置其实际开始运行时间
                pcb->_end_time = current_time + tmp_left_time;
                pcb->_state = State::Run;
                
                cout << "当前执行进程为：" << pcb->_pid << "  相对运行时间为：" << current_time << "  该进程在该时间片内执行完毕，其属性如下所示：" << endl;
                pcb->show();
                pcb->_state = State::Finish; // 最后设置为结束状态
                
                // 别忘了更新当前时间
                current_time += tmp_left_time;
                ret.push_back(pcb);
            }
        }
        cout << endl;
    }

    cout << endl << "最后各进程的状态如下所示： " << endl;
    for(auto& e : ret)
        e->show();
    cout << "*********************************************" << endl;
}

// 创建多级队列中的PCB，其实就是换成插入到多级队列中
void PCBManager::CreateMultiQueue(uint16_t pid, std::string name, uint16_t priority, time_t left_time)
{
    PCB* ptr = new PCB(pid, name, priority, left_time);
    if(ptr == nullptr)
    {
        std::cout << "创建PCB失败" << std::endl;
        return;
    }
    _multi_list[0].push_back(ptr);
    process_num++;
}

// 多级反馈队列调度算法
void PCBManager::MultiLevelFeedbackQueue()
{
    /*
        其基本思想是，把就绪进程按优先级排成多个队列，同队列的进程具有相同的时间片。
        高优先级队列的时间片比低优先级队列的小。调度时先从高优先级队列中选出某一进程投入运行，
        当该进程的时间片到期后则转至低一级的就绪队列中。只有高优先级队列为空时才从低一级队列中调度进程。

        1. 这里设置3个队列，创建进程的时候往最高优先级的队列中的队尾插入进程即可
        2. 队列都采用FCFS策略进行等待，采用RR方式运行，时间片按照优先级的降低而变大
        3. 处理完高优先级的队列再处理低优先级的队列，如果运行过程发现出现优先级更高的进程则采用抢占式处理高优先级的进程
        4. 仅当当前队列空闲的时候，才到下一层队列中的进程运行
    */
    cout << "*********************************************" << endl;
    cout << "多级反馈队列调度算法：" << endl;
    std::vector<PCB*> ret;

    // 因为这里是模拟算法，所以就以进程个数小于等于0时终止进程，实际上是死循环才对
    int current_time = 0;
    int pre_size = process_num; // 用于记录该进程减少时候的进程个数
    while(process_num > 0)
    {
        int level = 0;
        while(level < MULTI_QUEUE_NUM)
        {
            time_t chip = MULTI_TIMER_CHIP[level]; // 每层的时间片
            cout << "当前为第" << level + 1 << "层，时间片为：" << chip << endl;
                
            while(!_multi_list[level].empty())
            {
                // 开始从头遍历链表每个PCB，每个PCB获得一个时间片也就是TIMER_CHIP的运行时间
                auto it = _multi_list[level].begin();
                while(it != _multi_list[level].end())
                {
                    // 判断时间片和该PCB剩余执行时长的关系
                    if((*it)->_left_time > chip)
                    {
                        // 如果大于时间片的话，说明还不能执行完，则简单的更新时间就行
                        if((*it)->_start_time == -1) 
                            (*it)->_start_time = current_time; // 第一次运行该进程则设置其实际开始运行时间
                        (*it)->_used_time += chip;
                        (*it)->_left_time -= chip;
                        (*it)->_state = State::Run;

                        cout << "当前执行进程为：" << (*it)->_pid << "  相对运行时间为：" << current_time << "  该进程还没运行完，其属性如下所示：" << endl;
                        (*it)->show();
                        current_time += chip;
                        (*it)->_state = State::Wait; // 打印完再设置为等待状态

                        // 如果不是最后一层的话，则将it放到下一层中去，是的话则不需要
                        if(level != MULTI_QUEUE_NUM - 1)
                        {
                            _multi_list[level + 1].push_back(*it);
                            it = _multi_list[level].erase(it); // 注意迭代器失效问题，要让it重新赋值
                        }
                    }
                    else
                    {
                        // 如果小于等于时间片的话，说明可以执行完毕，此时需要进行插入、删除、更新操作
                        // 将该节点从就绪队列中删除
                        PCB* pcb = *it;
                        it = _multi_list[level].erase(it); // 注意迭代器失效问题，要让it重新赋值

                        // 更新该节点的属性
                        int tmp_left_time = pcb->_left_time; // 先记录下剩余时间，下面要用
                        pcb->_used_time += pcb->_left_time;
                        pcb->_left_time = 0; 
                        if(pcb->_start_time == -1) 
                            pcb->_start_time = current_time; // 第一次运行该进程则设置其实际开始运行时间
                        pcb->_end_time = current_time + tmp_left_time;
                        pcb->_state = State::Run;
                        
                        cout << "当前执行进程为：" << pcb->_pid << "  相对运行时间为：" << current_time << "  该进程在该时间片内执行完毕，其属性如下所示：" << endl;
                        pcb->show();
                        pcb->_state = State::Finish; // 最后设置为结束状态
                        
                        // 别忘了更新当前时间
                        current_time += tmp_left_time;
                        process_num--; 
                        pre_size--;
                        ret.push_back(pcb);
                    }
                }
                cout << endl;
            }
            level++;
        }
    }
    
    cout << endl << "最后各进程的状态如下所示： " << endl;
    for(auto& e : ret)
        e->show();
    cout << "*********************************************" << endl;
}
