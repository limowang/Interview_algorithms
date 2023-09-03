//换钱的最少货币数，arr数组表示每类货币的面值，
//dp[i][k] ----> 表示用前i张货币组成k元的最少货币数量
//dp[i + 1][k] = dp[i][k];min(dp[i + 1][k],i + dp[i][k - i * arr[i + 1]]) <-------- 动态转移方程

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int LeastWaysOfMoney(vector<int>& arr,int n)
{
    int size = arr.size();
    sort(arr.begin(),arr.end());
    vector<vector<int>> dp(size,vector<int>(n + 1,-1));

    for(int i = 1;i * arr[0] <= n;i++)
        dp[0][i * arr[0]] = i;

    for(int i= 0;i < size;i++)
        dp[i][0] = 0;

    for(auto& it : dp[0])
        cout << it << " ";
    cout << endl;
    
    for(int i = 1;i < size;i++)
    {
        for(int j = 1;j <= n;j++)
        {
            dp[i][j] = dp[i - 1][j];
            for(int k = 1;k * arr[i] <= j;k++)
            {
                int tmp = j - k * arr[i];
                if(dp[i - 1][tmp] != -1)
                {
                    dp[i][j] = dp[i][j] == -1 ? dp[i - 1][tmp] + k : min(dp[i - 1][tmp] + k,dp[i][j]);
                }
            }
        }
    }

    return dp[size - 1][n];
}


int main()
{
    vector<int> arr = {5,2,3};
    cout << LeastWaysOfMoney(arr,20) << endl;
    system("pause");
    return 0;
}