/*
 * @Author: zck
 * @Date: 2020-11-23 10:25:08
 * @LastEditTime: 2020-11-30 14:40:32
 * @Description: file content
 */

#ifndef CONCURRENT_THREAD_POOL_H
#define CONCURRENT_THREAD_POOL_H

#include <functional>
#include <future>
#include <thread>
#include <type_traits>
#include <vector>

#include "mpmc_block_queue.h"

namespace concurrent {

class ThreadPool {
   public:
    ThreadPool(size_t threadNum = std::thread::hardware_concurrency()) noexcept
        : mThreads(threadNum) {
        for (auto& thread : mThreads) {
            thread = std::thread([this]() { this->loop(); });
        }
    }

    ~ThreadPool() {
        for (auto& thread : mThreads) {
            Wrap wrap;
            wrap.isStop = true;
            mQueue.push(std::move(wrap));
        }
        for (auto& thread : mThreads) {
            thread.join();
        }
    }

    template <typename F, typename... Args>
    std::future<typename std::result_of<F(Args...)>::type> enqueue(
        F&& func, Args&&... args) {
        using ReturnType = typename std::result_of<F(Args...)>::type;
        auto taskPackage = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...));

        Wrap wrap;
        wrap.func = [taskPackage]() { taskPackage->operator()(); };
        mQueue.push(std::move(wrap));

        return taskPackage->get_future();
    }

   private:
    struct Wrap {
        bool isStop = false;
        std::function<void()> func;
    };

    MPMCBlockQueue<Wrap> mQueue;
    std::vector<std::thread> mThreads;

    void loop() {
        while (true) {
            Wrap wrap;
            mQueue.pop(wrap);
            if (wrap.isStop) break;
            wrap.func();
        }
    }
};

}  // namespace concurrent

#endif
