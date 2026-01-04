//
// Created by viceda-s on 04/01/2026.
//

#include "Span.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(void) {
    // Basic test from subject
    std::cout << "=== Basic Test ===" << std::endl;
    Span sp = Span(5);

    sp.addNumber(6);
    sp.addNumber(3);
    sp.addNumber(17);
    sp.addNumber(9);
    sp.addNumber(11);

    std::cout << sp.shortestSpan() << std::endl;
    std::cout << sp.longestSpan() << std::endl;

    // Test with 10,000 numbers
    std::cout << "\n=== Large Scale Test (10,000 numbers) ===" << std::endl;
    Span sp2 = Span(10000);
    srand(time(NULL));

    for (int i = 0; i < 10000; ++i) {
        sp2.addNumber(rand() % 1000);
    }

    std::cout << "Shortest span: " << sp2.shortestSpan() << std::endl;
    std::cout << "Longest span: " << sp2.longestSpan() << std::endl;

    // Test with iterator range
    std::cout << "\n=== Iterator Range Test ===" << std::endl;
    std::vector<int> numbers;
    numbers.push_back(1);
    numbers.push_back(2);
    numbers.push_back(4);
    numbers.push_back(8);
    numbers.push_back(16);

    Span sp3 = Span(10);
    sp3.addNumbers(numbers.begin(), numbers.end());
    std::cout << "Shortest span: " << sp3.shortestSpan() << std::endl;
    std::cout << "Longest span: " << sp3.longestSpan() << std::endl;

    // Error handling
    std::cout << "\n=== Error Handling ===" << std::endl;
    Span sp4 = Span(2);
    sp4.addNumber(42);

    try {
        sp4.addNumber(100);
        sp4.addNumber(200);
    } catch (const std::exception &e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    try {
        Span sp5 = Span(1);
        sp5.addNumber(42);
        sp5.shortestSpan();
    } catch (const std::exception &e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}