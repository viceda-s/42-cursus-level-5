# CPP Module 06

*This project has been created as part of the 42 curriculum by viceda-s.*

## Description

CPP Module 06 focuses on **C++ type casting** and type conversions. This module explores the different casting operators available in C++ (static_cast, dynamic_cast, reinterpret_cast, const_cast) and when to use each one. Understanding type conversion is crucial for writing safe, maintainable C++ code. The project consists of three exercises:

- **Exercise 00 (Scalar Converter)**: Converting string literals to different scalar types (char, int, float, double)
- **Exercise 01 (Serialization)**: Converting pointers to integers and back using reinterpret_cast
- **Exercise 02 (Type Identification)**: Using dynamic_cast to identify the actual type of objects in a class hierarchy

### Learning Objectives

- Understanding the four C++ cast operators
- Learning when to use each cast type
- Understanding static vs dynamic type identification
- Working with RTTI (Run-Time Type Information)
- Mastering pointer conversions and reinterpretation
- Understanding type safety and its limitations

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

#### Exercise 00 - Scalar Converter
```bash
cd ex00
make
./convert "42"
./convert "42.0f"
./convert "nan"
./convert "c"
```

#### Exercise 01 - Serialization
```bash
cd ex01
make
./serialize
```

#### Exercise 02 - Type Identification
```bash
cd ex02
make
./identify
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
cpp06/
├── ex00/                    # Scalar type conversion
│   ├── ScalarConverter.hpp # Static class for conversions
│   ├── ScalarConverter.cpp # Conversion logic implementation
│   ├── main.cpp            # Test program (takes one argument)
│   └── Makefile
├── ex01/                    # Serialization
│   ├── Data.hpp            # Simple data structure
│   ├── Serializer.hpp      # Static serialization class
│   ├── Serializer.cpp      # Serialize/deserialize implementation
│   ├── main.cpp            # Test program
│   └── Makefile
└── ex02/                    # Dynamic type identification
    ├── Base.hpp            # Base class with virtual destructor
    ├── Base.cpp
    ├── A.hpp               # Derived class A
    ├── B.hpp               # Derived class B
    ├── C.hpp               # Derived class C
    ├── Functions.hpp       # Utility functions declarations
    ├── Functions.cpp       # generate, identify implementations
    ├── main.cpp            # Test program
    └── Makefile
```

## Features

### Exercise 00: ScalarConverter

A static class that converts string literals to scalar types:

**Supported Input Formats**:
- **char literals**: Single character (e.g., `'a'`, `'Z'`)
- **int literals**: Integer numbers (e.g., `"42"`, `"-42"`)
- **float literals**: Floating-point with 'f' suffix (e.g., `"42.0f"`, `"-4.2f"`)
- **double literals**: Floating-point numbers (e.g., `"42.0"`, `"-4.2"`)
- **Pseudo literals**: `nan`, `nanf`, `inf`, `inff`, `-inf`, `-inff`, `+inf`, `+inff`

**Output Format**:
```
char: [value or 'impossible' or 'Non displayable']
int: [value or 'impossible']
float: [value with .0f]
double: [value]
```

**Implementation Details**:
- Static class (non-instantiable)
- Private constructors and destructor
- Type detection logic to identify input format
- Conversion logic for each type
- Special handling for pseudo literals (infinity, NaN)
- Display formatting with precision

**Example**:
```bash
./convert "42"
char: '*'
int: 42
float: 42.0f
double: 42.0

./convert "nan"
char: impossible
int: impossible
float: nanf
double: nan
```

### Exercise 01: Serialization

Implements pointer serialization using `uintptr_t`:

**Serializer Class**:
- Static class (non-instantiable)
- `serialize(Data* ptr)`: Converts pointer to unsigned integer
- `deserialize(uintptr_t raw)`: Converts integer back to pointer

**Data Structure**:
```cpp
struct Data {
    int value;
    std::string name;
    // ... other members
};
```

**Usage**:
```cpp
Data* original = new Data;
uintptr_t serialized = Serializer::serialize(original);
Data* deserialized = Serializer::deserialize(serialized);
// original == deserialized (same address)
```

**Key Concepts**:
- `uintptr_t`: Unsigned integer type large enough to hold a pointer
- `reinterpret_cast`: Reinterprets bit pattern without conversion
- Round-trip conversion (serialize → deserialize) preserves pointer value
- Useful for low-level operations, IPC, or saving pointers

### Exercise 02: Type Identification

Implements runtime type identification using `dynamic_cast`:

**Class Hierarchy**:
```
    Base (abstract, virtual destructor)
     / | \
    A  B  C
```

**Functions**:
1. `Base* generate(void)`:
   - Randomly instantiates A, B, or C
   - Returns as Base pointer

2. `void identify(Base* p)`:
   - Identifies actual type using `dynamic_cast`
   - Prints "A", "B", or "C"

