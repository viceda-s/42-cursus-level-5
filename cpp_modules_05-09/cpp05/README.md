# CPP Module 05

*This project has been created as part of the 42 curriculum by viceda-s.*

## Description

CPP Module 05 introduces **exceptions and exception handling** in C++, a crucial mechanism for error handling and control flow in modern C++ programming. This module focuses on creating custom exceptions, using try-catch blocks, and understanding how exceptions propagate through the call stack. The project uses a bureaucracy/form system metaphor to demonstrate these concepts.

The project is divided into four exercises:

- **Exercise 00 (Bureaucrat)**: Implementation of a Bureaucrat class with grade validation using exceptions
- **Exercise 01 (Form)**: Adding a Form class that bureaucrats can sign, with exception handling for invalid operations
- **Exercise 02 (Concrete Forms)**: Creating specific form types (ShrubberyCreationForm, RobotomyRequestForm, PresidentialPardonForm) with execution capabilities
- **Exercise 03 (Intern)**: Implementing an Intern class that can create forms dynamically

### Learning Objectives

- Understanding exception handling mechanism in C++
- Creating custom exception classes
- Using try-catch blocks effectively
- Understanding exception propagation and stack unwinding
- Learning about RAII and exception safety
- Implementing the Template Method pattern
- Working with abstract classes and polymorphism with exceptions

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

#### Exercise 00 - Bureaucrat
```bash
cd ex00
make
./Bureaucrat
```

#### Exercise 01 - Form
```bash
cd ex01
make
./Form
```

#### Exercise 02 - Concrete Forms
```bash
cd ex02
make
./Forms
```

#### Exercise 03 - Intern
```bash
cd ex03
make
./Intern
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
cpp05/
├── ex00/                    # Basic exception handling
│   ├── Bureaucrat.hpp      # Bureaucrat class with custom exceptions
│   ├── Bureaucrat.cpp      # Implementation
│   ├── main.cpp            # Test program
│   └── Makefile
├── ex01/                    # Forms and signing
│   ├── Bureaucrat.hpp      # Bureaucrat class
│   ├── Bureaucrat.cpp
│   ├── Form.hpp            # Form class with grade requirements
│   ├── Form.cpp
│   ├── main.cpp            # Test program
│   └── Makefile
├── ex02/                    # Concrete form implementations
│   ├── Bureaucrat.hpp      # Bureaucrat class
│   ├── Bureaucrat.cpp
│   ├── AForm.hpp           # Abstract form base class
│   ├── AForm.cpp
│   ├── ShrubberyCreationForm.hpp    # Creates ASCII trees
│   ├── ShrubberyCreationForm.cpp
│   ├── RobotomyRequestForm.hpp      # Robotomy with random success
│   ├── RobotomyRequestForm.cpp
│   ├── PresidentialPardonForm.hpp   # Presidential pardon
│   ├── PresidentialPardonForm.cpp
│   ├── main.cpp            # Test program
│   └── Makefile
└── ex03/                    # Form factory pattern
    ├── Bureaucrat.hpp      # Bureaucrat class
    ├── Bureaucrat.cpp
    ├── AForm.hpp           # Abstract form base class
    ├── AForm.cpp
    ├── ShrubberyCreationForm.hpp
    ├── ShrubberyCreationForm.cpp
    ├── RobotomyRequestForm.hpp
    ├── RobotomyRequestForm.cpp
    ├── PresidentialPardonForm.hpp
    ├── PresidentialPardonForm.cpp
    ├── Intern.hpp          # Form factory
    ├── Intern.cpp
    ├── main.cpp            # Test program
    └── Makefile
```

## Features

### Exercise 00: Bureaucrat Class

Implements a bureaucrat with grade-based validation:
- **Attributes**:
  - `const std::string _name`: Bureaucrat's name (immutable)
  - `int _grade`: Grade from 1 (highest) to 150 (lowest)
- **Member Functions**:
  - `incrementGrade()`: Increases grade (decreases number)
  - `decrementGrade()`: Decreases grade (increases number)
  - Getters for name and grade
- **Custom Exceptions**:
  - `GradeTooHighException`: Thrown when grade < 1
  - `GradeTooLowException`: Thrown when grade > 150
- **Stream Operator**: Overloaded `<<` for formatted output

**Key Concept**: Exceptions are thrown when invalid operations are attempted (grade out of range).

### Exercise 01: Form Class

Adds a Form class that can be signed by bureaucrats:
- **Attributes**:
  - `const std::string _name`: Form name
  - `bool _isSigned`: Whether form is signed
  - `const int _signGrade`: Minimum grade required to sign
  - `const int _execGrade`: Minimum grade required to execute
- **Member Functions**:
  - `beSigned(const Bureaucrat& bureaucrat)`: Signs form if bureaucrat's grade is sufficient
  - Getters for all attributes
