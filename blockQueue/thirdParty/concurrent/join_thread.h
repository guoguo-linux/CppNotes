/*
 * @Author: zck
 * @Date: 2020-11-30 11:39:22
 * @LastEditTime: 2020-11-30 11:43:50
 * @Description: file content
 */

#ifndef CONCURRENT_JOIN_THREAD_H
#define CONCURRENT_JOIN_THREAD_H

#include <atomic>
#include <thread>

namespace concurrent {

class JoinThread {
   public:
    class StopToken {
        friend class JoinThread;

       public:
        bool isStopRequested() const noexcept {
            return mStopFlag.load(std::memory_order_acquire);
        }

       private:
        StopToken(const std::atomic<bool> &stopFlag) noexcept
            : mStopFlag(stopFlag) {}

        const std::atomic<bool> &mStopFlag;
    };

    template <class Callable, class... Args>
    JoinThread(Callable &&callable, Args &&...args) noexcept
        : mThread(
              [this](typename std::decay<Callable>::type callable,
                     typename std::decay<Args>::type... args) {
                  StopToken stopToken(this->mStopFlag);
                  callable(stopToken, args...);
              },
              std::forward<Callable>(callable), std::forward<Args>(args)...) {}

    virtual ~JoinThread() noexcept {
        if (mThread.joinable()) {
            this->requestStop();
            mThread.join();
        }
    }

    JoinThread(const JoinThread &) = delete;
    JoinThread(JoinThread &&) = delete;
    JoinThread &operator=(const JoinThread &) = delete;
    JoinThread &operator=(JoinThread &&) = delete;

    void requestStop() noexcept {
        mStopFlag.store(true, std::memory_order_release);
    }

    bool joinable() noexcept { return mThread.joinable(); }

    void join() noexcept { mThread.join(); }

   private:
    std::atomic<bool> mStopFlag{false};
    std::thread mThread;
};

}  // namespace concurrent

#endif
