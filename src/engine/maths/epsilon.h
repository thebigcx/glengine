#pragma once

template<typename T>
static inline constexpr T epsilon()
{
    return std::numeric_limits<T>::epsilon();
}

template<typename T>
static bool epsilon_equal(const T& x, const T& y, const T& epsilon)
{
    return std::abs(x - y) < epsilon;
}

template<typename T>
static bool epsilon_not_equal(const T& x, const T& y, const T& epsilon)
{
    return std::abs(x - y) >= epsilon;
}