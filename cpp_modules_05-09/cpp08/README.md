# CPP Module 08

*This project has been created as part of the 42 curriculum by viceda-s.*

## Description

CPP Module 08 focuses on **STL containers and iterators**, introducing the Standard Template Library (STL) and demonstrating how to work with templated containers and algorithms. This module emphasizes understanding iterators as the glue between containers and algorithms, and how to create custom container classes that integrate with the STL. The project consists of three exercises:

- **Exercise 00 (easyfind)**: A templated function to find values in STL containers
- **Exercise 01 (Span)**: A custom container class for storing integers and finding spans
- **Exercise 02 (MutantStack)**: Extending std::stack to make it iterable

### Learning Objectives

- Understanding STL containers (vector, list, stack, etc.)
- Mastering iterators and their categories
- Learning STL algorithms and how to use them
- Creating custom containers that work with STL
- Understanding container adapters
- Implementing iterator access in custom containers
- Working with template member functions

## Instructions

### Compilation Requirements

- **Compiler**: `c++` (or `g++`, `clang++`)
- **Standard**: C++98
- **Flags**: `-Wall -Wextra -Werror -std=c++98`

### Compilation

Each exercise has its own Makefile. Navigate to the desired exercise directory and run:

```bash
make
```

This will compile the exercise and create an executable.

#### Exercise 00 - easyfind
```bash
cd ex00
make
./easyfind
```

#### Exercise 01 - Span
```bash
cd ex01
make
./Span
```

#### Exercise 02 - MutantStack
```bash
cd ex02
make
./MutantStack
```

### Cleaning

To remove object files:
```bash
make clean
```

To remove object files and executable:
```bash
make fclean
```

To recompile everything:
```bash
make re
```

## Project Structure

```
cpp08/
├── ex00/                    # easyfind function template
│   ├── easyfind.hpp        # Template function implementation
│   ├── main.cpp            # Test program
│   └── Makefile
├── ex01/                    # Span container class
│   ├── Span.hpp            # Span class declaration and template methods
│   ├── Span.cpp            # Span class implementation
│   ├── main.cpp            # Test program
│   └── Makefile
└── ex02/                    # MutantStack iterable stack
    ├── MutantStack.hpp     # MutantStack template class
    ├── main.cpp            # Test program (compare with list)
    └── Makefile
```

## Features

### Exercise 00: easyfind

A templated function that finds an integer in any STL container:

**Function Signature**:
```cpp
template <typename T>
typename T::iterator easyfind(T& container, int value);
```

**Behavior**:
- Takes a container of type T and an integer value
- Uses `std::find()` algorithm to search the container
- Returns an iterator to the first occurrence of value
- Throws an exception if value is not found

**Key Concepts**:
- Template function works with any container type
- Uses container's iterator type (typename T::iterator)
- Integrates with STL algorithms (std::find)
- Exception handling for not-found case

**Works With**:
- `std::vector<int>`
- `std::list<int>`
- `std::deque<int>`
- Any STL container that holds integers and provides iterators

**Example**:
```cpp
std::vector<int> vec;
vec.push_back(1);
vec.push_back(42);
vec.push_back(3);

try {
    std::vector<int>::iterator it = easyfind(vec, 42);
    std::cout << "Found: " << *it << std::endl;
} catch (std::exception& e) {
    std::cout << "Not found" << std::endl;
}
```

### Exercise 01: Span

A custom container class for storing N integers and computing spans:

**Class Interface**:
```cpp
class Span {
public:
    Span(unsigned int N);  // Max N elements
    
    void addNumber(int number);              // Add single number
    int shortestSpan(void) const;           // Find smallest difference
    int longestSpan(void) const;            // Find largest difference
    
    template <typename InputIterator>
    void addNumbers(InputIterator begin, InputIterator end);  // Add range
};
```

**Features**:
- Stores up to N integers (throws exception if full)
- `shortestSpan()`: Returns minimum difference between any two numbers
- `longestSpan()`: Returns maximum difference (max - min)
- `addNumbers()`: Template method to add range of numbers using iterators
- Uses `std::vector<int>` internally for storage

**Span Calculation**:
- **Shortest**: Minimum absolute difference between any two elements
  - Algorithm: Sort, then find minimum consecutive difference
- **Longest**: Difference between maximum and minimum elements
  - Algorithm: Use std::max_element and std::min_element

**Exceptions**:
- Throws exception if span requested with < 2 elements
- Throws exception if adding number to full Span

**Example**:
```cpp
Span sp(5);
sp.addNumber(6);
sp.addNumber(3);
sp.addNumber(17);
sp.addNumber(9);
sp.addNumber(11);

std::cout << sp.shortestSpan() << std::endl;  // 2 (11-9 or 6-4)
std::cout << sp.longestSpan() << std::endl;   // 14 (17-3)

// Add range using iterators
std::vector<int> numbers;
// ... fill numbers ...
sp.addNumbers(numbers.begin(), numbers.end());
```

### Exercise 02: MutantStack

A templated stack class that extends `std::stack` to provide iterator access:

