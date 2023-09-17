//
// Created by just do it on 2023/9/18.
//
#include <string>
#include <iostream>
#include <unordered_set>
#include <assert.h>

class Object
{
public:
    Object(const std::string& f1, const std::string& f2, int f3):
    field1(f1), field2(f2), field3(f3) {}

    bool operator==(const Object& o) const
    {
        return (field1 == o.field1 && field2 == o.field2 && field3 == o.field3);
    }

    std::string field1;
    std::string field2;
    int field3;
};

class ObjectHash
{
public:
    size_t operator()(const Object& obj) const
    {
        return hash_val(obj.field1, obj.field2, obj.field3);
    }

private:
    template<typename... Args>
    [[nodiscard]] size_t hash_val(const Args&... args) const
    {
        size_t seed = 0;
        hash_value(seed, args...);
        return seed;
    }

    template<typename T, typename... Args>
    void hash_value(size_t& seed, const T& t, const Args&... args) const
    {
        hash_combine(seed, t);
        hash_value(seed, args...);
    }

    template<typename T>
    void hash_value(size_t& seed, const T& t) const
    {
        hash_combine(seed, t);
    }

    // about hash_combine: https://zhuanlan.zhihu.com/p/574573421
    template<typename T>
    void hash_combine(size_t& seed, const T& t) const
    {
        seed ^= std::hash<T>()(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

int main()
{
    Object objs[] = {{"abc", "def", 100},
                     {"aaa", "def", 100},
                     {"abc", "bbb", 100},
                     {"abc", "def", 101}};
    std::unordered_set<Object, ObjectHash> s;
    for(const auto & obj : objs)
    {
        assert(s.insert(obj).second == true);
    }
    assert(s.insert(objs[0]).second == false);
}