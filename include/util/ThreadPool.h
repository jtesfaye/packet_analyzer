//
// Created by jeremiah tesfaye on 8/18/25.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:

    explicit ThreadPool(size_t thread_count = std::thread::hardware_concurrency());
    ~ThreadPool() {

        shutdown();

    }

    template<class F, class... Args>
    void submit(F&& f, Args&&... args);

    void do_work();

    void shutdown();

private:

    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex lock;
    std::condition_variable m_work_to_do;
    std::atomic<bool> m_stop{false};

};

#include "ThreadPool.tpp"


#endif //THREADPOOL_H
