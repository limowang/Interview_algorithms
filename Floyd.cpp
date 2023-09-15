//弗洛伊德算法，解决全员最短路径问题 --------基于邻接矩阵版本
#include <vector>
#include <limits.h>
#include <iostream>
using namespace std;

//grid为邻接矩阵，n为顶点个数
vector<vector<int>> Floyd(vector<vector<int>>& grid,int n)
{
    vector<vector<int>> distance(n,vector<int>(n,INT_MAX));

    //枚举每一个顶点
    for(int k = 0;k < n;k++)
        for(int i = 0;i < n;i++)
            for(int j = 0;j < n;j++)
            {
                if(distance[i][k] != INT_MAX && distance[k][j] != INT_MAX && distance[i][k] + distance[k][j] < distance[i][j])
                    distance[i][j] = distance[i][k] + distance[k][j];
            }
    
    return distance;
}
