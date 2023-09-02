#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

using namespace std;

//环形缓冲区的实现,生产者与消费者的模式
template <typename T>
class BoundBuffer
{
private:
    vector<T> m_buffer;
    mutex m_mutex;
    int capacity;
    int curSize;
    int start;
    int end;
    condition_variable not_full;
    condition_variable not_empty;

public:
    BoundBuffer(int c) : capacity(c),curSize(0),start(0),end(0) {
        m_buffer.resize(c);
    }

    void Produce(T x)
    {
        unique_lock<mutex> lock(m_mutex);

        not_full.wait(lock,[=]{return curSize < capacity;});
        m_buffer[end] = x;
        end = (end + 1) % capacity;
        ++curSize;
        cout << "Produce successfully: " << x  << " in thread: " << this_thread::get_id() << endl;
        lock.unlock();

        not_empty.notify_one();
    }

    T Consume()
    {
        unique_lock<mutex> lock(m_mutex);

        not_empty.wait(lock,[=]{return curSize > 0;});
        T v = m_buffer[start];
        start = (start + 1) % capacity;
        --curSize;
        cout << "Consume successfully: " << v << " in thread: " << this_thread::get_id() << endl;
        lock.unlock();

        not_full.notify_one();
        return v;
    }
};

BoundBuffer<int> m_buffer(10);

void Producer()
{
    for(int i = 0;i < 100;i++)
    {
        m_buffer.Produce(i);
    }

    m_buffer.Produce(-1);
}

void Consumer()
{
    thread::id thread_id = this_thread::get_id();

    int n = 0;
    do{
        n = m_buffer.Consume();
    }while(n != -1);

    m_buffer.Produce(-1);
}

int main()
{
    vector<thread> m_threads;

    m_threads.push_back(thread(Producer));
    m_threads.push_back(thread(Consumer));
    m_threads.push_back(thread(Consumer));
    m_threads.push_back(thread(Consumer));

    for(auto& it : m_threads)
        it.join();
    
    system("pause");
    return 0;
}



