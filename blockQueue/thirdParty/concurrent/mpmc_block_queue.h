#ifndef CONCURRENT_MPMC_BLOCK_QUEUE_H
#define CONCURRENT_MPMC_BLOCK_QUEUE_H

#include "third_party/concurrentqueue/blockingconcurrentqueue.h"

namespace concurrent {

template <typename T>
class MPMCBlockQueue {
   public:
    MPMCBlockQueue() = default;

    MPMCBlockQueue(size_t size) : mQueue(size) {}

    template <typename Message>
    void push(Message&& msg) {
        mQueue.enqueue(std::forward<Message>(msg));
    }

    template <typename Message>
    bool tryPush(Message&& msg) {
        return mQueue.try_enqueue(std::forward<Message>(msg));
    }

    void pop(T& msg) { mQueue.wait_dequeue(msg); }

    template <typename Rep, typename Period>
    bool tryPop(T& msg, const std::chrono::duration<Rep, Period>& waitTime) {
        return mQueue.wait_dequeue_timed(msg, waitTime);
    }

    bool tryPop(T& msg) { return mQueue.try_dequeue(msg); }

    size_t size() const { return mQueue.size_approx(); }

    bool empty() const { return mQueue.size_approx() == 0; }

   private:
    moodycamel::BlockingConcurrentQueue<T> mQueue;
};

}  // namespace concurrent

#endif