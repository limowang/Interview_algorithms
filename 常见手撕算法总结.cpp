#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <memory>
using namespace std;


// struct Dnode
// {
//     int value;
//     Dnode* prev;
//     Dnode* next;

//     Dnode(int v) : value(v),prev(nullptr),next(nullptr) {}
// };

// class DLinkList
// {
// private:
//     Dnode* head;
//     Dnode* tail;

// public:
//     DLinkList() : head(nullptr),tail(nullptr) {}
//     ~DLinkList()
//     {
//         Dnode* cur = nullptr;
//         while(head != nullptr)
//         {
//             cur = head;
//             head = cur -> next;
//             delete cur;
//             cur = nullptr;
//         }
//     }

//     //在表头插入一个节点
//     void addDnode(Dnode* node)
//     {
//         if(head == nullptr)
//         {
//             head = node;
//             tail = node;
//         }else
//         {
//             node -> next = head;
//             head -> prev = node;
//             head = node;
//         }
//     }

//     //将一个节点移到表头
//     void moveToHead(Dnode* node)
//     {
//         if(node == head)
//         {
//             return;
//         }

//         node -> prev -> next = node -> next;
//         node -> next -> prev = node -> prev;
//         node -> next = head;
//         head -> prev = node;
//         head = node;
//     }

//     //删除尾节点
//     Dnode* removeTail()
//     {
//         Dnode* cur = tail;
//         tail = cur -> prev;
//         cur -> prev = nullptr;
//         return cur;
//     }
// };

// class LRUCache
// {
// private:
//     int capacity;
//     DLinkList* recordList;
//     map<int,Dnode*> keyToNode;
//     map<Dnode*,int> NodeToKey;

// public:
//     LRUCache(int s) : capacity(s){recordList = new DLinkList();}
//     ~LRUCache()
//     {
//         delete recordList;
//     }

//     int get(int key)
//     {
//         if(keyToNode.find(key) == keyToNode.end())
//             return -1;
        
//         int res = keyToNode[key] -> value;
//         recordList->addDnode(keyToNode[key]);
//         return res;
//     }

//     void set(int key,int value)
//     {
//         if(keyToNode.find(key) != keyToNode.end())
//         {
//             //存在
//             auto it = keyToNode[key];
//             it -> value = value;
//             recordList->moveToHead(it);
//         }else
//         {
//             //插入
//             Dnode* cur = new Dnode(value);
//             keyToNode[key] = cur;
//             NodeToKey[cur] = key;

//             recordList->addDnode(cur);
//             if(keyToNode.size() > capacity)
//             {
//                 auto flag = recordList->removeTail();
//                 int k = NodeToKey[flag];
//                 NodeToKey.erase(flag);
//                 keyToNode.erase(k);
//             }
//         }
//     }
// };


// int main()
// {
//     LRUCache mcache(3);

//     mcache.set(1,10);
//     mcache.set(2,3);

//     cout << mcache.get(2) << endl;
//     mcache.set(3,5);
//     mcache.set(5,1);
//     cout << mcache.get(1) << endl;

//     system("pause");
//     return 0;
// }


//快速排序
// int partition(vector<int>& nums,int l,int r)
// {
//     int start = l,end = r;
//     int tmp = nums[l];

//     while(start < end)
//     {
//         while(end > start && nums[end] > tmp)
//             end--;
//         nums[start] = nums[end];
//         while(start < end && nums[start] <= tmp)
//             start++;
        
//         nums[end] = nums[start];
//     }

//     nums[start] = tmp;
//     return start;
// }

// int quickSort(vector<int>& nums,int l,int r)
// {
//     if(l < r)
//     {
//         int index = partition(nums,l,r);

//         quickSort(nums,0,index - 1);
//         quickSort(nums,index + 1,r);
//     }
// }

// int main()
// {
//     vector<int> nums = {4,1,6,95,2,25,6};

//     quickSort(nums,0,6);

//     for(auto& it : nums)
//         cout << it << endl;
//     system("pause");
//     return 0;
// }

//归并排序
// int mergeSort(vector<int>& nums,int l,int r)
// {
//     if(l < r)
//     {
//         int mid = (l + r) / 2;