**Class Declaration**:
```cpp
template <typename T>
class MutantStack : public std::stack<T> {
public:
    typedef typename std::stack<T>::container_type::iterator iterator;
    typedef typename std::stack<T>::container_type::const_iterator const_iterator;
    typedef typename std::stack<T>::container_type::reverse_iterator reverse_iterator;
    typedef typename std::stack<T>::container_type::const_reverse_iterator const_reverse_iterator;
    
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
};
```

**Key Features**:
- Inherits from `std::stack<T>` (container adapter)
- Exposes underlying container's iterators
- Provides begin/end methods (like standard containers)
- Supports forward and reverse iteration
- Works exactly like std::stack but iterable

**The Problem with std::stack**:
- `std::stack` is a **container adapter** (wraps another container)
- Deliberately hides iterators to enforce LIFO semantics
- Can't traverse elements without popping them

**The MutantStack Solution**:
- Access underlying container (typically `std::deque`)
- Expose container's iterators through public methods
- Maintains stack interface while allowing iteration

**Example**:
```cpp
MutantStack<int> mstack;
mstack.push(5);
mstack.push(17);

// Iterate through stack (bottom to top)
MutantStack<int>::iterator it = mstack.begin();
MutantStack<int>::iterator ite = mstack.end();

while (it != ite) {
    std::cout << *it << std::endl;
    ++it;
}

// Still works as normal stack
mstack.pop();  // Removes 17
```

**Comparison with std::list**:
The main should demonstrate that MutantStack produces same output as std::list, showing it behaves like a standard container.

## Algorithm and Data Structure Explanation

### STL Overview

The Standard Template Library consists of:
1. **Containers**: Data structures (vector, list, map, etc.)
2. **Iterators**: Generalized pointers to traverse containers
3. **Algorithms**: Functions that work on ranges (sort, find, etc.)
4. **Function Objects**: Objects that can be called like functions

### Iterators: The Glue of STL

Iterators abstract the concept of position in a container:

```
Container <---> Iterator <---> Algorithm
```

**Iterator Categories**:

| Category | Operations | Examples |
|----------|-----------|----------|
| **Input Iterator** | Read, forward only, single-pass | istream_iterator |
| **Output Iterator** | Write, forward only, single-pass | ostream_iterator |
| **Forward Iterator** | Read/write, forward only, multi-pass | forward_list::iterator |
| **Bidirectional Iterator** | Read/write, forward/backward | list::iterator, map::iterator |
| **Random Access Iterator** | Read/write, random access, arithmetic | vector::iterator, deque::iterator |

**Why Iterators?**
- Decouple algorithms from containers
- One algorithm works with many containers
- Uniform interface for traversal
- Enable generic programming

### std::find Algorithm (ex00)

```cpp
template <typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, const T& value);
```

**How it works**:
1. Start at `first`, compare with `value`
2. If match, return current iterator
3. If not match, increment iterator
4. Repeat until match found or reach `last`
5. Return `last` if not found

**Complexity**: O(n) where n = distance(first, last)

**Generic**: Works with any container providing input iterators

### Span Algorithms (ex01)

#### Shortest Span Algorithm

**Approach 1: Brute Force**
- Compare all pairs: O(n²)
- Not efficient for large N

**Approach 2: Sort and Compare** (Optimal)
```cpp
int shortestSpan() {
    if (_numbers.size() < 2) throw exception;
    
    std::vector<int> sorted = _numbers;
    std::sort(sorted.begin(), sorted.end());
    
    int minSpan = INT_MAX;
    for (size_t i = 1; i < sorted.size(); ++i) {
        int span = sorted[i] - sorted[i-1];
        if (span < minSpan)
            minSpan = span;
    }
    return minSpan;
}
```
- **Complexity**: O(n log n) for sort + O(n) for iteration = O(n log n)

#### Longest Span Algorithm

```cpp
int longestSpan() {
    if (_numbers.size() < 2) throw exception;
    
    int min = *std::min_element(_numbers.begin(), _numbers.end());
    int max = *std::max_element(_numbers.begin(), _numbers.end());
    
    return max - min;
}
```
- **Complexity**: O(n) for min_element + O(n) for max_element = O(n)

### Container Adapters (ex02)

**Container Adapter**: Provides restricted interface to underlying container

Standard adapters:
- `std::stack`: LIFO (Last-In-First-Out)
- `std::queue`: FIFO (First-In-First-Out)
- `std::priority_queue`: Heap-based priority queue

**std::stack internals**:
```cpp
template <typename T, typename Container = std::deque<T>>
class stack {
protected:
    Container c;  // Underlying container

public:
    void push(const T& value) { c.push_back(value); }
    void pop() { c.pop_back(); }
    T& top() { return c.back(); }
    // No iterators exposed!
};
```

**MutantStack approach**:
- Inherit from std::stack
- Access protected member `c` (the underlying container)
- Expose `c.begin()` and `c.end()` through public methods

### Template Member Functions (ex01)

The `addNumbers` method is a template member function:

