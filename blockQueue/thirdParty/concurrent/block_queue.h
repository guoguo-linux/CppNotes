/*
 * @Author: zck
 * @Date: 2020-11-30 11:56:17
 * @LastEditTime: 2020-12-01 17:51:34
 * @Description: file content
 */

#ifndef CONCURRENT_BLOCK_QUEUE_H
#define CONCURRENT_BLOCK_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

namespace concurrent {

template <typename T>
class BlockQueue {
   public:
    BlockQueue() noexcept {}

    bool empty() const noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

    size_t size() const noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.size();
    }

    template <typename Message, typename Condition>
    void push(Message&& msg, Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        mPopCond.wait(lock, std::forward<Condition>(func));
        mQueue.push(std::forward<Message>(msg));
        lock.unlock();
        mPushCond.notify_all();
    }

    template <typename Message>
    void push(Message&& msg) noexcept {
        this->push(std::forward<Message>(msg), []() { return true; });
    }

    template <typename Message>
    void push(Message&& msg, size_t upperLimit) noexcept {
        this->push(std::forward<Message>(msg),
                   [this, upperLimit]() { return mQueue.size() < upperLimit; });
    }

    template <typename Message, typename Rep, typename Period,
              typename Condition>
    bool push(Message&& msg, const std::chrono::duration<Rep, Period>& waitTime,
              Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        auto success =
            mPopCond.wait_for(lock, waitTime, std::forward<Condition>(func));
        if (success) {
            mQueue.push(std::forward<Message>(msg));
            lock.unlock();
            mPushCond.notify_all();
        }
        return success;
    }

    template <typename Message, typename Rep, typename Period>
    bool push(Message&& msg, const std::chrono::duration<Rep, Period>& waitTime,
              size_t upperLimit) noexcept {
        auto start = std::chrono::steady_clock::now();
        for (auto delta = start - start; delta <= waitTime;
             delta = std::chrono::steady_clock::now() - start) {
            auto success = this->push(
                std::forward<Message>(msg), waitTime - delta,
                [this, upperLimit]() { return mQueue.size() < upperLimit; });
            if (success) return true;
        }
        return false;
    }

    template <typename Message, typename Condition>
    bool tryPush(Message&& msg, Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        if (!func()) return false;
        mQueue.push(std::forward<Message>(msg));
        lock.unlock();
        mPushCond.notify_all();
        return true;
    }

    template <typename Message>
    bool tryPush(Message&& msg, size_t upperLimit) noexcept {
        return this->tryPush(std::forward<Message>(msg), [this, upperLimit]() {
            return mQueue.size() < upperLimit;
        });
    }

    template <typename... Args, typename Condition>
    void emplace(Args&&... args, Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        mPopCond.wait(lock, std::forward<Condition>(func));
        mQueue.emplace(std::forward<Args>(args)...);
        lock.unlock();
        mPushCond.notify_all();
    }

    template <typename... Args>
    void emplace(Args&&... args) noexcept {
        this->emplace(std::forward<Args>(args)..., []() { return true; });
    }

    template <typename... Args>
    void emplace(Args&&... args, size_t upperLimit) noexcept {
        this->emplace(
            [this, upperLimit]() { return mQueue.size() < upperLimit; });
    }

    template <typename... Args, typename Rep, typename Period,
              typename Condition>
    bool emplace(Args&&... args,
                 const std::chrono::duration<Rep, Period>& waitTime,
                 Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        auto success =
            mPopCond.wait_for(lock, waitTime, std::forward<Condition>(func));
        if (success) {
            mQueue.emplace(std::forward<Args>(args)...);
            lock.unlock();
            mPushCond.notify_all();
        }

        return success;
    }