//         mergeSort(nums,l,mid);
//         mergeSort(nums,mid + 1,r);

//         vector<int> tmp(r - l + 1);
//         int index = 0;
//         int lefts = l;
//         int rights = mid + 1;
//         while(lefts <= mid && rights <= r)
//         {
//             if(nums[lefts] < nums[rights])
//             {
//                 tmp[index++] = nums[lefts++];
//             }else
//             {
//                 tmp[index++] = nums[rights++];
//             }
//         }

//         while(lefts <= mid)
//             tmp[index++] = nums[lefts++];
//         while(rights <= r)
//             tmp[index++] = nums[rights++];
        
//         for(int i = 0;i < index;i++)
//             nums[l + i] = tmp[i];
//     }
// }

// int main()
// {
//     vector<int> nums = {4,1,6,95,2,25,6};

//     mergeSort(nums,0,6);

//     for(auto& it : nums)
//         cout << it << endl;
//     system("pause");
//     return 0;
// }

// void print()
// {
//     cout << "empty" << endl;
// }

// //模板实现递归参数
// template  <typename T,typename... Args>
// void print(T head,Args... args)
// {
//     cout << head << ",";
//     print(args...);
// }

// int main()
// {
//     print(1,2,3,4);

//     system("pause");
//     return 0;
// }

//循环引用代码示例
// class B;
// class A
// {
// public:
//     shared_ptr<B> pb_;

// public:
//     ~A()
//     {
//         cout << "A delete" << endl;
//     }
// };

// class B
// {
// public:
//     shared_ptr<A> pa_;

// public:
//     ~B()
//     {
//         cout << "B delete" << endl;
//     }
// };

// void fun()
// {
//     shared_ptr<A> pa(new A());
//     shared_ptr<B> pb(new B());
//     pa -> pb_ = pb;
//     pb -> pa_ = pa;

//     cout << pa.use_count() << endl;
//     cout << pb.use_count() << endl;
// }

// int main()
// {
//     fun();

//     system("pause");
//     return 0;
// }


// class A
// {
// public:
//     int *d;
//     static int n_castr;
//     static int n_dastr;
//     static int n_cptr;

//     A() : d(new int(0))
//     {
//         cout << "Construct: " << ++n_castr << endl;
//     }

//     A(const A& r)
//     {
//         d = new int(*r.d);
//         cout << "Copy construct: " << ++n_cptr << endl;
//     }
//     ~A()
//     {
//         cout << "Destuct: " << ++n_dastr << endl;
//         system("pause");
//     }

//     void print()
//     {
//         cout << "I lie" << endl;
//     }
// };

// int A::n_castr = 0;
// int A::n_cptr = 0;
// int A::n_dastr = 0;

// A getA()
// {
//     return A();
// }

// class B
// {
// public:
//     B()
//     {
//         cout << "i lie" << endl;
//     }

//     ~B()
//     {
//         cout << "i die" << endl;
//         system("pause");
//     }
// };


// int main()
// {
//     A a = getA();
//     a.print();

//     //B b;

//     //system("pause");
//     return 0;
// }



// template <typename T>
// class FreeLockStack
// {
// private:
//     struct node
//     {
//         T data;
//         node* next;
//         node(const T& x) : data(x),next(nullptr) {}
//     };

//     node* top;

// public:
//     FreeLockStack() : top(nullptr) {}
//     ~FreeLockStack()
//     {
//         while(top)
//         {
//             node* next = top -> next;
//             delete top;
//             top = next;
//         }
//     }

//     void push(const T& data)
//     {
//         node* newNode = new node(data);

//         while(true)
//         {
//             newNode -> next = top;
//             if(CAS(&top,newNode -> next,newNode))
//             {
//                 cout << data << " push successfully" << endl;
//                 break;
//             }
//         }
//     }

//     T pop() throw()
//     {
//         while(true)
//         {
//             node* res = top;
//             if(res == nullptr)
//             {
//                 throw string("Cannot pop from empty stack");
//             }
//             if(top && CAS(&top,res,res -> next))
//             {
//                 cout << "pop successfully! -> " << res -> data << endl;
//             }

