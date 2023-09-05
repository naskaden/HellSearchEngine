#ifndef HELLSEARCHENGINE_THREADPOOL_H
#define HELLSEARCHENGINE_THREADPOOL_H

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <future>

class ThreadPool {
public:
    explicit ThreadPool(int _num_threads) {
        num_threads = _num_threads;
        for (int i = 0; i < num_threads; i++) {
            threads.emplace_back(&ThreadPool::Run, this);
        }
    }

    template <typename Func, typename ...Args>
    void AddTask(const Func& task_func, Args&&... args) {
        std::lock_guard<std::mutex> lock(tasks_mtx);
        tasks.emplace(std::async(std::launch::deferred, task_func, args...));
        tasks_cv.notify_one();
    }

    void WaitAll() {
        terminate = true;
        tasks_cv.notify_all();
        for (std::thread &thread : threads) {
            thread.join();
        }
        stop = true;
        //std::cout<<"Threads joined"<<std::endl;
    }

    ~ThreadPool() {
        if (!stop) {
            WaitAll();
        }
    }

private:
    void Run() {
        std::future<void> task;
        while (true) {
            std::unique_lock<std::mutex> lock(tasks_mtx);
            tasks_cv.wait(lock, [this] { return !tasks.empty() || terminate; });

            if (terminate && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
            lock.unlock();
            try {
                task.get();
            } catch (std::runtime_error &ex) {
                std::cout << "Error at " << std::this_thread::get_id() << " " << ex.what() << std::endl;
            }
        }
    }

    int num_threads = 1;
    std::vector<std::thread> threads;
    std::queue<std::future<void>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable tasks_cv;
    std::atomic_bool terminate = false;
    std::atomic_bool stop = false;

};

#endif //HELLSEARCHENGINE_THREADPOOL_H
