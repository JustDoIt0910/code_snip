//
// Created by just do it on 2023/11/30.
//
#include "parallel_accumulate.h"
#include <iostream>
#include <chrono>

int main() {
    std::vector<long long> v;
    for(int i = 0; i < 10000000; i++) {
        v.push_back(i);
    }

    long long init = 0;
    auto start = std::chrono::steady_clock::now();
    std::cout << std::accumulate(v.begin(), v.end(), init) << std::endl;
    auto end = std::chrono::steady_clock::now();
    double t = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "time consume (single thread) = " << t << " ms" << std::endl;

    start = std::chrono::steady_clock::now();
    std::cout << parallel_accumulate(v.begin(), v.end(), init) << std::endl;
    end = std::chrono::steady_clock::now();
    t = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "time consume (parallel) = " << t << " ms" << std::endl;
}