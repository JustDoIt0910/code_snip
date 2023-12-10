//
// Created by just do it on 2023/12/3.
//

#ifndef CODE_SNIP_PARALLEL_QUICKSORT_H
#define CODE_SNIP_PARALLEL_QUICKSORT_H
#include <list>
#include <algorithm>
#include <future>

/** sequential quick sort using functional programming style **/

template<typename T>
std::list<T> sequential_quicksort(std::list<T> input) {
    if(input.empty()) {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    const T& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(),
                                       [&pivot](const T& t) {
        return t < pivot;
    });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    auto new_lower(sequential_quicksort(std::move(lower_part)));
    auto new_higher(sequential_quicksort(std::move(input)));
    result.splice(result.begin(), new_lower);
    result.splice(result.end(), new_higher);
    return result;
}

/** parallel quick sort using std::async and std::future (but slower) **/

template<typename T>
std::list<T> parallel_quicksort(std::list<T> input) {
    if(input.empty()) {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    const T& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(),
                                       [&pivot](const T& t) {
                                           return t < pivot;
                                       });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    std::future<std::list<T>> new_lower_future(std::async(
            &parallel_quicksort<T>, std::move(lower_part)));
    auto new_higher(parallel_quicksort(std::move(input)));
    result.splice(result.begin(), new_lower_future.get());
    result.splice(result.end(), new_higher);
    return result;
}

#endif //CODE_SNIP_PARALLEL_QUICKSORT_H