```cpp
template <typename InputIterator>
void addNumbers(InputIterator begin, InputIterator end) {
    for (InputIterator it = begin; it != end; ++it) {
        if (_numbers.size() >= _maxSize)
            throw std::out_of_range("Span is full");
        _numbers.push_back(*it);
    }
}
```

**Benefits**:
- Works with any iterator type (vector::iterator, list::iterator, pointers, etc.)
- Single implementation handles many use cases
- Type-safe (compile-time checking)

**Must be defined in header**: Template definitions must be visible at instantiation point

### Performance Considerations

**std::vector**:
- Random access: O(1)
- Insert at end: O(1) amortized
- Insert at middle: O(n)

**std::list**:
- Random access: O(n)
- Insert anywhere: O(1) if position known
- No random access iterators

**std::deque** (default for std::stack):
- Random access: O(1)
- Insert at ends: O(1)
- Insert at middle: O(n)
- Slightly slower than vector for random access

## Resources

### Documentation
- [cppreference.com - Containers](https://en.cppreference.com/w/cpp/container)
- [cppreference.com - Iterators](https://en.cppreference.com/w/cpp/iterator)
- [cppreference.com - Algorithms](https://en.cppreference.com/w/cpp/algorithm)
- [cppreference.com - std::stack](https://en.cppreference.com/w/cpp/container/stack)
- [cppreference.com - std::vector](https://en.cppreference.com/w/cpp/container/vector)
- [cppreference.com - std::find](https://en.cppreference.com/w/cpp/algorithm/find)

### Articles and Tutorials
- [LearnCPP.com - STL](https://www.learncpp.com/cpp-tutorial/introduction-to-the-standard-template-library-stl/)
- [LearnCPP.com - Iterators](https://www.learncpp.com/cpp-tutorial/introduction-to-iterators/)
- [GeeksforGeeks - STL in C++](https://www.geeksforgeeks.org/the-c-standard-template-library-stl/)

### Books
- "Effective STL" by Scott Meyers (50 specific ways to improve your use of STL)
- "The C++ Standard Library" by Nicolai Josuttis (comprehensive STL reference)
- "C++ Primer" by Stanley Lippman (STL chapters)

### AI Usage

AI was **not** used in the development of this project. All code was written manually to ensure full understanding of:
- STL containers and their interfaces
- Iterator concepts and categories
- STL algorithms and their usage
- Template member functions
- Container adapters and inheritance
- Iterator implementation and exposure

The README documentation was also written without AI assistance, based on understanding gained through:
- Reading official STL documentation
- Studying iterator patterns
- Implementing and testing container operations
- Understanding STL design philosophy

This hands-on approach ensured deep comprehension of STL architecture and best practices in generic programming.

## Testing

### Exercise 00 Tests

Test easyfind with:
- **std::vector**: Test found and not found cases
- **std::list**: Test found and not found cases
- **std::deque**: Additional container type
- **Edge cases**: Empty container, single element

```cpp
std::vector<int> vec;
vec.push_back(1);
vec.push_back(42);
vec.push_back(3);

try {
    easyfind(vec, 42);   // Should find
    easyfind(vec, 99);   // Should throw
} catch (std::exception& e) {
    // Handle not found
}
```

### Exercise 01 Tests

Test Span with:

1. **Basic Test** (from subject):
   ```cpp
   Span sp(5);
   sp.addNumber(6);
   sp.addNumber(3);
   sp.addNumber(17);
   sp.addNumber(9);
   sp.addNumber(11);
   
   std::cout << sp.shortestSpan() << std::endl;  // 2
   std::cout << sp.longestSpan() << std::endl;   // 14
   ```

2. **Large Scale Test**: 10,000+ numbers
3. **Iterator Range Test**: Use addNumbers with iterators
4. **Exception Tests**:
   - Span with 0 or 1 element (should throw)
   - Adding to full Span (should throw)

### Exercise 02 Tests

Test MutantStack by:

1. **Basic Operations**:
   ```cpp
   MutantStack<int> mstack;
   mstack.push(5);
   mstack.push(17);
   mstack.push(3);
   ```

2. **Iteration**:
   ```cpp
   for (MutantStack<int>::iterator it = mstack.begin(); 
        it != mstack.end(); ++it) {
       std::cout << *it << std::endl;
   }
   ```

3. **Comparison with std::list**:
   - Run same operations on MutantStack and std::list
   - Verify identical output

4. **Stack Operations**: Verify push/pop/top still work

### Memory Testing

Run with memory leak detection:
```bash
valgrind --leak-check=full ./Span
valgrind --leak-check=full ./MutantStack
```

## Notes

- All code follows the **C++98 standard** (no C++11 or later features)
- Template implementations must be in header files
- STL algorithms require `<algorithm>` header
- Use `typename` keyword for dependent types in templates
- Container adapters protect their underlying container (use protected inheritance)
- Iterator invalidation: be careful when modifying containers during iteration
- Span should handle edge cases (< 2 elements) with exceptions

## Author

**viceda-s**  
*42 Luxembourg*

---

*For questions, issues, or suggestions, please refer to the 42 project evaluation guidelines.*

