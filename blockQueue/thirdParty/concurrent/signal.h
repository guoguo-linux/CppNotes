/*
 * @Author: zck
 * @Date: 2020-11-30 12:19:25
 * @LastEditTime: 2020-12-02 17:36:56
 * @Description: file content
 */

#ifndef CONCURRENT_SIGNAL_H
#define CONCURRENT_SIGNAL_H

#include <condition_variable>
#include <mutex>

namespace concurrent {

class Signal {
   public:
    Signal() noexcept {}

    void notifyOne() noexcept { mCond.notify_one(); }

    void notifyAll() noexcept { mCond.notify_all(); }

    void wait() noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        mCond.wait(lock);
    }

    template <typename Predicate>
    void wait(Predicate&& predicate) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        mCond.wait(lock, std::forward<Predicate>(predicate));
    }

    template <typename Rep, typename Period>
    bool waitFor(const std::chrono::duration<Rep, Period>& waitTime) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mCond.wait_for(lock, waitTime);
    }

    template <typename Rep, typename Period, typename Predicate>
    bool waitFor(const std::chrono::duration<Rep, Period>& waitTime,
                 Predicate&& predicate) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mCond.wait_for(lock, waitTime,
                              std::forward<Predicate>(predicate));
    }

    template <typename Rep, typename Period>
    std::cv_status waitUtil(
        const std::chrono::duration<Rep, Period>& waitTime) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mCond.wait_until(lock, waitTime);
    }

    template <typename Rep, typename Period, typename Predicate>
    std::cv_status waitUtil(const std::chrono::duration<Rep, Period>& waitTime,
                            Predicate&& predicate) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mCond.wait_until(lock, waitTime,
                                std::forward<Predicate>(predicate));
    }

   private:
    std::mutex mMutex;
    std::condition_variable mCond;
};

}  // namespace concurrent

#endif
