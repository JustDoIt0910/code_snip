//
// Created by just do it on 2023/12/10.
//
#include <tuple>
#include <string>
#include <iostream>

template <typename T>
struct function_traits: function_traits<decltype(&T::operator())> {};

template <typename R, typename C, typename... Args>
struct function_traits<R(C::*)(Args...)>: function_traits<R(*)(Args...)> {};

template <typename R, typename C, typename... Args>
struct function_traits<R(C::*)(Args...) const>: function_traits<R(*)(Args...)> {};

template <typename R, typename... Args>
struct function_traits<R(*)(Args...)> {
    using result_type = R;
    static constexpr size_t args_count = sizeof...(Args);
    using args_type = std::tuple<typename std::decay<Args>::type...>;

    template<size_t I>
    struct typeof_arg_n {
        using type = std::remove_reference_t<
                decltype(std::get<I - 1>(std::declval<args_type>()))>;
    };
};

template<typename F, size_t Idx>
using arg_type = typename function_traits<F>::template typeof_arg_n<Idx>::type;



int f(int a, char b) {
    return a + b;
}

class C {
public:
    void method(std::string) {}
    void const_method(int, int) const {}

    bool operator()(int x) { return false; }
};

int main() {
    constexpr size_t args_count_of_f = function_traits<decltype(&f)>::args_count;
    static_assert(args_count_of_f == 2, "");
    std::cout << "return type of f: " << typeid(function_traits<decltype(&f)>::result_type).name() << std::endl;
    std::cout << "first param type of f: " << typeid(arg_type<decltype(&f), 1>).name() << std::endl;
    std::cout << "second param type of f: " << typeid(arg_type<decltype(&f), 2>).name() << std::endl;

    constexpr size_t args_count_of_cm = function_traits<decltype(&C::method)>::args_count;
    static_assert(args_count_of_cm == 1, "");
    std::cout << "return type of C::method(): " << typeid(function_traits<decltype(&f)>::result_type).name() << std::endl;
    std::cout << "first param type of C::method(): " << typeid(arg_type<decltype(&C::method), 1>).name() << std::endl;

    constexpr size_t args_count_of_operator = function_traits<C>::args_count;
    static_assert(args_count_of_operator == 1, "");
    std::cout << "return type of C::operator(): " << typeid(function_traits<C>::result_type).name() << std::endl;
    std::cout << "first param type of C::method(): " << typeid(arg_type<C, 1>).name() << std::endl;
}