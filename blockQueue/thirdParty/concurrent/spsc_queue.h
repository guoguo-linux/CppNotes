#ifndef CONCURRENT_SPSC_QUEUE_H
#define CONCURRENT_SPSC_QUEUE_H

#include "third_party/concurrentqueue/readerwriterqueue.h"

namespace concurrent {

template <typename T>
class SPSCQueue {
   public:
    SPSCQueue() = default;

    SPSCQueue(size_t size) : mQueue(size) {}

    template <typename Message>
    void push(Message&& msg) {
        mQueue.enqueue(std::forward<Message>(msg));
    }

    template <typename Message>
    bool tryPush(Message&& msg) {
        return mQueue.try_enqueue(std::forward<Message>(msg));
    }

    bool tryPop(T& msg) { return mQueue.try_dequeue(msg); }

    size_t size() const { return mQueue.size_approx(); }

    bool empty() const { return mQueue.size_approx() == 0; }

   private:
    moodycamel::ReaderWriterQueue<T> mQueue;
};

}  // namespace concurrent

#endif