//             return res -> data;
//         }
//     }
    
// };

// void func()
// {
//     for(int i = 0;i < 100;i++)
//     {
//         cout << "线程函数运行中" << endl;
//     }
// }

FreeLockQueue<int> mFQ;

void producer()
{
    for(int i = 0;i < 100;i++)
    {
        mFQ.enqueue(i);
        //cout << "Enqueue data: " << i << "successfully!" << endl;
    }
}

void consumer()
{
    while(true)
    {
        int d = mFQ.dequeue();
        if(d != -1)
            cout << "Get data: " << d << "successfully!" << endl;
        
    }
}

//无锁队列的实现
template <typename T>
bool CAS(T* ptr,T oldVal,T newVal)
{
    if(*ptr == oldVal)
    {
        *ptr = newVal;
        return true;
    }
    return false;
}

template <typename T>
class FreeLockQueue
{
private:
    struct Node
    {
        T data;
        Node* next;
        Node(const T& x) : data(x),next(nullptr) {}
    };

    Node* head;
    Node* tail;
public:
    FreeLockQueue()
    {
        Node* node = new Node(-1);
        head = node;
        tail = node;
    }

    ~FreeLockQueue()
    {
        while(head)
        {
            Node* tmp = head -> next;
            delete head;
            head = tmp;
        }
    }

    void enqueue(const T& x)
    {
        Node* cur = new Node(x);
        Node* t = nullptr;
        while(true)
        {
            t = tail;
            Node* next = t -> next;

            if(t != tail)
            {
                continue;
            }

            if(next != nullptr)
            {
                CAS(&tail,t,next);
                continue;
            }

            if(CAS(&t -> next,next,cur))
                break;
        }
        CAS(&tail,t,cur);
    }

    T dequeue()
    {
        T d;
        Node* h = nullptr;
        while(true)
        {
            h = head;
            Node* t = tail;
            Node* next = h -> next;

            //head指针已经移动
            if(h != head)
                continue;
            //如果队列为空的话
            if(h == t && next == nullptr)
            {
                cout << "Can not dequeue from empty queue" << endl;
                return -1;
            }

            //尾指针没有更新
            if(h == t && next != nullptr)
            {
                CAS(&tail,t,next);
                continue;
            }

            if(CAS(&head,h,next))
            {
                d = next -> data;
                break;
            }
        }

        free(h);
        return d;
    }
};

int main()
{
    thread p(producer);
    //_sleep(1);
    thread ca(consumer);
    thread cb(consumer);

    p.join();
    ca.join();
    cb.join();
    system("pause");
    return 0;
}

//手撕共享指针
template <typename T>
class mySmartPtr{
private:
    T* m_val;
    int* m_count;

public:
    mySmartPtr<T>(T* val)
    {
        m_val = val;

        try{
            m_count = new int(1);
        }catch(...)
        {
            delete m_val;
            m_val = nullptr;
            delete m_count;
            m_count = nullptr;
        }
        //m_count = new int(1);
    }

    mySmartPtr<T>(const mySmartPtr<T>& x)
    {
        m_val = x.m_val;
        m_count = x.m_count;
        ++(*m_count);
    }

    mySmartPtr<T>& operator=(const mySmartPtr<T>& rhs)
    {
        ++(*rhs.m_count);

        if(--(*m_count) == 0)
        {
            delete m_val;
            m_val = nullptr;
            delete m_count;
            m_count = nullptr;
        }

        m_val = rhs.m_val;
        m_count = rhs.m_count;

        return *this;
    }

    ~mySmartPtr()
    {
        if(--(*m_count) == 0)
        {
            delete m_val;
            m_val = nullptr;
            delete m_count;
            m_count = nullptr;
        }
    }

    //解引用操作
    T operator*()
    {
        return *m_val;
    }

    //获取参数成员操作
    T* operator->()
    {
        return m_val;
    }
};

int main()
{
    mySmartPtr<int> a(new int(1));
    mySmartPtr<int> b(new int(2));
    a = b;

    cout << *a << endl;
    system("pause");
    return 0;
}