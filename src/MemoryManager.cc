#include "../include/MemoryManager.h"

// 定长分区的分配算法
void MemoryManager::FixAreaAllocation()
{
    /*
        根据作业的大小，找到一个能满足要求的、尚未分配的分区，分配给该程序，
        然后将该表项中的状态设置为“已分配”即可，若未找到适合的，则拒绝分配内存。
    */
    cout << "******************************************************************************************" << endl;
    cout << "定长分区的分配算法：" << endl;
    _fix.show();
    cout << "输入作业的名称、所需的内存大小>> ";
    std::string name;
    size_t size;
    std::cin >> name >> size;

    for(int i = 0; i < FIXED_AREA_NUM; ++i)
    {
        if(size <= _fix._fixed_area[i]->_length && _fix._fixed_area[i]->_used == false)
        {
            // 找到了合适并且空闲的，则直接分配，并且修改属性
            cout << "找到了合适并且空闲的分区！分区号为：" << _fix._fixed_area[i]->_id << 
            " 分区大小为：" << _fix._fixed_area[i]->_length 
            << " 起始地址为：" << _fix._fixed_area[i]->_start << endl;

            _fix._fixed_area[i]->_name = name;
            _fix._fixed_area[i]->_used = true;

            cout << endl << "此时固定分区表状态为：" << endl;
            _fix.show();
            cout << "******************************************************************************************" << endl;
            return;
        }
    }

    // 走到这说明没找到，则拒绝分配内存
    cout << "很抱歉当前没有适合该作业的分区，无法分配内存!!!" << endl;
    cout << "******************************************************************************************" << endl;
}

// 定长分区的释放算法
void MemoryManager::FixAreaRelease()
{
    cout << "******************************************************************************************" << endl;
    cout << "定长分区的释放算法：" << endl;
    cout << "输入要释放的作业名>> ";
    std::string name;
    std::cin >> name;

    for(int i = 0; i < FIXED_AREA_NUM; ++i)
    {
        if(!_fix._fixed_area[i]->_name.empty() && name.compare(_fix._fixed_area[i]->_name) == 0)
        {
            // 找到了对应的内存分区
            cout << "找到了对应的内存分区！分区号为：" << _fix._fixed_area[i]->_id << 
            " 分区大小为：" << _fix._fixed_area[i]->_length 
            << " 起始地址为：" << _fix._fixed_area[i]->_start << endl;

            _fix._fixed_area[i]->_name.clear();
            _fix._fixed_area[i]->_used = false;

            cout << "释放成功！！！" << endl;
            cout << endl << "此时固定分区表状态为：" << endl;
            _fix.show();
            cout << "******************************************************************************************" << endl;
            return;
        }
    }

    // 走到这说明没找到，则不需要释放
    cout << "当前作业" << name << "并没有分配内存分区，不需要释放!!!" << endl;
    cout << "******************************************************************************************" << endl;
}

// 变长分区的分配算法
void MemoryManager::VariableAreaAllocation()
{
    /*
        根据作业的大小，找到一个能满足要求的、尚未分配的分区，分配给该程序，
        然后将该表项中的状态设置为“已分配”即可，若未找到适合的，则拒绝分配内存。
    */
    cout << "******************************************************************************************" << endl;
    cout << "变长分区的分配算法：" << endl;
    _var.show();
    cout << "输入作业的名称、所需的内存大小>> ";
    std::string name;
    size_t size;
    std::cin >> name >> size;
    if(size <= 0)
    {
        cout << "输入错误，size不能小于等于0！！！" << endl;
        return;
    }

    for(int i = 0; i < _var._free_variable_area.size(); ++i)
    {
        // 不需要判断是否空闲，因为空闲表存放的就是空闲的分区 
        if(size <= _var._free_variable_area[i]->_length)
        {
            // 找到了合适并且空闲的，则直接分配，并且修改属性
            cout << "找到了合适并且空闲的分区！分区号为：" << _var._free_variable_area[i]->_id << 
            " 分区大小为：" << _var._free_variable_area[i]->_length 
            << " 起始地址为：" << _var._free_variable_area[i]->_start << endl;

            // 将分区号为id的分区，根据size大小切割，然后分配到对应分区表中
            // 1. 将该分区低地址处开始切，开辟一个新区间，插入到已分配链表中
            Area* newarea = new Area(_var._used_num++, _var._free_variable_area[i]->_start, size, true);
            newarea->_name = name;
            newarea->_prev = _var._free_variable_area[i]->_prev; // 先让新区域的pre指向原分区的pre，方便后面合并
            if(_var._free_variable_area[i]->_prev != nullptr)
                _var._free_variable_area[i]->_prev->_next = newarea; // 别忘了让前置节点改变指向

            // 2. 更新一下当前分区的属性
            _var._free_variable_area[i]->_start += size;
            _var._free_variable_area[i]->_length -= size;
            if(_var._free_variable_area[i]->_length <= 0)
            {
                newarea->_next = _var._free_variable_area[i]->_next;
                if(_var._free_variable_area[i]->_next != nullptr)
                    _var._free_variable_area[i]->_next->_prev = newarea;

                // 如果长度变为小于0了，则删除原分区
                _var._free_variable_area.erase(_var._free_variable_area.begin() + i);
                delete _var._free_variable_area[i]; // 注意要释放内存，不然会内存泄漏
            }
            else
            {
                // 否则就让新分区的next指向当前分区，并且更新当前分区的prev指向
                newarea->_next = _var._free_variable_area[i];
                _var._free_variable_area[i]->_prev = newarea;

                // 并且要对未分配空间按照长度大小重新排序
                std::sort(_var._free_variable_area.begin(), _var._free_variable_area.end(), [&](const Area* a1, const Area* a2){
                    return a1->_length < a2->_length;
                });
            }
            _var._used_variable_area.push_back(newarea);

            // 最后打印表的信息
            cout << endl << "此时变长分区表状态为：" << endl;
            _var.show();
            cout << "******************************************************************************************" << endl;
            return;
        }
    }

    // 走到这说明没找到，则拒绝分配内存
    cout << "很抱歉当前没有适合该作业的分区，无法分配内存!!!" << endl;
    cout << "******************************************************************************************" << endl;
}              
    