- **Custom Exceptions**:
  - `GradeTooHighException`: Invalid grade (< 1)
  - `GradeTooLowException`: Bureaucrat's grade too low to sign
- **Bureaucrat Extension**:
  - `signForm(Form& form)`: Bureaucrat attempts to sign a form

### Exercise 02: Concrete Form Types

Creates an abstract `AForm` base class and three concrete implementations:

#### AForm (Abstract Base)
- Adds `execute(const Bureaucrat& executor)` pure virtual method
- Validates executor's grade before execution
- Template Method pattern: `execute()` validates, then calls `executeAction()`

#### ShrubberyCreationForm
- **Grades**: Sign 145, Execute 137
- **Action**: Creates a file `<target>_shrubbery` with ASCII trees
- **File I/O**: Writes tree art to file system

#### RobotomyRequestForm
- **Grades**: Sign 72, Execute 45
- **Action**: Makes drilling noises and announces robotomy
- **Random Success**: 50% chance of success or failure

#### PresidentialPardonForm
- **Grades**: Sign 25, Execute 5
- **Action**: Announces that target has been pardoned by Zaphod Beeblebrox

**Execution Flow**:
1. Check if form is signed
2. Check if executor's grade is sufficient
3. Call specific `executeAction()` implementation
4. Throw exceptions for invalid states

### Exercise 03: Intern (Factory Pattern)

Implements a form factory:
- **Method**: `makeForm(const std::string& formName, const std::string& target)`
- **Supported Forms**:
  - "shrubbery creation"
  - "robotomy request"
  - "presidential pardon"
- **Return**: Pointer to newly created form, or NULL if form name invalid
- **Pattern**: Factory Method design pattern

**Usage**:
```cpp
Intern someRandomIntern;
AForm* form = someRandomIntern.makeForm("robotomy request", "Bender");
```

## Algorithm and Data Structure Explanation

### Exception Handling in C++

Exceptions provide a mechanism to transfer control from a point where an error occurs to a handler that can deal with the error.

#### Exception Flow

```
Try Block → Exception Thrown → Stack Unwinding → Catch Block
```

1. **Try Block**: Code that might throw exceptions
2. **Throw**: `throw ExceptionType()` transfers control
3. **Stack Unwinding**: Destructors called for all local objects
4. **Catch Block**: Handler catches exceptions of matching type

#### Custom Exception Classes

```cpp
class GradeTooHighException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "Grade is too high!";
    }
};
```

**Design Decisions**:
- Inherit from `std::exception` for standard exception interface
- Override `what()` to provide error message
- Use `throw()` specification (C++98) to indicate no exceptions thrown by `what()`
- Nested classes to organize related exceptions within their context

#### Exception Safety Guarantees

**Basic Guarantee**: If an exception is thrown, no resources are leaked (RAII)
**Strong Guarantee**: If an exception is thrown, program state is unchanged
**No-throw Guarantee**: Operation cannot fail (e.g., destructors)

In this project:
- Constructors provide **strong guarantee** (validate before constructing)
- `beSigned()` and `execute()` provide **basic guarantee**
- Destructors provide **no-throw guarantee**

### Design Patterns

#### Template Method Pattern (ex02)

The `AForm::execute()` method implements the Template Method pattern:

```cpp
void AForm::execute(const Bureaucrat& executor) const {
    // Common validation logic
    if (!_isSigned)
        throw FormNotSignedException();
    if (executor.getGrade() > _execGrade)
        throw GradeTooLowException();
    
    // Call derived class-specific implementation
    executeAction(executor);
}
```

**Benefits**:
- Common validation logic in one place
- Prevents code duplication
- Enforces consistent behavior across all form types

#### Factory Method Pattern (ex03)

The `Intern` class implements Factory Method:

```cpp
AForm* Intern::makeForm(const std::string& formName, const std::string& target) {
    // Array of form names and corresponding creation functions
    // Match formName and create appropriate form
    // Return pointer to new form or NULL
}
```

**Benefits**:
- Encapsulates object creation logic
- Allows creating objects without specifying exact class
- Easy to add new form types without changing client code

### Exception vs Error Codes

| Aspect | Exceptions | Error Codes |
|--------|-----------|-------------|
| **Separation** | Error handling separated from normal code | Mixed with normal code |
| **Propagation** | Automatic up the call stack | Must be manually checked at each level |
| **Impossible to Ignore** | Yes (program terminates if uncaught) | No (can be ignored) |
| **Constructors** | Can report errors | Cannot return error codes |
| **Performance** | Overhead only when thrown | Constant checking overhead |
| **C++98 Standard** | Yes | Yes |

**In this project**: Exceptions are appropriate because:
- Invalid grades are exceptional conditions (not common)
- Constructors need to validate parameters
- Clear separation between normal flow and error handling
- Errors need to propagate up to meaningful handling point

