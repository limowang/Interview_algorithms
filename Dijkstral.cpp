// #include <vector>
// #include <iostream>
// #include <limits.h>
// #include <queue>

// using namespace std;


// vector<int> pro(vector<vector<int>>& nodes,int n,int start,vector<int>& path)
// {
//     vector<int> minDis(n,INT_MAX);
//     //存储最近距离路径的前一个节点
//     // vector<int> path(n);
//     // for(int i = 0;i < n;i++)
//     //     path[i] = i;

//     vector<bool> isVisited(n,false);

//     priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
//     minDis[start] = 0;
//     pq.push({0,start});

//     while(!pq.empty())
//     {
//         auto it = pq.top();
//         int curnode = it.second;
//         pq.pop();

//         //之前已经访问过
//         if(isVisited[it.second])
//             continue;
        
//         isVisited[it.second] = true;
//         for(int i = 0;i < n;i++)
//         {
//             if(i == it.second || nodes[curnode][i] == -1)
//                 continue;
//             if(minDis[curnode] + nodes[curnode][i] < minDis[i])
//             {
//                 pq.push({minDis[curnode] + nodes[curnode][i],i});
//                 path[i] = curnode;
//             }
//         }
//     }

//     return minDis;
// }

// int main()
// {
//     vector<vector<int>> nodes = {{0,2,INT_MAX,INT_MAX,1,INT_MAX},{2,0,INT_MAX,INT_MAX,INT_MAX,2},{INT_MAX,INT_MAX,0,4,INT_MAX,INT_MAX},
//                                  {INT_MAX,INT_MAX,4,0,3,INT_MAX},{1,INT_MAX,INT_MAX,3,0,INT_MAX},{INT_MAX,2,INT_MAX,INT_MAX,INT_MAX,0}};

//     vector<int> path(6);


//     vector<int> minDis = pro(nodes,6,0,path);
//     for(auto& it : minDis)
//         cout << it << endl;
//     return 0;
// }


#include <vector>
#include <iostream>
#include <limits.h>
#include <queue>

using namespace std;


vector<int> pro(vector<vector<int>>& nodes,int n,int start,vector<int>& path)
{
    vector<int> minDis(n,INT_MAX);
    //存储最近距离路径的前一个节点
    // vector<int> path(n);
    // for(int i = 0;i < n;i++)
    //     path[i] = i;

    vector<bool> isVisited(n,false);

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    minDis[start] = 0;
    pq.push({0,start});

    while(!pq.empty())
    {
        auto it = pq.top();
        int curnode = it.second;
        pq.pop();

        //之前已经访问过
        if(isVisited[it.second])
            continue;
        
        isVisited[it.second] = true;
        for(int i = 0;i < n;i++)
        {
            if(i == it.second || nodes[curnode][i] == -1)
                continue;
            if(minDis[curnode] + nodes[curnode][i] < minDis[i])
            {
                pq.push({minDis[curnode] + nodes[curnode][i],i});
                path[i] = curnode;
            }
        }
    }

    return minDis;
}

int main()
{
    vector<vector<int>> nodes = {{0,2,INT_MAX,INT_MAX,1,INT_MAX},{2,0,INT_MAX,INT_MAX,INT_MAX,2},{INT_MAX,INT_MAX,0,4,INT_MAX,INT_MAX},
                                 {INT_MAX,INT_MAX,4,0,3,INT_MAX},{1,INT_MAX,INT_MAX,3,0,INT_MAX},{INT_MAX,2,INT_MAX,INT_MAX,INT_MAX,0}};

    vector<int> path(6);


    vector<int> minDis = pro(nodes,6,0,path);
    for(auto& it : minDis)
        cout << it << endl;
    return 0;
}