// 变长分区的释放算法
void MemoryManager::VariableAreaRelease()
{
    /*
        将分区合并回空闲表中，判断前后是否存在关系进行不同位置的合并
    */
    cout << "******************************************************************************************" << endl;
    cout << "变长分区的释放算法：" << endl;
    cout << "输入要释放的作业名>> ";
    std::string name;
    std::cin >> name;

    for(int i = 0; i < _var._used_variable_area.size(); ++i)
    {
        if(!_var._used_variable_area[i]->_name.empty() && name.compare(_var._used_variable_area[i]->_name) == 0)
        {
            // 1. 找到了对应的内存分区
            cout << "找到了对应要释放的内存分区！分区号为：" << _var._used_variable_area[i]->_id << 
            " 分区大小为：" << _var._used_variable_area[i]->_length 
            << " 起始地址为：" << _var._used_variable_area[i]->_start << endl;

            // 2. 先将其从分配表中删除
            Area* cur = _var._used_variable_area[i];
            _var._used_variable_area.erase(_var._used_variable_area.begin() + i);

            // 3. 进行内存分区的合并，分情况讨论
            //  3.1 先拿到空闲表中对应的前后分区，只有存在并且在空闲表才拿
            Area* prev = nullptr;
            Area* next = nullptr;
            if(cur->_prev != nullptr && cur->_prev->_used == false) prev = cur->_prev;
            if(cur->_next != nullptr && cur->_next->_used == false) next = cur->_next;

            //  3.2 分情况讨论
            if(prev != nullptr && next != nullptr)
            {
                // 说明要将前后都合并起来：直接修改prev的属性，然后释放当前节点和next即可
                prev->_length += cur->_length + next->_length;
                prev->_next = next->_next;

                // 还要记得将next从空闲表中pop掉
                for(int i = 0; i < _var._free_variable_area.size(); ++i)
                {
                    if(_var._free_variable_area[i] == next)
                        _var._free_variable_area.erase(_var._free_variable_area.begin() + i);
                }
                delete cur;
                delete next;
            }
            else if(prev != nullptr)
            {
                // 说明要与前面的分区合并：直接修改prev的属性，然后释放当前节点
                prev->_length += cur->_length;
                prev->_next = cur->_next;
                delete cur;
            }
            else if(next != nullptr)
            {
                // 说明要与后面的分区合并：直接修改当前节点属性，然后释放next
                cur->_length += next->_length;
                cur->_next = next->_next;
                if(next->_next != nullptr)
                    next->_next->_prev = cur;
                cur->_name.clear();
                cur->_used = false; // 别忘了状态修改
                _var._free_variable_area.push_back(cur);

                // 还要记得将next从空闲表中pop掉
                for(int i = 0; i < _var._free_variable_area.size(); ++i)
                {
                    if(_var._free_variable_area[i] == next)
                        _var._free_variable_area.erase(_var._free_variable_area.begin() + i);
                }
                delete next;
            }
            else
            {
                // 说明要单独作为一个分区
                cur->_id = _var._free_num++;
                cur->_name.clear();
                cur->_used = false;
                _var._free_variable_area.push_back(cur);
            }

            // 4. 要对未分配空间按照长度大小重新排序
            std::sort(_var._free_variable_area.begin(), _var._free_variable_area.end(), [&](const Area* a1, const Area* a2){
                return a1->_length < a2->_length;
            });

            // 最后打印表的信息
            cout << "释放成功！！！" << endl;
            cout << endl << "此时变长分区表状态为：" << endl;
            _var.show();
            cout << "******************************************************************************************" << endl;
            return;
        }
    }

    // 走到这说明没找到，则不需要释放
    cout << "当前作业" << name << "并没有分配内存分区，不需要释放!!!" << endl;
    cout << "******************************************************************************************" << endl;
}