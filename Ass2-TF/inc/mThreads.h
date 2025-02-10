#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
template <typename T>
class ThreadSafeQueue {
public:
    // 向队列中添加元素
    void Push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(value);
        conditionVariable.notify_one();
    }

    // 从队列中取出元素
    T Pop() {
        std::unique_lock<std::mutex> lock(mutex);
        conditionVariable.wait(lock, [this] { return !queue.empty(); });

        T frontValue = queue.front();
        queue.pop();
        return frontValue;
    }

private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable conditionVariable;
};

//for mthreads
void processPixel(int& pixel);
void matfprint(int row, int col, std::vector<int> data);
void processPixels(ThreadSafeQueue<int>& queue, std::vector<int>& image);