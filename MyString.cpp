#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

class MyString
{
public:
    MyString(const char* str = nullptr);
    MyString(const MyString& r);
    MyString(MyString &&other);        //移动构造函数

    MyString& operator=(const MyString& x);
    MyString& operator=(MyString&& x);

private:
    char* m_data;
};


MyString::MyString(const char* str)
{
    if(str == nullptr)
    {
        m_data = new char[1];
        *m_data = '\0';
        cout << "默认构造函数" << endl;
    }else
    {
        int len = strlen(str);
        m_data = new char[len + 1];
        strcpy(m_data,str);
        cout << "有参构造" << endl;
    }
}

MyString::MyString(const MyString& r)
{
    int len = strlen(r.m_data);
    m_data = new char[len + 1];
    strcpy(m_data,r.m_data);
    cout << "拷贝构造" << endl;
}

MyString::MyString(MyString&& other)
{
    //窃取
    m_data = other.m_data;
    other.m_data = nullptr;
    cout << "移动构造" << endl;
}

MyString& MyString::operator=(const MyString& x)
{
    //非自赋值
    if(this != &x)
    {
        if(!m_data)
            delete[] m_data;
        int len = strlen(x.m_data);
        m_data = new char[len + 1];
        strcpy(m_data,x.m_data);
    }

    cout << "拷贝赋值" << endl;
    return *this;
}

MyString& MyString::operator=(MyString&& x)
{
    if(this != &x)
    {
        delete[] m_data;
        m_data = x.m_data;
        x.m_data = nullptr;
    }

    cout << "移动赋值" << endl;
    return *this;
}

int main()
{
    //system("chcp 69001");
    MyString s1;                //默认构造
    MyString s2("hello");       //参数构造
    MyString s3(s2);            //拷贝构造
    MyString s4(move(s3));      //移动构造
    MyString s5;
    s5 = s4;                    //拷贝赋值
    MyString s6;
    s6 = move(s5);              //移动赋值

    system("pause");
    return 0;
}