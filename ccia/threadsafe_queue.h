//
// Created by just do it on 2023/12/3.
//

#ifndef CODE_SNIP_THREADSAFE_QUEUE_H
#define CODE_SNIP_THREADSAFE_QUEUE_H

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue {
private:
    std::queue<T> queue_;
    mutable std::mutex mu_;
    std::condition_variable cond_;

public:
    threadsafe_queue() = default;

    threadsafe_queue(const threadsafe_queue& other) {
        std::lock_guard<std::mutex> lg(other.mu_);
        queue_ = other.queue_;
    }

    void push(T new_value) {
        std::lock_guard<std::mutex> lg(mu_);
        queue_.push(new_value);
        cond_.notify_one();
    }

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mu_);
        cond_.wait(lk, [this](){ return !queue_.empty(); });
        value = queue_.front();
        queue_.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mu_);
        cond_.wait(lk, [this](){ return !queue_.empty(); });
        auto res = std::make_shared<T>(queue_.front());
        queue_.pop();
        return res;
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lg(mu_);
        if(queue_.empty()) {
            return false;
        }
        value = queue_.front();
        queue_.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lg(mu_);
        if(queue_.empty()) {
            return std::shared_ptr<T>();
        }
        auto res = std::make_shared<T>(queue_.front());
        queue_.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lg(mu_);
        return queue_.empty();
    }
};

#endif //CODE_SNIP_THREADSAFE_QUEUE_H
