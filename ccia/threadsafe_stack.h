//
// Created by just do it on 2023/12/1.
//

#ifndef CODE_SNIP_THREADSAFE_STACK_H
#define CODE_SNIP_THREADSAFE_STACK_H

#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack: std::exception {
    const char* what() const throw() {
        return "empty_stack";
    }
};

template<typename T>
class threadsafe_stack {
private:
    std::stack<T> data_;
    mutable std::mutex mu_;

public:
    threadsafe_stack() = default;

    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.mu_);
        data_ = other.data_;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value) {
        std::lock_guard<std::mutex> lock(mu_);
        data_.push(std::move(new_value));
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(mu_);
        if(data_.empty()) {
            throw empty_stack();
        }
        std::shared_ptr<T> const res(std::make_shared<T>(data_.top));
        data_.pop();
        return res;
    }

    void pop(T& value) {
        std::lock_guard<std::mutex> lock(mu_);
        if(data_.empty()) {
            throw empty_stack();
        }
        value = data_.top();
        data_.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mu_);
        return data_.empty();
    }
};

#endif //CODE_SNIP_THREADSAFE_STACK_H
