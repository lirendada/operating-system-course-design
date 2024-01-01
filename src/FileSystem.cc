#include "../include/FileSystem.h"

FileSystem::FileSystem()
{
    std::ifstream in("./filesequence.txt");
    if (!in.is_open()) 
    {
        std::cerr << "Failed to open file." << std::endl;
        abort();
    }

    std::string line;
    while (std::getline(in, line)) 
    {
        std::stringstream ss(line);
        int num;
        while (ss >> num) 
        {
            _sequence.push_back(num);
        }
    }
    in.close();
    std::cout << "文件中的读取序列读入成功！" << std::endl;
}

// 打印磁道服务顺序以及移动总道数
void FileSystem::show_result(std::vector<int>& ret, int sum)
{
    cout << "移动总道数为：" << sum << "  平均寻道长度：" << ((double)sum)/ret.size() << endl;
    cout << "磁道服务顺序为：";
    for(int i = 0; i < ret.size(); ++i)
    {
        cout << ret[i];
        if(i != ret.size() - 1)
            cout << "->";
    }
    cout << endl;
}    

// 先来先服务算法
void FileSystem::FCFS()
{
    cout << "******************************************************************************************" << endl;
    cout << "FIFO先来先服务算法：" << endl;
    cout << "请输入起始磁道>> ";
    int cur; // 当前指向的磁道
    std::cin >> cur;
    int n = _sequence.size();

    std::vector<int> ret; // 存放磁道服务顺序
    int sum = 0; // 移动总道数
    
    cout << "----------------------------------------" << endl;
    cout << "|  被访问的下一个磁道号    移动距离    |  " << endl;
    for(int i = 0; i < _sequence.size(); ++i)
    {
        // 更新磁道服务顺序、移动总道数，并不难
        int gap = abs(cur - _sequence[i]);
        sum += gap;
        cur = _sequence[i];
        ret.push_back(_sequence[i]);

        // 进行数据打印
        printf("|        %5d%13c%5d       |\n", _sequence[i], ' ', gap);
    }
    cout << "----------------------------------------" << endl;

    show_result(ret, sum);
    cout << "******************************************************************************************" << endl;
} 

// 最短寻道优先算法
void FileSystem::SSTF()
{
    cout << "******************************************************************************************" << endl;
    cout << "SSTF最短寻道优先算法：" << endl;
    cout << "请输入起始磁道>> ";
    int cur; // 当前指向的磁道
    std::cin >> cur;
    int n = _sequence.size();
    std::vector<int> tmp(_sequence.begin(), _sequence.end()); // 复制一份数组，因为需要修改内容

    std::vector<int> ret; // 存放磁道服务顺序
    int sum = 0; // 移动总道数
    
    cout << "----------------------------------------" << endl;
    cout << "|  被访问的下一个磁道号    移动距离    |  " << endl;
    for(int i = 0; i < n; ++i)
    {
        int Min = INT_MAX;  // 记录最小差值
        int Min_index = -1; // 记录差值最小磁道的下标

        // 这里采用遍历的方式查找差值最小的磁道
        for(int j = 0; j < n; ++j)
        {
            if(tmp[j] != -1 && abs(cur - tmp[j]) < Min)
            {
                Min = abs(cur - tmp[j]);
                Min_index = j;
            }
        }

        // 更新磁道服务顺序、移动总道数，并不难
        int gap = Min;
        sum += gap;
        cur = tmp[Min_index];
        ret.push_back(tmp[Min_index]);

        // 进行数据打印
        printf("|        %5d%13c%5d       |\n", tmp[Min_index], ' ', gap);

        // 别忘了将访问的磁道设为-1
        tmp[Min_index] = -1;
    }
    cout << "----------------------------------------" << endl;

    show_result(ret, sum);
    cout << "******************************************************************************************" << endl;
} 

// 电梯（扫描）算法
void FileSystem::SCAN()
{
    cout << "******************************************************************************************" << endl;
    cout << "SCAN扫描（电梯）算法：" << endl;
    cout << "请输入起始磁道>> ";
    int cur; // 当前指向的磁道
    std::cin >> cur;
    int n = _sequence.size();
    std::vector<int> tmp(_sequence.begin(), _sequence.end()); // 复制一份数组，因为需要修改内容
    std::vector<int> ret; // 存放磁道服务顺序
    int sum = 0; // 移动总道数

    // 先对磁道号进行从小到大排序
    std::sort(tmp.begin(), tmp.end(), [&](int a, int b){
        return a <= b;
    });

    cout << "----------------------------------------" << endl;
    cout << "|  被访问的下一个磁道号    移动距离    |  " << endl;
    bool direction = true; // true表示向右，false表示向左
    int position = cur;
    for (int i = 0; i < 2; ++i)
    {
        if (direction == true)
        {
            for (int j = 0; j < tmp.size(); ++j)
            {
                if (tmp[j] < position)
                    continue;
                int gap = abs(tmp[j] - cur);
                cur = tmp[j];
                sum += gap;
                ret.push_back(cur);

                // 进行数据打印
                printf("|        %5d%13c%5d       |\n", cur, ' ', gap);
            }
            direction = false;
        }
        else
        {
            for (int j = tmp.size() - 1; j >= 0; --j)
            {
                if (tmp[j] > position)
                    continue;
                int gap = abs(tmp[j] - cur);
                cur = tmp[j];
                sum += gap;
                ret.push_back(cur);

                // 进行数据打印
                printf("|        %5d%13c%5d       |\n", cur, ' ', gap);
            }
            direction = true;
        }
    }
    cout << "----------------------------------------" << endl;

    show_result(ret, sum);
    cout << "******************************************************************************************" << endl;
}  
