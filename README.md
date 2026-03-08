# Milestone 5 – Completed 🟢

Welcome to my repository showcasing my progress in **Milestone 5** at 42 School. Below you'll find an overview of each project, the current status, and the challenges I'm facing as I continue to develop them.

## 🏅 My Scores

| Project            | Status / Score | Notable Features |
| :--                |:--------------:| :--              |
| CPP Modules 05–09  |  ✅ 5/5  | Advanced C++ (exceptions, templates, STL, containers) |
| ft_irc             |   🌟 125/100   | IRC server, TCP sockets, multiplexing, real-time communication |
| Inception          |   🌟 125/100   | Docker, Docker Compose, containerised infrastructure |
| webserv            |     🚧 WIP     | HTTP server, sockets, configuration parsing |

---

## 💬 ft_irc (w/ joao-alm) – 125/100 🌟

### Objective
Implement a fully functional **IRC server** in C++, compliant with the IRC protocol, capable of handling multiple clients simultaneously.

### Key Features Implemented
- TCP server using **non-blocking sockets**
- Client connection handling with `poll()`
- Core IRC commands (`PASS`, `NICK`, `USER`, `JOIN`, `PART`, `PRIVMSG`, `QUIT`, etc.)
- Channel management (operators, topics, modes)
- Protocol-compliant replies and robust error handling
- Bonus features implemented → **125/100**

### Challenges Faced
- Designing a scalable architecture for clients, channels, and commands
- Managing concurrency and asynchronous I/O
- Ensuring strict protocol compliance
- Handling edge cases such as partial reads, disconnects, and invalid commands

### Skills Developed
- Advanced C++ design and clean architecture
- Network programming with sockets
- Event-driven programming using `poll()`
- Implementation of real-world communication protocols

---

## 🖥️ CPP Modules 05–09 – 100/100 ✅

### Objective
Deepen C++ expertise through advanced language features and standard library usage.

### Challenges Faced
- Understanding the nuances of different cast types and when to appropriately use each one required careful study of type safety.
- Implementing exception handling without memory leaks and ensuring proper resource management in error scenarios.
- Mastering template syntax and debugging template compilation errors, which can be cryptic and difficult to interpret.
- Learning to efficiently use STL algorithms and choosing the right container for specific use cases based on performance requirements.

### Skills Developed
- Exception handling and error management, type casting and conversions, template programming and generic code design, STL containers and algorithms mastery, and performance optimization techniques.

### Module Details

1. **CPP05 – Exceptions and Error Handling**
   - **Objective:** Exception handling mechanisms, try-catch blocks, custom exception classes, and error propagation in C++.
   - **Score: 100/100 ✅**

2. **CPP06 – C++ Casts and Type Conversions**
   - **Objective:** Type casting in C++ including static_cast, dynamic_cast, const_cast, reinterpret_cast, and understanding type conversion rules.
   - **Score: 100/100 ✅**

3. **CPP07 – Templates and Generic Programming**
   - **Objective:** Function templates, class templates, template specialization, and writing generic, reusable code.
   - **Score: 100/100 ✅**

4. **CPP08 – STL Containers, Iterators, and Algorithms**
   - **Objective:** Standard Template Library containers (vector, list, map, etc.), iterators, and standard algorithms for data manipulation.
   - **Score: 100/100 ✅**

5. **CPP09 – Advanced STL Usage and Performance Considerations**
   - **Objective:** Complex STL usage patterns, container adapters (stack, queue), algorithm optimization, and performance analysis.
   - **Score: 100/100 ✅**

---

## 🐳 Inception – Work in Progress 🚧

### Objective
Build a complete containerized infrastructure using **Docker** and **Docker Compose**, following best practices for service isolation and deployment.

### Technologies & Concepts
- Docker & Docker Compose
- NGINX, WordPress, MariaDB
- Volumes, networks, and environment variables
- Service orchestration and container lifecycle

### Current Focus
- Understanding container networking and service dependencies
- Writing clean and maintainable Dockerfiles
- Managing multi-container setups in a production-like environment

---

## 🌐 webserv (w/ brunmart, mdelima-m, rbaldin) – Work in Progress 🚧

### Objective
Develop a **custom HTTP server** in C++, compliant with the HTTP/1.1 specification, capable of handling multiple clients and requests concurrently.

### Technologies & Concepts
- TCP sockets and non-blocking I/O
- HTTP request parsing and response generation
- Configuration file parsing (nginx-like)
- CGI execution
- Error handling and status codes

### Current Focus
- Designing a robust server architecture
- Implementing request routing and method handling
- Managing multiple connections efficiently
- Ensuring compliance with HTTP standards

---

## 🏆 Personal Achievements

- **Major Achievement:** Completed **ft_irc** and **Inception** with bonus (125/100), demonstrating strong networking, system programming and containerisation skills
- **Technical Growth:** Expanding expertise in advanced C++, containerization, and infrastructure management
- **Next Steps:** Complete remaining Milestone 5 projects and continue refining code quality and system design

---

## 📂 Repository Structure

Each project is in its own directory, containing the full source code, as well as build and execution instructions (where applicable):
```
42-cursus-milestone-4/
│
├── cpp_modules_05_09/
│ ├── CPP05/
│ ├── CPP06/
│ ├── CPP07/
│ ├── CPP08/
│ └── CPP09/
├── Inception/
├── ft_irc/
└── webserv/
```

## 🚀 How to Explore

1. Clone this repository:
`git clone https://github.com/viceda-s/42-cursus-level-5.git`
2. Check the README in each project folder for details on progress, challenges, and explanations of problem-solving.

**Feel free to contact me if you have any questions, would like to collaborate, review code, or discuss any of the projects!**

<div style="text-align: center">⁂</div>
