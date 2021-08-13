/*
 * @Author: zck
 * @Date: 2020-11-30 11:27:10
 * @LastEditTime: 2020-11-30 15:14:19
 * @Description: file content
 */

#ifndef CONCURRENT_SPIN_LOCK_H
#define CONCURRENT_SPIN_LOCK_H

#include <atomic>
#include <thread>

namespace concurrent {

class SpinLock {
   public:
    void lock() noexcept {
        while (!mLock.test_and_set(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    bool tryLock() noexcept {
        return mLock.test_and_set(std::memory_order_acquire);
    }

    bool try_lock() noexcept { return tryLock(); }

    void unlock() noexcept { mLock.clear(std::memory_order_release); }

   private:
    std::atomic_flag mLock = ATOMIC_FLAG_INIT;
};

}  // namespace concurrent

#endif
