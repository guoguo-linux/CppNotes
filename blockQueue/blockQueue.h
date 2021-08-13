#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template <typename T>
class BlockQueue {
   public:
    T pop() {
        std::unique_lock<std::mutex> lock(mMutex);
        mCondition.wait(lock, [this]() { return !mQueue.empty(); });
        auto value = std::move(mQueue.front());
        mQueue.pop();
        return value;
    }

    void pop(T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        mCondition.wait(lock, [this]() { return !mQueue.empty(); });
        item = std::move(mQueue.front());
        mQueue.pop();
    }

    bool tryPop(T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mQueue.empty()) {
            return false;
        }
        item = std::move(mQueue.front());
        mQueue.pop();
        return true;
    }

    bool empty() const noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

    size_t size() const noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.size();
    }

    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.push(item);
        lock.unlock();  // unlock before notificiation to minimize mutex
                        // contention
        mCondition.notify_one();  // notify one waiting thread
    }

    void push(T&& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.push(std::move(item));
        lock.unlock();
        mCondition.notify_one();
    }

    BlockQueue() = default;
    BlockQueue(const BlockQueue&) = delete;             // disable copying
    BlockQueue& operator=(const BlockQueue&) = delete;  // disable assignment

   private:
    std::queue<T> mQueue;
    std::mutex mMutex;
    std::condition_variable mCondition;
};

#endif