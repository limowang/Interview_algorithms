#include <vector>
#include <string>
#include <iostream>

using namespace std;

//分割字符串
vector<string> splitString(const string& str,const string& delimiter)
{
    vector<string> tokens;
    size_t pos = 0;
    size_t prev = 0;

    while((pos = str.find(delimiter,prev)) != string::npos)
    {
        tokens.push_back(str.substr(prev,pos - prev));
        prev = pos + delimiter.size();
    }

    tokens.push_back(str.substr(prev));
    return tokens;
}

//解析http请求
void parseHttpRequest(const string& request)
{
    //分割请求行
    vector<string> lines = splitString(request,"\r\n");
    vector<string> requestLine = splitString(lines[0]," ");
    if(requestLine.size() >= 3)
    {
        cout << "Method: " << requestLine[0] << endl;
        cout << "Path: " << requestLine[1] << endl;
    }

    cout << "print Headers---------" << endl;
    //分割头部字段
    for(int i = 1;i < lines.size();i++)
    {
        vector<string> header = splitString(lines[i],": ");
        if(header.size()  >= 2)
        {
            cout << "Header: " << header[0] << " - " << header[1] << endl;
        }
    }

    //找到正文的起始处
    size_t bodyStart = request.find("\r\n\r\n");
    if(bodyStart != string::npos)
    {
        string body = request.substr(bodyStart + 4);
        cout << "Body: " << body << endl;
    }
}

int main()
{
    string request = "GET /example HTTP/1.1\r\n"
                     "Host: www.example.com\r\n"
                     "Content-Type: application/json\r\n"
                     "\r\n"
                     "{\"key\": \"value\"}";
    
    parseHttpRequest(request);

    system("pause");
    return 0;
}