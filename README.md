## Milestone 5 – In Progress 🟡

Milestone 5 focuses on advanced system programming, networking, containerization, and deeper C++ concepts. At this stage, **ft_irc** has been successfully completed with bonus, while **CPP Modules 05–09**, **Inception** and **webserv** are currently **work in progress**.

## 🏅 My Scores

| Project            | Status / Score | Notable Features |
| :--                |:--------------:| :--              |
| ft_irc             |   🌟 125/100   | IRC server, TCP sockets, multiplexing, real-time communication |
| CPP Modules 05–09  |  🚧 WIP (4/5)  | Advanced C++ (exceptions, templates, STL, containers) |
| Inception          |     🚧 WIP     | Docker, Docker Compose, containerized infrastructure |
| webserv            |     🚧 WIP     | HTTP server, sockets, configuration parsing |

---

## 💬 ft_irc – 125/100 🌟

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

## 🖥️ CPP Modules 05–09 – Work in Progress 🚧

### Objective
Deepen C++ expertise through advanced language features and standard library usage.

### Topics Covered
- **CPP05:** Exceptions and error handling ✅
- **CPP06:** C++ casts and type conversions ✅
- **CPP07:** Templates and generic programming ✅
- **CPP08:** STL containers, iterators, and algorithms ✅
- **CPP09:** Advanced STL usage and performance considerations

### Current Focus
- Writing safer and more expressive C++ code
- Leveraging STL for clean and efficient solutions
- Improving understanding of generic programming and type safety

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

## 🌐 webserv – Work in Progress 🚧

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

## 🏆 Milestone 5 Progress Summary

- **Major Achievement:** Completed **ft_irc** with bonus (125/100), demonstrating strong networking and system programming skills
- **Ongoing Work:** Actively developing **CPP Modules 05–09** and **Inception**
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
