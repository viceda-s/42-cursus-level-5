//
// Created by viceda-s on 03/01/2026.
//

#ifndef WHATEVER_HPP
#define WHATEVER_HPP

#include <iostream>
#include <string>

// Exercise 00: Start with a few functions
// Function templates: swap, min, max

template <typename T>
void swap(T &a, T &b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <typename T>
T min(T a, T b) {
    return (a < b) ? a : b;
}

template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

#endif