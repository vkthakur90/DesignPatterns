#include <concepts>
#include <cstddef>
#include <iostream>
#include <cmath>

// 1. The NumericPolicy concept (policy first, then float type)
template<template <typename> class Policy, typename FloatType>
concept NumericPolicy =
    std::floating_point<FloatType> &&
    requires(std::size_t idx, FloatType a, FloatType b) {
        { Policy<FloatType>::initNum1(idx) }  -> std::same_as<FloatType>;
        { Policy<FloatType>::initNum2(idx) }  -> std::same_as<FloatType>;
        { Policy<FloatType>::compute(a, b) }  -> std::same_as<FloatType>;
    };

// 2. The Data struct (fixed result size)
template<std::floating_point T, std::size_t N>
struct Data {
    T num1[N]{};
    T num2[N]{};
    T result[N]{};
    static constexpr std::size_t size = N;
};

// 3. applyPolicy using the new concept order
template <template <typename> class Policy, std::floating_point FloatType, std::size_t N>
requires NumericPolicy<Policy, FloatType>
void applyPolicy(Data<FloatType, N>& d) {
    for (std::size_t i = 0; i < N; ++i) {
        d.num1[i]   = Policy<FloatType>::initNum1(i);
        d.num2[i]   = Policy<FloatType>::initNum2(i);
        d.result[i] = Policy<FloatType>::compute(d.num1[i], d.num2[i]);
    }
}

// 4. Example: a simple trigonometric policy
template <std::floating_point FloatType>
struct TrigPolicy {
    static FloatType initNum1(std::size_t i) { return std::sin(FloatType(i)); }
    static FloatType initNum2(std::size_t i) { return std::cos(FloatType(i)); }
    static FloatType compute(FloatType a, FloatType b)        { return a + b; }
};

int main() {
    Data<double, 8> d;
    applyPolicy<TrigPolicy>(d);

    for (std::size_t i = 0; i < d.size; ++i) {
        std::cout 
            << "i=" << i
            << "  sin="   << d.num1[i]
            << "  cos="   << d.num2[i]
            << "  sum="   << d.result[i]
            << "\n";
    }
    return 0;
}
