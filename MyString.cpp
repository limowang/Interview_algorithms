#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

class MyString
{
public:
    MyString(const char* str = nullptr);
    MyString(const MyString& r);
    MyString(MyString &&other);        //�ƶ����캯��

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
        cout << "Ĭ�Ϲ��캯��" << endl;
    }else
    {
        int len = strlen(str);
        m_data = new char[len + 1];
        strcpy(m_data,str);
        cout << "�вι���" << endl;
    }
}

MyString::MyString(const MyString& r)
{
    int len = strlen(r.m_data);
    m_data = new char[len + 1];
    strcpy(m_data,r.m_data);
    cout << "��������" << endl;
}

MyString::MyString(MyString&& other)
{
    //��ȡ
    m_data = other.m_data;
    other.m_data = nullptr;
    cout << "�ƶ�����" << endl;
}

MyString& MyString::operator=(const MyString& x)
{
    //���Ը�ֵ
    if(this != &x)
    {
        if(!m_data)
            delete[] m_data;
        int len = strlen(x.m_data);
        m_data = new char[len + 1];
        strcpy(m_data,x.m_data);
    }

    cout << "������ֵ" << endl;
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

    cout << "�ƶ���ֵ" << endl;
    return *this;
}

int main()
{
    //system("chcp 69001");
    MyString s1;                //Ĭ�Ϲ���
    MyString s2("hello");       //��������
    MyString s3(s2);            //��������
    MyString s4(move(s3));      //�ƶ�����
    MyString s5;
    s5 = s4;                    //������ֵ
    MyString s6;
    s6 = move(s5);              //�ƶ���ֵ

    system("pause");
    return 0;
}