    template <typename... Args, typename Rep, typename Period>
    bool emplace(Args&&... args,
                 const std::chrono::duration<Rep, Period>& waitTime,
                 size_t upperLimit) noexcept {
        auto start = std::chrono::steady_clock::now();
        for (auto delta = start - start; delta <= waitTime;
             delta = std::chrono::steady_clock::now() - start) {
            auto success = this->emplace(
                std::forward<Args>(args)..., waitTime - delta,
                [this, upperLimit]() { return mQueue.size() < upperLimit; });
            if (success) return true;
        }
        return false;
    }

    template <typename... Args, typename Condition>
    bool tryEmplace(Args&&... args, Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        if (!func()) return false;
        mQueue.push(std::forward<Args>(args)...);
        lock.unlock();
        mPushCond.notify_all();
        return true;
    }

    template <typename... Args>
    bool tryEmplace(Args&&... args, size_t limit) noexcept {
        return this->tryEmplace(std::forward<Args>(args)..., [this, limit]() {
            return mQueue.size() < limit;
        });
    }

    template <typename Condition>
    T pop(Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        mPushCond.wait(lock, std::forward<Condition>(func));
        auto msg = std::move(mQueue.front());
        mQueue.pop();
        lock.unlock();
        mPopCond.notify_all();
        return msg;
    }

    T pop() noexcept {
        return this->pop([this]() { return !mQueue.empty(); });
    }

    void pop(T& msg) noexcept {
        msg = this->pop([this]() { return !mQueue.empty(); });
    }

    T pop(size_t lowerLimit) noexcept {
        return this->pop(
            [this, lowerLimit]() { return mQueue.size() > lowerLimit; });
    }

    template <typename Message, typename Rep, typename Period,
              typename Condition>
    bool pop(Message& msg, const std::chrono::duration<Rep, Period>& waitTime,
             Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        auto success =
            mPushCond.wait_for(lock, waitTime, std::forward<Condition>(func));
        if (success && !mQueue.empty()) {
            msg = std::move(mQueue.front());
            mQueue.pop();
            lock.unlock();
            mPopCond.notify_all();
        }
        return success;
    }

    template <typename Message, typename Rep, typename Period>
    bool pop(Message& msg,
             const std::chrono::duration<Rep, Period>& waitTime) noexcept {
        return this->pop(msg, waitTime, []() { return true; });
    }

    template <typename Message, typename Rep, typename Period>
    bool pop(Message& msg, const std::chrono::duration<Rep, Period>& waitTime,
             size_t lowerLimit) noexcept {
        auto start = std::chrono::steady_clock::now();

        for (auto delta = start - start; delta <= waitTime;
             delta = std::chrono::steady_clock::now() - start) {
            auto success = this->pop(
                msg, waitTime - delta,
                [this, lowerLimit]() { return mQueue.size() > lowerLimit; });
            if (success) return true;
        }

        return false;
    }

    template <typename Message, typename Condition>
    bool tryPop(Message& msg, Condition&& func) noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mQueue.empty() || !func()) {
            return false;
        }
        msg = std::move(mQueue.front());
        mQueue.pop();
        lock.unlock();
        mPopCond.notify_all();
        return true;
    }

    template <typename Message>
    bool tryPop(Message& msg) noexcept {
        return this->tryPop(msg, []() { return true; });
    }

    template <typename Message>
    bool tryPop(Message& msg, size_t lowerLimit) noexcept {
        return this->tryPop(
            msg, [this, lowerLimit]() { return mQueue.size() > lowerLimit; });
    }

    bool remove() noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mQueue.empty()) {
            return false;
        }
        mQueue.pop();
        lock.unlock();
        mPopCond.notify_all();
        return true;
    }

    void clear() noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        if (!mQueue.empty()) {
            std::queue<T> emptyQueue;
            mQueue.swap(emptyQueue);
            lock.unlock();
            mPopCond.notify_all();
        }
    }

   private:
    mutable std::mutex mMutex;
    std::condition_variable mPushCond;
    std::condition_variable mPopCond;
    std::queue<T> mQueue;
};

}  // namespace concurrent

#endif
