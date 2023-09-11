#include<bits/stdc++.h>
using namespace std;

struct fast_ios {//取消同步，快速IO，让cin/cout比printf/scanf更快
    fast_ios() {
        cin.tie(nullptr),
        ios::sync_with_stdio(false);
    };
}fast_ios_;

vector<int> split_to_vi(const string &str, const char &delim) {
    //将字符串按delim分割为若干个子串(整数)，插入到数组中
    vector<int> res;
    stringstream ss(str);
    string tmp;
    while(getline(ss, tmp, delim)) 
    {//按delim分割
        if(!tmp.empty()) res.push_back(stoi(tmp));
    }
    return res;
}

vector<string> split_to_vs(const string &str, const char &delim, bool flag = false) {
    //将字符串按delim分割为若干个子串，flag为true时，即word的前后带有双引号
    vector<string> res;
    stringstream ss(str);
    string tmp;
    while(getline(ss, tmp, delim)) 
    {//按delim分割
        if(!tmp.empty()) 
        {
            if(flag)  res.emplace_back(tmp.substr(1, tmp.size() - 2));
            else res.emplace_back(tmp);
        }
    }
    return res;
}

void vecInt_in(){
//整数输入，以逗号分隔，eg：1,2,3,4,5
    vector<int> arr;
    string str;
    cin>>str;
    arr = split_to_vi(str, ',');
    for(int i : arr) cout << i << " ";
    cout << endl;
}

void vecStr_in(){
//字符串输入，以逗号分隔，eg：hello,world,hello,uestc,hello,wavelab
    vector<string> vstrs;
    string str;
    cin>>str;
    vstrs = split_to_vs(str, ',');
    for(const string &s : vstrs) cout << s << " ";
    cout << endl;
}

void lessInput(){

    int i;
    vector<int> arr;
    while(cin >> i)
    {
        arr.push_back(i);
        if(cin.get() == '\n') break;
    }

    vector<int> brr;
    while(cin >> i)
    {
        brr.push_back(i);
    }

}

void matrix_In(vector<vector<int>> &matrix, int row = 1, int col = 1)
{
    for(int i = 0; i < row; ++i)
    {
        for(int j = 0; j < col; ++j) cin >> matrix[i][j];
    }
}

void lotsOfInput(bool flag = false)
{
    int T;//测试数量
    cin >> T;
    if(!flag)
    {//规模固定
        int n, m;
        cin >> n >> m;
        for(int i = 0; i < T; ++i)
        {
            vector<vector<int>> matrix(n, vector<int>(m, 0));
            matrix_In(matrix, n, m);
        }
    }else{
        int n, m;
        for(int i = 0; i < T; ++i)
        {
            cin >> n >> m;
            vector<vector<int>> matrix(n, vector<int>(m, 0));
            matrix_In(matrix, n, m);
        }
    }
}

void vecStr_in_Plus(){
//字符串输入(带双引号)，以逗号分隔，eg："hello","world","hello","uestc","hello","wavelab"
    vector<string> vstrs;
    string str;
    cin>>str;
    vstrs = split_to_vs(str, ',', true);
    for(const string &s : vstrs) cout << s << " ";
    cout << endl;
}


int main(int argc, char** argv){
    int start = clock();
    /**************************Test IO*********************************/
    //1、整数输入，以逗号分隔，eg：1,2,3,4,5
    //vecInt_in();

    //2、字符串输入，以逗号分隔，eg：hello,world,hello,uestc,hello,wavelab
    //vecStr_in();
    //vecStr_in_Plus();//word带双引号"hello"

    //3、多组测试数据(默认以空格分隔)
    lessInput();
    lotsOfInput();

    /******************************************************************/
    printf("%.3lf\n",double(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}