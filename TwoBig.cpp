#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class A
{
public:
    void print()
    {
        cout << "hello" << endl;
    }
};

//一个字符串和一位相乘，模拟相乘过程
string step(string s,char c)
{
    int cnt = 0;
    int flag = c - '0';
    string res;

    int n = s.size();
    for(int i = n - 1;i >= 0;i--)
    {
        int tmp = s[i] - '0';
        int t = tmp * flag + cnt;
        res += (t % 10 + '0');
        cnt = (t / 10);
    }

    if(cnt != 0)
    {
        res += (cnt % 10) + '0';
    }

    //cout << res << endl;

    //res.reserve();
    int l = 0,r = res.size() -1;
    while(l < r)
    {
        swap(res[l++],res[r--]);
    }

    //cout << res << endl;
    return res;
}

//两个字符串相加
string addString(string s1,string s2)
{
    int cnt = 0;
    string res;
    int index1 = s1.size() - 1;
    int index2 = s2.size() - 1;

    while(index1 >= 0 && index2 >= 0)
    {
        int tmp = s1[index1--] - '0' + s2[index2--] - '0' + cnt;
        res += (tmp % 10) + '0';
        cnt = (tmp / 10);
    }

    while(index1 >= 0)
    {
        int tmp = s1[index1--] - '0' + cnt;
        res += (tmp % 10) + '0';
        cnt = (tmp / 10);
    }
    while(index2 >= 0)
    {
        int tmp = s2[index2--] - '0' + cnt;
        res += (tmp % 10) + '0';
        cnt = (tmp / 10);
    }

    if(cnt != 0)
    {
        res += (cnt % 10) + '0';
    }

    //res.reserve();
    int l = 0,r = res.size() -1;
    while(l < r)
    {
        swap(res[l++],res[r--]);
    }
    return res;
}

//大整数相乘
string pro(string s1,string s2)
{
    //模拟每一位相乘
    string res;
    string pre = "";
    int times = 0;
    for(int i = s2.size() - 1;i >= 0;i--)
    {
        string tmp = step(s1,s2[i]);
        //移位加0
        for(int i = 0;i < times;i++)
        {
            tmp += '0';
        }

        times++;
        if(pre == "")
        {
            pre = tmp;
        }else
        {
            pre = addString(pre,tmp);
        }
    }

    res = pre;
    return res;
}

int main()
{
    string s1 = "9999";
    string s2 = "9999";

    A a;
    int n = sizeof(a);
    cout << n << endl;

    cout << pro(s1,s2) << endl;
    system("pause");
    return 0;
}