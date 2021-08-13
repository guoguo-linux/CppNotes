/*
 * @Author: zck
 * @Date: 2020-11-23 12:26:06
 * @LastEditTime: 2020-12-02 17:27:51
 * @Description: file content
 */

#ifndef CONCURRENT_QUEUE_h
#define CONCURRENT_QUEUE_h

#include <queue>

#include "spin_lock.h"

namespace concurrent {

template <typename T>
class Queue {
   public:
    Queue() noexcept {}

    bool empty() const noexcept {
        mLock.lock();
        auto val = mQueue.empty();
        mLock.unlock();
        return val;
    }

    size_t size() const noexcept {
        mLock.lock();
        auto val = mQueue.size();
        mLock.unlock();
        return val;
    }

    template <typename Element, typename Condition>
    bool push(Element&& element, Condition&& func) noexcept {
        mLock.lock();
        if (!func()) {
            mLock.unlock();
            return false;
        }
        mQueue.push(std::forward<Element>(element));
        mLock.unlock();
        return true;
    }

    template <typename Element>
    void push(Element&& element) noexcept {
        this->push(std::forward<Element>(element), []() { return true; });
    }

    template <typename Element>
    bool push(Element&& element, size_t upperLimit) noexcept {
        return this->push(std::forward<Element>(element), [this, upperLimit]() {
            return mQueue.size() < upperLimit;
        });
    }

    template <typename... Args, typename Condition>
    bool emplace(Args&&... args, Condition&& func) noexcept {
        mLock.lock();
        if (!func()) {
            mLock.unlock();
            return false;
        }
        mQueue.emplace(std::forward<Args>(args)...);
        mLock.unlock();
        return true;
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        this->emplace(std::forward<Args>(args)..., []() { return true; });
    }

    template <typename... Args>
    bool emplace(Args&&... args, size_t upperLimit) noexcept {
        return this->emplace(std::forward<Args>(args)..., [this, upperLimit]() {
            return mQueue.size() < upperLimit;
        });
    }

    T pop() {
        mLock.lock();
        auto element = std::move(mQueue.front());
        mQueue.pop();
        mLock.unlock();
        return element;
    }

    template <typename Element, typename Condition>
    bool pop(Element& element, Condition&& func) noexcept {
        mLock.lock();
        if (mQueue.empty() || !func()) {
            mLock.unlock();
            return false;
        }
        element = std::move(mQueue.front());
        mQueue.pop();
        mLock.unlock();
        return true;
    }

    template <typename Element>
    bool pop(Element& element, size_t lowerLimit) noexcept {
        return this->pop(element, [this, lowerLimit]() {
            return mQueue.size() > lowerLimit;
        });
    }

    template <typename Element>
    bool pop(Element& element) noexcept {
        return this->pop(element, []() { return true; });
    }

    template <typename Element, typename Condition>
    bool tryPop(Element& element, Condition&& func) noexcept {
        if (!mLock.tryLock()) {
            return false;
        }
        mLock.lock();
        if (mQueue.empty() || !func()) {
            mLock.unlock();
            return false;
        }
        element = std::move(mQueue.front());
        mQueue.pop();
        mLock.unlock();
        return true;
    }

    template <typename Element>
    bool tryPop(Element& element, size_t lowerLimit) noexcept {
        return this->tryPop(element, [this, lowerLimit]() {
            return mQueue.size() > lowerLimit;
        });
    }

    bool remove() noexcept {
        if (!mLock.tryLock()) {
            return false;
        }
        if (mQueue.empty()) {
            mLock.unlock();
            return false;
        }
        mQueue.pop();
        mLock.unlock();
        return true;
    }

    void clear() noexcept {
        mLock.lock();
        std::queue<T> emptyQueue;
        mQueue.swap(emptyQueue);
        mLock.unlock();
    }

   private:
    mutable SpinLock mLock;
    std::queue<T> mQueue;
};

}  // namespace concurrent

#endif
