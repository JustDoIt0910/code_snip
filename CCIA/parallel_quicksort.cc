//
// Created by just do it on 2023/12/3.
//
#include "parallel_quicksort.h"
#include <iostream>
#include <random>

void print(const std::list<int>& l) {
    for(int i : l) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::list<int> l1, l2;
    std::uniform_int_distribution<int> u(0, 100000);
    std::default_random_engine e(time(nullptr));
    for(int i = 0; i < 50000; i++) {
        int val = u(e);
        l1.push_back(val);
        l2.push_back(val);
    }

    auto start = std::chrono::steady_clock::now();
    auto l11 = sequential_quicksort(l1);
    auto end = std::chrono::steady_clock::now();
    double t = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "time consume (sequential) = " << t << " ms" << std::endl;

    start = std::chrono::steady_clock::now();
    auto l22 = parallel_quicksort(l2);
    end = std::chrono::steady_clock::now();
    t = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "time consume (parallel) = " << t << " ms" << std::endl;

//    print(l);
//    print(l1);
//    print(l2);
}