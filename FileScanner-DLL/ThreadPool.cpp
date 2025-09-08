#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t n_threads) {

    stop = false;
    
    for (std::size_t i = 0; i < n_threads; i++) {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }

    cv.notify_all();
    for (auto& worker : workers) {
        worker.join();
    }
}

void ThreadPool::worker() {

    while (true) {

        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex);

            cv.wait(lock, [this]() {
                return stop || !queue.empty();
            });

            if (stop && queue.empty()) 
                break;
            if (queue.empty())
                continue; 

            task = queue.front();
            queue.pop();
        }

        task();
    }
}

template<typename F, typename... Args>

inline auto ThreadPool::add_task(F&& f, Args&&... args)
-> std::future<decltype(f(args...))> {

    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    
    auto encapsulated_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

    std::future<std::result_of_t<F(Args...)>> future_object = encapsulated_ptr->get_future();
    {
        std::unique_lock<std::mutex> lock(mutex);

        queue.emplace([encapsulated_ptr]() {
            (*encapsulated_ptr)(); 
        });
    }

    cv.notify_one();
    return future_object;
}