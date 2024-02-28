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

using namespace std;

// std::move无条件的将它的实参转换为右值
template<typename T>//在std命名空间
typename remove_reference<T>::type && // 相当于 无引用的T &&move(T &&param)
move(T &&param)
{
    using ReturnType = typename remove_reference<T>::type &&;
    return static_cast<ReturnType>(param);
}

int main()
{

    return 0;
}