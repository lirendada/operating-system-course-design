#include "../include/VirtualMemory.h"

VirtualMemory::VirtualMemory()
{
    // 从文件中读入页面序列
    char buffer[1024] = { 0 };
    std::ifstream in("./sequence.txt", std::ifstream::in);
    if (!in.is_open()) 
    {
        std::cerr << "Failed to open file." << std::endl;
        abort();
    }
    in.getline(buffer, 1024);
    in.close();
    std::cout << "文件中的页面序列读入成功！" << std::endl;

    // 将序列转化为整型放到数组中
    for(int i = 0; buffer[i] != '\0'; ++i)
        _sequence.push_back(buffer[i] - '0');
}

// 打印页面序列
void VirtualMemory::show_sequence()
{
    cout << "页面序列为：";
    for(int i = 0 ;i < _sequence.size(); ++i)
    {
        cout << _sequence[i];
        if(i != _sequence.size() - 1)
            cout << "->";
    }
    cout << endl;
}

// 打印缺页次数和缺页序列
void VirtualMemory::show_eliminate(std::vector<int>& eliminate, int sum, std::list<int>& queue)
{
    cout << "缺页次数为：" << sum << endl;
    cout << "缺页序列为：";
    for(int i = 0; i < eliminate.size(); ++i)
    {
        if(eliminate[i] == -1)
            cout << "无";
        else
            cout << eliminate[i];
        
        if(i != eliminate.size() - 1)
            cout << "->";
    }
    cout << endl;

    cout << "最后队列中的元素为：";
    auto it = queue.begin();
    while(it != queue.end())
    {
        cout << *it;
        if((++it) != queue.end())
            cout << "->";
    }
    cout << endl;
} 

// 检查是否当前队列中是否存在当前要调度的页面
bool VirtualMemory::test_same_in_queue(std::list<int>& queue, int i, std::list<int>::iterator& out)
{
    auto it = queue.begin();
    while(it != queue.end())
    {
        if(*it == _sequence[i])
        {
            out = it; // 迭代器作为输出型参数
            return true;
        }
        ++it;
    }
    return false;
}

// 先进先出页面置换算法
void VirtualMemory::FIFO()
{
    cout << "******************************************************************************************" << endl;
    cout << "FIFO先进先出页面置换算法：" << endl;
    cout << "页面序列为：";
    show_sequence();
    cout << "请输入分配物理块的数量>> ";
    int count;
    std::cin >> count;

    int n = _sequence.size();
    std::vector<int> eliminate(n, -1); // 对应位置存放淘汰页面，-1表示没有淘汰
    int sum = 0; // 缺页总次数
    std::list<int> queue;
    for(int i = 0; i < n; ++i)
    {
        // 如果出现元素和队列中一样的，则直接跳过
        std::list<int>::iterator out; // 因为FIFO算法不涉及刷新旧页面，所以这里的out并没有作用
        if(test_same_in_queue(queue, i, out) == true)
            continue;
        else
        {
            // 如果没有出现相同的页面才继续操作
            // 先保证将前count个页面放入队列中，以后就控制队列中只能有count个元素
            if(i < count)
                queue.push_back(_sequence[i]);
            else
            {
                // 超过count个则进行队头弹出
                eliminate[i] = queue.front();
                queue.pop_front();
                queue.push_back(_sequence[i]);
                sum++;
            }
        }
    }

    show_eliminate(eliminate, sum, queue);
    cout << "******************************************************************************************" << endl;
} 

