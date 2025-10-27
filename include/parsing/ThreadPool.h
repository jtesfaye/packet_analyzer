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
#include <parsing/InitialParser.h>
#include <parsing/DetailParser.h>
#include <capture/CaptureConfig.h>


enum class ParseJob {
    InitialParse, DetailParse
};

class ThreadPool {
public:

    explicit ThreadPool(
        const PoolInit &init,
        size_t thread_count = std::thread::hardware_concurrency()
        );

    ~ThreadPool() = default;

    void add(size_t index, ParseJob job_type);

    void do_work();

    void shutdown();

private:

    template<class F, class... Args>
    void submit(F&& f, Args&&... args);

    std::shared_ptr<InitialParser> m_initial_parser;
    std::shared_ptr<DetailParser> m_detail_parser;
    std::shared_ptr<IContainerType<packet_ref>> m_initial_buffer;

    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex lock;
    std::condition_variable m_work_to_do;
    std::atomic<bool> m_stop{false};

};

#include "ThreadPool.tpp"


#endif //THREADPOOL_H
