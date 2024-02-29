#include "threads.hpp"
// void foo();
// void bar();

thread::thread_pool::thread_pool(int n) {
    std::thread *temp;
    for (int i = 0; i < n; ++i) {
        pool.push_back(new std::thread([this] { this->work(); }));
        pool.back()->detach();
    }
}

thread::thread_pool::~thread_pool() {
    keep_alive = false;
    while (!pool.empty()) {
        std::thread *temp = pool.back();
        pool.pop_back();
        free(temp);
    }
}

void thread::thread_pool::work() {
    std::function<void()> job;
    while (keep_alive) {
        {
        // std::cout << "thread waiting...\n";
        std::unique_lock<std::mutex> lk(mtx);
        std::queue<std::function<void()>> *temp = &pending;
        cond.wait(lk, [&temp] { return !(temp->empty()); });

        // std::cout << "thread running\n";
        job = temp->front();
        temp->pop();
        }
        job();
        // foo();
    }
}

void thread::thread_pool::enqueue(std::function<void()> f, int flags) {
    if (flags & 0b1) {
        std::thread *temp = new std::thread([this] { this->work(); });
        temp->detach();

        pool.push_back(temp);
    }

    std::lock_guard<std::mutex> lk(mtx);

    // mtx.lock();
    pending.push(f);
    // mtx.unlock();

    cond.notify_one();
}

// void foo() {
//     Sleep(2000);
//     printf("foo");
// }

// void bar() {
//     Sleep(2000);
//     printf("bar");
// }

// int main() {
//     thread::thread_pool tp(2);

//     Sleep(5000);
//     tp.enqueue(foo);
//     std::cout << "AAA\n";
//     tp.enqueue(bar);
//     // Sleep(8000);
//     // std::vector<std::thread *> f;
//     // for (int i = 0; i < 2; ++i) {
//     //     auto t = new std::thread(foo);
//     //     t->detach();
//     //     f.push_back(t);
//     // }

//     Sleep(9000);
// }