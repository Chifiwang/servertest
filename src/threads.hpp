#include "utils.hpp"
#include <functional>


namespace thread {
struct thread_pool {
    bool keep_alive{true};
    std::vector<std::thread *> pool{};
    std::queue<std::function<void()>> pending;

    std::mutex mtx;
    std::condition_variable cond;
    // friend class std::thread;

    void work();
    thread_pool(int n);
    ~thread_pool();
    void enqueue(std::function<void()> f, int flags=0);
};
};  