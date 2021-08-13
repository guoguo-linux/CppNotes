#ifndef CONCURRENT_SPSC_BLOCK_QUEUE_H
#define CONCURRENT_SPSC_BLOCK_QUEUE_H

#include "third_party/concurrentqueue/readerwritercircularbuffer.h"

namespace concurrent {

template <typename T>
class SPSCBlockQueue {
   public:
    SPSCBlockQueue(size_t size) : mQueue(size) {}

    template <typename Message>
    void push(Message&& msg) {
        mQueue.wait_enqueue(std::forward<Message>(msg));
    }

    template <typename Message, typename Rep, typename Period>
    bool tryPush(Message&& msg,
                 const std::chrono::duration<Rep, Period>& waitTime) {
        return mQueue.wait_enqueue_timed(std::forward<Message>(msg), waitTime);
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
    moodycamel::BlockingReaderWriterCircularBuffer<T> mQueue;
};

}  // namespace concurrent

#endif