### Memory Management with Exceptions

**RAII (Resource Acquisition Is Initialization)** ensures exception safety:
- Resources acquired in constructor
- Resources released in destructor
- If exception thrown during construction, destructors called for completed members
- No manual cleanup needed

Example in ShrubberyCreationForm:
```cpp
std::ofstream file(filename.c_str());  // RAII: file closed when out of scope
if (!file.is_open())
    throw FileOpenException();
// file automatically closed even if exception thrown
```

### Inheritance and Polymorphism with Exceptions

- `AForm` is abstract (has pure virtual `executeAction()`)
- Concrete forms override `executeAction()`
- `execute()` is non-virtual (Template Method)
- Polymorphism allows storing different form types as `AForm*`
- Virtual destructor ensures proper cleanup through base pointer

## Resources

### Documentation
- [cppreference.com - Exceptions](https://en.cppreference.com/w/cpp/language/exceptions)
- [cppreference.com - try-catch](https://en.cppreference.com/w/cpp/language/try_catch)
- [cppreference.com - throw](https://en.cppreference.com/w/cpp/language/throw)
- [cppreference.com - std::exception](https://en.cppreference.com/w/cpp/error/exception)
- [Exception Safety](https://en.cppreference.com/w/cpp/language/exceptions#Exception_safety)

### Articles and Tutorials
- [LearnCPP.com - Exception Handling](https://www.learncpp.com/cpp-tutorial/exception-handling/)
- [GeeksforGeeks - Exception Handling in C++](https://www.geeksforgeeks.org/exception-handling-c/)
- [RAII and Exception Safety](https://en.cppreference.com/w/cpp/language/raii)

### Design Patterns
- "Design Patterns" by Gang of Four (Template Method, Factory Method)
- [Refactoring.Guru - Template Method](https://refactoring.guru/design-patterns/template-method)
- [Refactoring.Guru - Factory Method](https://refactoring.guru/design-patterns/factory-method)

### Books
- "Effective C++" by Scott Meyers (Items on exceptions)
- "More Effective C++" by Scott Meyers (Exception safety)
- "The C++ Programming Language" by Bjarne Stroustrup (Exception chapter)

### AI Usage

AI was **not** used in the development of this project. All code was written manually to ensure full understanding of:
- Exception handling mechanisms
- Custom exception class design
- Stack unwinding and RAII
- Template Method and Factory Method patterns
- Abstract classes with exceptions
- Exception safety guarantees

The README documentation was also written without AI assistance, based on understanding gained through:
- Reading official C++ documentation
- Studying exception handling best practices
- Implementing and testing various exception scenarios
- Debugging exception propagation issues

This hands-on approach ensured deep comprehension of C++ exception handling and design patterns.

## Testing

### Exception Testing Strategy

For each exercise, test:

1. **Valid Cases**:
   - Creating bureaucrats with valid grades (1-150)
   - Signing forms with sufficient grade
   - Executing forms when signed and grade sufficient

2. **Exception Cases**:
   - Grade too high (< 1) in constructor
   - Grade too low (> 150) in constructor
   - Incrementing/decrementing beyond valid range
   - Signing form without sufficient grade
   - Executing unsigned form
   - Executing form without sufficient grade

3. **Edge Cases**:
   - Grade exactly at boundary (1, 150)
   - Grade exactly at form requirement

### Example Test Pattern

```cpp
try {
    Bureaucrat bob("Bob", 0);  // Should throw GradeTooHighException
} catch (std::exception& e) {
    std::cout << "Exception caught: " << e.what() << std::endl;
}
```

### Exercise 02 Specific Tests

- **ShrubberyCreationForm**: Verify file creation and content
- **RobotomyRequestForm**: Test multiple times to verify randomness
- **PresidentialPardonForm**: Test with various bureaucrat grades

### Exercise 03 Specific Tests

Test Intern with:
- Valid form names (should create and return form)
- Invalid form names (should handle gracefully)
- All three form types

### Memory Testing

Run with memory leak detection:
```bash
valgrind --leak-check=full ./Forms
```

Pay special attention to:
- Form objects created by Intern (must be deleted)
- Exception throwing scenarios (RAII should prevent leaks)

## Notes

- All code follows the **C++98 standard** (no C++11 or later features)
- Destructors should never throw exceptions
- Use `const char* what() const throw()` for exception interface (C++98)
- Grade system is counter-intuitive: 1 is highest, 150 is lowest
- Always use RAII to ensure exception safety
- Factory pattern in ex03 returns raw pointer (no smart pointers in C++98)

## Author

**viceda-s**  
*42 Luxembourg*

---

*For questions, issues, or suggestions, please refer to the 42 project evaluation guidelines.*

