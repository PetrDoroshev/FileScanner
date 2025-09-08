#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <future>
#include <functional>

class ThreadPool {

private:
    std::vector<std::thread> workers;
    std::mutex mutex;
    std::condition_variable cv;
    std::queue<std::function<void()>> queue;
    void worker();
    bool stop;

public:
    ThreadPool(std::size_t n_threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template<typename F, typename... Args>
    auto add_task(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

};


#endif