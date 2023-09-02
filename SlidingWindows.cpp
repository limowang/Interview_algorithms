#include <queue>
#include <iostream>
using namespace std;

//生成窗口最大值数组
//{4,3,5,4,3,3,6,7} -> {5,5,5,4,6,7}
//滑动窗口来解决

//w表示窗口大小, w > 0 && w <= nums.size()
vector<int> pro(vector<int>& nums,int w)
{
    int n = nums.size();
    vector<int> res(n - w + 1);
    //存储的是下标
    deque<int> SlicdingQueue;
    int index = 0;

    for(int i = 0;i < n;i++)
    {
        while(!SlicdingQueue.empty() && nums[SlicdingQueue.back()] <= nums[i])
            SlicdingQueue.pop_back();

        //合法情况放入
        SlicdingQueue.push_back(i);

        if(SlicdingQueue.front() == i - w)
            SlicdingQueue.pop_front();

        if(i - w >= -1)
            res[index++] = nums[SlicdingQueue.front()];
    }

    return res;
}

int main()
{
    vector<int> nums = {4,3,5,4,3,3,6,7};

    vector<int> res = pro(nums,3);
    for(auto&  it : res)
        cout << it << endl;
    return 0;
}

