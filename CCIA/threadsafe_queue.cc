//
// Created by just do it on 2023/12/3.
//
#include "threadsafe_queue.h"
#include <thread>
#include <unistd.h>
#include <iostream>

int main() {
    threadsafe_queue<int> queue;

    std::thread t([&queue]() {
        sleep(1);
        queue.push(10);
    });

    int val;
    queue.wait_and_pop(val);
    std::cout << val << std::endl;

    t.join();
}