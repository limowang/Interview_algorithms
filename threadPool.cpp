#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>
//#include <unordered_map>
#include <atomic>
#include <functional>
#include <chrono>
#include <future>
#include <unistd.h>

using namespace std;

//线程池的实现,支持仿函数
#define THREADPOOL_MAX_NUM 16

class threadPool
{
private:
    using Task = function<void()>;
    unsigned short _initSize;
    vector<thread> m_pool;
    queue<Task> m_tasks;
    mutex m_q_mutex;
    condition_variable m_q_cv;
    atomic<bool> m_run{true};
    atomic<int> m_idThNum{0}; // 空闲的线程数量

public:
    threadPool(unsigned short size = 4)
    {
        _initSize = size;
        addThread(size);
    }

    ~threadPool()
    {
        m_run = false;
        // 唤醒所有线程执行剩余的任务
        m_q_cv.notify_all();

        for (auto &thread : m_pool)
        {
            if (thread.joinable())
                thread.join();
        }
    }

    // 空闲线程的数量
    int idlCount() { return m_idThNum; }

    // 线程池中线程的数量
    int thCount() { return m_pool.size(); }

    // 向线程池中注册任务
    template <typename F, typename... Args>
    auto addTask(F &&f, Args &&...args) -> future<decltype(f(args...))>          //future从异步任务中获取结果
    {
        if (!m_run)
            throw runtime_error("commit on ThreadPool is Stopped.");

        using RetType = decltype(f(args...));
        auto task = make_shared<packaged_task<RetType()>>(bind(forward<F>(f), forward<Args>(args)...));

        future<RetType> fu = task->get_future();

        {
            // 将任务放到队列
            lock_guard<mutex> lock{m_q_mutex};
            m_tasks.emplace([task]()
                            { (*task)(); });
        }

        // 唤醒一个线程
        m_q_cv.notify_one();
        return fu;
    }

    // 添加循环工作线程
    void addThread(unsigned short size)
    {
        for (; m_pool.size() < THREADPOOL_MAX_NUM && size > 0; --size)
        {
            // 每个工作线程的任务，一直循环
            auto it = [this]
            {
                while (true)
                {
                    Task task;
                    {
                        unique_lock<mutex> lock{m_q_mutex};
                        m_q_cv.wait(lock, [this]
                                    { return !m_run || !m_tasks.empty(); });
                        if (!m_run && m_tasks.empty())
                            return;

                        m_idThNum--;
                        task = move(m_tasks.front());
                        m_tasks.pop();
                    }

                    // 执行任务
                    task();
                    {
                        unique_lock<mutex> lock{m_q_mutex};
                        m_idThNum++;
                    }
                }
            };

            //添加线程到线程池
            m_pool.emplace_back(it);
        }
    }
};

int addTwo(int a,int b)
{
    return a + b;
}

// void fun1(int slp)
// {
//     printf("  hello, fun1 !  %d\n", this_thread::get_id());
//     if (slp > 0)
//     {
//         printf(" ======= fun1 sleep %d  =========  %d\n", slp, this_thread::get_id());
//         this_thread::sleep_for(chrono::milliseconds(slp));
//         // Sleep(slp );
//     }
// }

// struct gfun
// {
//     int operator()(int n)
//     {
//         printf("%d  hello, gfun !  %d\n", n, this_thread::get_id());
//         return 42;
//     }
// };

// void print()
// {
//     cout << "hello thread: " << this_thread::get_id() << endl;
// }

//测试函数
int main()
{

    threadPool mthreads(4);

    future<int> f = mthreads.addTask(addTwo,10,20);

    //get函数如果任务还没有执行完，则会阻塞直到任务结束，主线程此时不能做其他事情
    cout << f.get() << endl;

    // threadPool executor{10};

    // future<void> ff = executor.addTask(fun1, 0);
    // future<int> fg = executor.addTask(gfun(), 0);

    // for (int i = 0; i < 50; i++)
    // {
    //     executor.addTask(fun1, i * 100);
    // }

    // cout << " =======  commit all ========= " << this_thread::get_id() << " idlsize=" << executor.idlCount() << endl;

    // cout << " =======  sleep ========= " << this_thread::get_id() << endl;
    // this_thread::sleep_for(chrono::seconds(3));

    // ff.get(); // 调用.get()获取返回值会等待线程执行完,获取返回值
    // std::cout << fg.get() << "  " << this_thread::get_id() << endl;

    // thread th(print);

    // th.join();

    // cout << "main thread: " << this_thread::get_id() << endl;

    system("pause");
    return 0;
}