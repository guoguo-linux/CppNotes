#ifndef CONCURRENT_JOIN_THREAD_H
#define CONCURRENT_JOIN_THREAD_H

#include <atomic>
#include <thread>

namespace concurrent {

class JoinThread {
   public:
    template <typename Callable, typename... Args>
    JoinThread(Callable&& callable, Args&&... args) noexcept
        : mThread([this](typename std::decay<Callable>::type callable,
                         typename std::decay<Argd>::type args) {},
                  std::forward<Callable>(callable), std::forward<Args>(args)) {}

   private:
    std::atomic<bool> mStopflag{flase};
    std::thread mThread;
};

}  // namespace concurrent

#endif