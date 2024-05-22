//
// Created by Samsara on 2024/2/28.
// File name: Item23.cpp
// Description: 理解std::move和std::forward
//
// std::remove_reference
// 用于从给定类型中移除引用修饰符
// 如果有一个类型 T 是 int, int&或int&&，那么 std::remove_reference<T>::type 将返回 int
//
// typename
// 模板可能会理解错误一些东西，比如 x *tmp，这可以被理解为乘法，或者是定义一个指针
// typename会将x直接定义为类型，上面的例子也就没有了乘法的可能
//
#include <type_traits>
#include <algorithm>
#include <iostream>

using std::cout, std::remove_reference, std::endl;

class Array
{
public:
    int size_;
    int *data_;

    explicit Array(int size) : size_(size), data_(new int[size]) {}

    Array(Array &other) : size_(other.size_), data_(new int[size_])
    {
        for (int i = 0; i < size_; ++i)
        {
            data_[i] = other.data_[i];
        }
    }

    Array(Array &&other)  noexcept : size_(other.size_), data_(other.data_)
    {
        // 因为现在这个对象指向了other原来内存的内容，如果不清除other的指针，那么other析构时，会把data_也清除
        // 因为是std::move，返回右值引用
        other.data_ = nullptr;
    }

    virtual ~Array()
    {
        delete[] data_;
    }
};

// 为什么a = std::move(b)之后，b可能会用不了了？
// std::move只是把左值转换成右值，比如a = std::move(b)，则是相当于a = 右值b
// 然后因为等于号相当于.operator=()，所以就变为了a.operator=(右值b)
// =号是用来调用类型的赋值函数的，而如果=的右边是右值，则会调用对应的移动赋值函数
// 如果对应的移动赋值函数中，将原有的内容删除，则b就用不了了，但是这需要你自己决定
// 通常我们使用move就是为了这样，所以一般我们也这样实现，所以不是move做了一切，而是我们做了一切

int main()
{
    // Array b(100000);
    // auto a = std::move(b);
    // cout << a.data_ << endl;
    // cout << b.data_ << endl;

    // 2024-5-6
    std::string s1 = "10000";
    std::string s2 = "2";
    s1 = std::move(s2);

    // basic_string& operator=(basic_string&& __str)使用相同的分配器时，会直接转移指针
    // （在大多数情况下，字符串对象会使用默认分配器 std::allocator，所以 _allocator 是相同的。）
    // if (__equal_allocs)
    // {
    //   // __str can reuse our existing storage.
    //   __data = _M_data();
    //   __capacity = _M_allocated_capacity;
    // }

    // 最后，会进行__str.clear()，实现如下
    // void
    // clear() _GLIBCXX_NOEXCEPT
    // { _M_set_length(0); }
    // 原来的内存被s1占走，s2的长度设为0，并置最后一位为结束符


    return 0;
}

// std::move无条件的将它的实参转换为右值
template<typename T>//在std命名空间
typename remove_reference<T>::type && // 相当于 无引用的T &&move(T &&param)
move(T &&param)
{
    using ReturnType = typename remove_reference<T>::type &&;
    // 返回时将传入的通用引用参数变为右值
    return static_cast<ReturnType>(param);
}