// 最佳页面置换算法
void VirtualMemory::OPT()
{
    cout << "******************************************************************************************" << endl;
    cout << "OPT最佳页面置换算法：" << endl;
    cout << "页面序列为：";
    show_sequence();
    cout << "请输入分配物理块的数量>> ";
    int count;
    std::cin >> count;
    int n = _sequence.size();
    std::vector<int> eliminate(n, -1); // 对应位置存放淘汰页面，-1表示没有淘汰
    int sum = 0; // 缺页总次数
    std::list<int> queue;

    // 先将页面序列的所有出现位置记录下来
    std::unordered_map<int, std::list<int>> hash;
    for(int i = 0; i < n; ++i)
    {
        auto it = hash.find(_sequence[i]);
        if(it == hash.end())
            hash[_sequence[i]] = std::list<int>{ i };
        else
            hash[_sequence[i]].push_back(i);
    }

    // 遍历序列
    for(int i = 0; i < n; ++i)
    {
        // 如果出现元素和队列中一样的，则直接跳过
        std::list<int>::iterator out; // 因为OPT算法同样不涉及刷新旧页面，所以这里的out并没有作用
        if(test_same_in_queue(queue, i, out) == true)
        {} // do nothing
        else
        {
            // 如果没有出现相同的页面才继续操作
            // 将前count个页面放入队列中，以后就控制队列中只能有count个元素
            if(i < count)
                queue.push_back(_sequence[i]);
            else
            {
                // 超过count个则进行选择，将未来最长时间内不会被访问的页面淘汰
                // 所以首先要先找count个页面中未来最长时间内不会被访问的页面
                auto target = queue.begin();
                int Max = -1;
                auto it = queue.begin();
                while(it != queue.end())
                {
                    // 如果出现链表为空的情况，说明该页面后面都不会访问了，则直接将其淘汰即可
                    if(hash[*it].empty())
                    {
                        target = it;
                        hash.erase(*it); // 别忘了将该大小的节点删掉
                        break;
                    }

                    // 如果其未来出现的位置比Max大的话，则记录这个页面
                    if(hash[*it].empty() || hash[*it].front() > Max)
                    {
                        target = it;
                        Max = hash[*it].front();
                    }
                    ++it;
                }

                // 此时target就是要淘汰的页面的迭代器
                eliminate[i] = *target;
                queue.erase(target);
                queue.push_back(_sequence[i]);
                sum++;
            }
        }
        
        // 别忘了当前页面出现的下标要去掉
        hash[_sequence[i]].pop_front();
    }

    show_eliminate(eliminate, sum, queue);
    cout << "******************************************************************************************" << endl;
}  

// 最近最久未使用算法
void VirtualMemory::LRU()
{
    cout << "******************************************************************************************" << endl;
    cout << "LRU最近最久未使用算法：" << endl;
    cout << "页面序列为：";
    show_sequence();
    cout << "请输入分配物理块的数量>> ";
    int count;
    std::cin >> count;
    int n = _sequence.size();
    std::vector<int> eliminate(n, -1); // 对应位置存放淘汰页面，-1表示没有淘汰
    int sum = 0; // 缺页总次数
    std::list<int> queue;

    std::unordered_map<int, std::list<int>::iterator> hash; // 存放页面号与其链表节点迭代器的映射

    // 遍历途中保持让最久未使用的页面放在队尾
    for(int i = 0; i < n; ++i)
    {
        // 如果出现元素和队列中一样的，则将其提到队头，作为最活跃的页面
        auto it = hash.find(_sequence[i]);
        if(it != hash.end())
            queue.splice(queue.begin(), queue, it->second); // 使用splice可以将it处节点移动到头部
        else
        {
            // 如果没有出现相同的页面才继续操作
            // 将前count个页面放入队列中，以后就控制队列中只能有count个元素，所以出现了count以上则需要进行LRU处理
            if(i >= count)
            {
                // 此时需要将最不活跃的页面提出，也就是队尾页面
                int back = queue.back();
                hash.erase(back);
                queue.pop_back();

                // 更新缺页次数和淘汰页
                sum++;
                eliminate[i] = back;
            }
            
            // 然后将新页面插入到队头，建立映射关系
            queue.push_front(_sequence[i]);
            hash[_sequence[i]] = queue.begin(); // 别忘了将映射关系建立
        }
    }

    show_eliminate(eliminate, sum, queue);
    cout << "******************************************************************************************" << endl;
}  