3. `void identify(Base& p)`:
   - Identifies actual type without using pointers
   - Uses `dynamic_cast` with references (catches bad_cast exception)
   - Prints "A", "B", or "C"

**Key Concepts**:
- `dynamic_cast<Type*>(ptr)`: Returns nullptr if cast fails
- `dynamic_cast<Type&>(ref)`: Throws `std::bad_cast` if cast fails
- RTTI (Run-Time Type Information) required for dynamic_cast
- Works only with polymorphic types (classes with virtual functions)

## Algorithm and Data Structure Explanation

### The Four C++ Casts

C++ provides four cast operators, each with specific use cases:

#### 1. static_cast<Type>(expression)

**Purpose**: Compile-time type conversion between related types

**Use Cases**:
- Numeric conversions (int to float, double to int)
- Pointer conversions up/down class hierarchy (without runtime check)
- Explicit conversions that would be implicit
- Converting void* to typed pointer

**In this project** (ex00):
```cpp
int intValue = static_cast<int>(doubleValue);
char charValue = static_cast<char>(intValue);
```

**Safety**: Compile-time only; no runtime verification

#### 2. dynamic_cast<Type>(expression)

**Purpose**: Runtime type checking for polymorphic types

**Use Cases**:
- Safely downcasting in class hierarchies
- Type identification (check actual derived type)
- Only works with pointers/references to polymorphic classes

**In this project** (ex02):
```cpp
A* aPtr = dynamic_cast<A*>(basePtr);  // Returns nullptr if not A
if (aPtr)
    std::cout << "Type is A" << std::endl;
```

**Safety**: Runtime checked; safe downcasting

**Requirements**:
- Base class must have at least one virtual function
- Enables RTTI (Run-Time Type Information)
- Compiler flag: `-fno-rtti` disables it

#### 3. reinterpret_cast<Type>(expression)

**Purpose**: Low-level reinterpretation of bit pattern

**Use Cases**:
- Pointer to integer conversion
- Integer to pointer conversion
- Pointer type conversions (unsafe)
- Hardware-level programming

**In this project** (ex01):
```cpp
uintptr_t serialized = reinterpret_cast<uintptr_t>(dataPtr);
Data* deserialized = reinterpret_cast<Data*>(serialized);
```

**Safety**: No safety checks; programmer's responsibility

**Warning**: Most dangerous cast; can break type system

#### 4. const_cast<Type>(expression)

**Purpose**: Add or remove const/volatile qualifiers

**Use Cases**:
- Interfacing with legacy APIs that don't use const
- Removing const in rare justified cases

**Not used in this project**, but example:
```cpp
const int* constPtr = &value;
int* mutablePtr = const_cast<int*>(constPtr);  // Remove const
```

**Safety**: Modifying a truly const object is undefined behavior

### Comparison Table

| Cast | Runtime Check | Type Safety | Use Case | Speed |
|------|---------------|-------------|----------|-------|
| **static_cast** | No | Medium | Related types, up/down cast | Fast |
| **dynamic_cast** | Yes | High | Safe downcasting, type ID | Slow |
| **reinterpret_cast** | No | None | Bit reinterpretation, pointers↔integers | Fast |
| **const_cast** | No | Low | Add/remove const | Fast |

### Type Detection Strategy (ex00)

The ScalarConverter uses a detection pipeline:

```
Input String
    ↓
Is it a pseudo-literal? (nan, inf, etc.)
    ↓ No
Is it a char? (single character, possibly in quotes)
    ↓ No
Is it an int? (digits only, optional sign)
    ↓ No
Is it a float? (digits with decimal, 'f' suffix)
    ↓ No
Is it a double? (digits with decimal)
    ↓ No
Invalid input
```

**Implementation Approach**:
1. Check for special literals first
2. Try to parse as each type
3. Detect by pattern (regex or manual parsing)
4. Convert to internal representation
5. Convert from internal to all four types
6. Handle edge cases (overflow, non-displayable chars)

### Serialization and Pointer Conversion (ex01)

**Why serialize pointers?**
- Store pointer addresses for later use
- Send pointers over network (address space reconstruction)
- Save object relationships in files

**uintptr_t characteristics**:
- Defined in `<stdint.h>` (C++11: `<cstdint>`)
- Unsigned integer type
- Guaranteed to hold any pointer value
- Size equals pointer size (32-bit or 64-bit)

**Round-trip guarantee**:
```cpp
void* p = /* some pointer */;
uintptr_t i = reinterpret_cast<uintptr_t>(p);
void* p2 = reinterpret_cast<void*>(i);
assert(p == p2);  // Guaranteed to be true
```

### RTTI and dynamic_cast (ex02)

**How dynamic_cast works**:
1. Compiler adds type information to polymorphic classes
2. Type info stored in vtable (virtual function table)
3. dynamic_cast checks type info at runtime
4. If cast is valid, returns pointer; otherwise nullptr

