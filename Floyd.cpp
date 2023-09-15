//���������㷨�����ȫԱ���·������ --------�����ڽӾ���汾
#include <vector>
#include <limits.h>
#include <iostream>
using namespace std;

//gridΪ�ڽӾ���nΪ�������
vector<vector<int>> Floyd(vector<vector<int>>& grid,int n)
{
    vector<vector<int>> distance(n,vector<int>(n,INT_MAX));

    //ö��ÿһ������
    for(int k = 0;k < n;k++)
        for(int i = 0;i < n;i++)
            for(int j = 0;j < n;j++)
            {
                if(distance[i][k] != INT_MAX && distance[k][j] != INT_MAX && distance[i][k] + distance[k][j] < distance[i][j])
                    distance[i][j] = distance[i][k] + distance[k][j];
            }
    
    return distance;
}
