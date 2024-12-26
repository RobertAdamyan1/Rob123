#include <iostream>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

class parallel_scheduler {
public:
    explicit parallel_scheduler(int capacity);
    ~parallel_scheduler();

    template <typename Func, typename Arg>
    void run(Func&& func, Arg&& arg);

private:
    void worker();

    int capacity;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> task_queue;
    std::mutex queue_mutex;
    std::condition_variable cv;
    bool stop_flag;
};

parallel_scheduler::parallel_scheduler(int capacity)
    : capacity(capacity), stop_flag(false) {
    for (int i = 0; i < capacity; ++i) {
        threads.emplace_back(&parallel_scheduler::worker, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop_flag = true;
    }
    cv.notify_all();

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

template <typename Func, typename Arg>
void parallel_scheduler::run(Func&& func, Arg&& arg) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.push([=] { func(arg); });
    }
    cv.notify_one();
}

void parallel_scheduler::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [this] { return stop_flag || !task_queue.empty(); });

            if (stop_flag && task_queue.empty()) {
                return;
            }

            task = std::move(task_queue.front());
            task_queue.pop();
        }
        task();
    }
}

void print_message(int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Task " << id << " completed\n";
}

int main() {
    parallel_scheduler pool(3);

    for (int i = 0; i < 10; ++i) {
        pool.run(print_message, i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(6));

    return 0;
}