**Vtable structure** (simplified):
```
Object Memory:
[vptr] → [Vtable] → [Type Info] [Virtual Functions...]
[data members...]
```

**Performance considerations**:
- dynamic_cast has runtime overhead
- Requires vtable lookup and type checking
- Slower than static_cast
- Trade-off: safety vs speed

**Alternative to dynamic_cast**:
- Virtual functions (preferred)
- Type fields (error-prone)
- Visitor pattern (complex but efficient)

### Static Classes Pattern

Exercises 00 and 01 use static classes:

**Characteristics**:
- All methods are static
- Cannot be instantiated
- Private constructors and destructor
- Acts as namespace with access control

**Implementation**:
```cpp
class ScalarConverter {
private:
    ScalarConverter();                                // No default construction
    ScalarConverter(const ScalarConverter& other);    // No copy
    ScalarConverter& operator=(const ScalarConverter& other);  // No assignment
    ~ScalarConverter();                               // No destruction

public:
    static void convert(const std::string& literal); // Static method
};
```

**Benefits**:
- Prevents instantiation (all operations static)
- Clear intent: utility class, not an object
- Avoids global namespace pollution

## Resources

### Documentation
- [cppreference.com - static_cast](https://en.cppreference.com/w/cpp/language/static_cast)
- [cppreference.com - dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast)
- [cppreference.com - reinterpret_cast](https://en.cppreference.com/w/cpp/language/reinterpret_cast)
- [cppreference.com - const_cast](https://en.cppreference.com/w/cpp/language/const_cast)
- [cppreference.com - RTTI](https://en.cppreference.com/w/cpp/language/typeid)

### Articles and Tutorials
- [LearnCPP.com - Type Casting](https://www.learncpp.com/cpp-tutorial/introduction-to-type-conversion/)
- [GeeksforGeeks - Type Casting in C++](https://www.geeksforgeeks.org/casting-operators-in-cpp/)
- [C++ Type Conversions](https://www.learncpp.com/cpp-tutorial/explicit-type-conversion-casting/)

### Books
- "Effective C++" by Scott Meyers (Items on casting)
- "More Effective C++" by Scott Meyers (Cast usage guidelines)
- "The C++ Programming Language" by Bjarne Stroustrup (Type conversion chapter)

### AI Usage

AI was **not** used in the development of this project. All code was written manually to ensure full understanding of:
- The four C++ cast operators and their appropriate use cases
- Type detection and conversion logic
- Serialization using reinterpret_cast
- RTTI and dynamic_cast mechanisms
- Static class patterns

The README documentation was also written without AI assistance, based on understanding gained through:
- Reading official C++ documentation
- Studying type conversion mechanics
- Implementing and testing various casting scenarios
- Understanding vtable and RTTI internals

This hands-on approach ensured deep comprehension of C++ type system and casting operations.

## Testing

### Exercise 00 Tests

Test with various inputs:

**Character inputs**:
```bash
./convert "a"
./convert "z"
./convert " "  # non-displayable chars
```

**Integer inputs**:
```bash
./convert "0"
./convert "42"
./convert "-42"
./convert "2147483647"   # INT_MAX
./convert "-2147483648"  # INT_MIN
```

**Float inputs**:
```bash
./convert "0.0f"
./convert "42.42f"
./convert "-4.2f"
```

**Double inputs**:
```bash
./convert "0.0"
./convert "42.42"
./convert "-4.2"
```

**Pseudo-literals**:
```bash
./convert "nan"
./convert "nanf"
./convert "+inf"
./convert "-inff"
```

**Edge cases**:
```bash
./convert "999999999999"  # Overflow
./convert "0.000001"      # Small numbers
./convert ""              # Empty string
```

### Exercise 01 Tests

Test serialization/deserialization:
```cpp
Data original;
original.value = 42;

uintptr_t raw = Serializer::serialize(&original);
Data* ptr = Serializer::deserialize(raw);

assert(ptr == &original);  // Same address
assert(ptr->value == 42);   // Same data
```

### Exercise 02 Tests

Test type identification:
- Generate random objects multiple times
- Verify identify by pointer correctly identifies type
- Verify identify by reference correctly identifies type
- Test all three types (A, B, C)

### Memory Testing

Run with memory leak detection:
```bash
valgrind --leak-check=full ./convert "42"
valgrind --leak-check=full ./identify
```

## Notes

- All code follows the **C++98 standard** (no C++11 or later features)
- Never use C-style casts in C++ (avoid `(Type)value`)
- `dynamic_cast` only works with polymorphic types (virtual functions)
- `reinterpret_cast` is dangerous; use only when necessary
- Prefer `static_cast` for most conversions
- Exercise 00 may require parsing logic or `<sstream>` for conversions
- RTTI can be disabled with compiler flags (affects `dynamic_cast`)

## Author

**viceda-s**  
*42 Luxembourg*

---

*For questions, issues, or suggestions, please refer to the 42 project evaluation guidelines.*

