#include <iostream>
#include <array>
#include <string>

using dynamic_type = const void*;

struct handle {
private:
    using function_type = void(dynamic_type);

    dynamic_type m_arg;

    function_type* m_print;

    template<class T>
    static void print(dynamic_type arg) {
        std::cout << *static_cast<const T*>(arg);
    }

public:
    template<class T>
    handle(const T& arg) : m_arg(&arg), m_print(&print<T>) {}

    void do_print() { m_print(m_arg); }
};

template<class... Args>
std::array<handle, sizeof...(Args)> make_print_args(const Args&... args) {
    std::array<handle, sizeof...(Args)> result = {handle(args)...};
    return result;
}

template<class... Args>
void print(const Args&... args) {
    auto handles = make_print_args(args...);
    for (auto h : handles) {
        h.do_print();
        std::cout << ' ';
    }
    std::cout << '\n';
}

int main() {
  print("a", "b", "c", 1, 2, 3);
}
