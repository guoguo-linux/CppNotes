/*
 * @Author: zck
 * @Date: 2020-12-10 11:09:23
 * @LastEditTime: 2020-12-10 12:49:54
 * @Description: file content
 */
#ifndef MESSAGE_THREAD_H
#define MESSAGE_THREAD_H

#include <functional>
#include <thread>
#include <vector>

#include "block_queue.h"
#include "spsc_block_queue.h"

namespace concurrent {

template <typename T, template <typename M> typename Queue = SPSCBlockQueue>
class MessageThread {
   public:
    template <typename Callback>
    MessageThread(Callback &&onMessage) noexcept
        : mThread(
              [this](typename std::decay<Callback>::type onMessage) {
                  this->loop(std::forward<Callback>(onMessage));
              },
              std::forward<Callback>(onMessage)) {}

    template <typename Callback>
    MessageThread(Callback &&onMessage, size_t bufferSize) noexcept
        : mThread(
              [this](typename std::decay<Callback>::type onMessage) {
                  this->loop(std::forward<Callback>(onMessage));
              },
              std::forward<Callback>(onMessage)),
          mQueue(bufferSize) {}

    virtual ~MessageThread() noexcept {
        if (mThread.joinable()) {
            Wrap wrap;
            wrap.isStop = true;
            mQueue.push(std::move(wrap));
            mThread.join();
        }
    }

    MessageThread(const MessageThread &) = delete;
    MessageThread(MessageThread &&) = delete;
    MessageThread &operator=(const MessageThread &) = delete;
    MessageThread &operator=(MessageThread &&) = delete;

    void clearMessage() noexcept {
        Wrap tmp;
        for (size_t i = 0; i < mQueue.size(); ++i) {
            if (!mQueue.tryPop(tmp)) return;
        }
    }

    size_t messageSize() const noexcept { return mQueue.size(); }

    template <typename Message>
    void push(Message &&msg) noexcept {
        Wrap wrap;
        wrap.msg.push_back(std::forward<Message>(msg));
        mQueue.push(std::move(wrap));
    }

    template <typename Message, typename Rep, typename Period>
    bool push(Message &&msg,
              const std::chrono::duration<Rep, Period> &waitTime) noexcept {
        Wrap wrap;
        wrap.msg.push_back(std::forward<Message>(msg));
        return mQueue.push(std::move(wrap), waitTime);
    }

   private:
    struct Wrap {
        bool isStop = false;
        std::vector<T> msg;
    };

    Queue<Wrap> mQueue;

    std::thread mThread;

    template <typename Callback>
    void loop(Callback &&onMessage) noexcept {
        while (true) {
            Wrap wrap;
            mQueue.pop(wrap);
            if (wrap.isStop) break;
            onMessage(std::move(wrap.msg[0]));
        }
    }
};

}  // namespace concurrent

#endif