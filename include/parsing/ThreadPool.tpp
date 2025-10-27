//
// Created by jeremiah tesfaye on 8/18/25.
//

#pragma once

inline ThreadPool::ThreadPool(
    const PoolInit &init,
    size_t thread_count)
: m_initial_parser(init.init_parser)
, m_detail_parser(init.detail_parser)
, m_initial_buffer(init.buffer)
{
    for (size_t i = 0; i < thread_count; ++i) {
        m_workers.emplace_back([this] {do_work(); });
    }
}



template<class F, class... Args>
void ThreadPool::submit(F&& f, Args&&... args) {

    std::lock_guard guard(lock);

    m_tasks.emplace([=]() mutable {
        f(std::forward<Args>(args)...);
    });

     m_work_to_do.notify_all();
}

inline void ThreadPool::add(const size_t index, const ParseJob job_type) {



}

inline void ThreadPool::do_work() {

    while (true) {

        std::function<void()> task;
        {
            std::unique_lock u_lock(lock);
            m_work_to_do.wait(u_lock, [this] {return m_stop || !m_tasks.empty(); });

            if (m_stop && m_tasks.empty()) return;

            task = std::move(m_tasks.front());
            m_tasks.pop();

        }

        task();
    }
}

inline void ThreadPool::shutdown() {

    {
        std::unique_lock u_lock(lock);
        m_stop = true;
    }

    m_work_to_do.notify_all();

    for (auto &t : m_workers) {

        if (t.joinable())
            t.join();

    }
}
