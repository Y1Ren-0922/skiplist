#pragma once

#include <type_traits>
#include <string>

template <typename T,typename std::enable_if_t<std::is_integral_v < T>, int> = 0>
int switch_type(const std::string &sv) {
    return std::stoi(sv);
}


template <typename T,typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
double switch_type(const std::string &sv) {
    return std::stod(sv);
}

template <typename T,typename std::enable_if_t<std::is_same_v<T, std::string>, int> = 0>
std::string switch_type(const std::string &sv) {
    return sv;
}
