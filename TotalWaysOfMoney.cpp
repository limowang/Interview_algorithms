//换钱的总的方法数量
#include <vector>
#include <iostream>

using namespace std;

int TotalWaysOfMoney(vector<int>& arr,int n)
{
    int size = arr.size();
    vector<vector<int>> dp(size,vector<int>(n + 1,-1));

    for(int i = 1;i * arr[0] <= n;i++)
        dp[0][i * arr[0]] = 1;
    for(int i = 0;i < size;i++)
        dp[i][0] = 1;
    
    for(int i = 1;i < size;i++)
        for(int j = 1;j <= n;j++)
        {
            dp[i][j] = dp[i - 1][j];

            for(int k = 1;k * arr[i] <= j;k++)
            {
                int tmp = j - k * arr[i];
                if(dp[i - 1][tmp] != -1)
                    dp[i][j] += dp[i - 1][tmp];
            }
        }
    
    return dp[size - 1][n];
}

int main()
{
    vector<int> arr = {5,10,25,1};

    cout << TotalWaysOfMoney(arr,15) << endl;
    system("pause");
    return 0;
}