//
// Created by just do it on 2023/11/30.
//

#ifndef CODE_SNIP_PARALLEL_ACCUMULATE_H
#define CODE_SNIP_PARALLEL_ACCUMULATE_H
#include <numeric>
#include <algorithm>
#include <thread>
#include <vector>
#include <iostream>

template<typename Iter, typename T>
struct accumulate_block {
    void operator()(Iter first, Iter last, T& result) {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iter, typename T>
T parallel_accumulate(Iter first, Iter last, T init) {
    const unsigned long length = std::distance(first, last);
    if(!length) {
        return init;
    }
    const unsigned long min_per_thread = 25;
    const unsigned long max_threads = (length + min_per_thread - 1) / min_per_thread;
    const unsigned long hardware_threads = std::thread::hardware_concurrency();
    const unsigned long num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2,
                                               max_threads);
    std::cout << "using " << num_threads << " threads" << std::endl;
    const unsigned long block_size = length / num_threads;
    std::vector<std::thread> threads(num_threads - 1);
    std::vector<T> results(num_threads);
    Iter block_start = first;
    for(int i = 0; i < num_threads - 1; i++) {
        Iter block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iter, T>(),
                block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }
    accumulate_block<Iter, T>()(block_start, last, results[num_threads - 1]);
    for(auto& entry: threads) {
        entry.join();
    }
    return std::accumulate(results.begin(), results.end(), init);
}

#endif //CODE_SNIP_PARALLEL_ACCUMULATE_H
