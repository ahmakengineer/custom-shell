🐚 Custom UNIX Shell — Systems Programming in C

A UNIX-like command shell implemented in C, designed to explore Linux internals, POSIX system calls, and process orchestration.
The project emphasizes correctness, performance, and modular design, reflecting real-world systems engineering practices.

📌 Project Overview

Implemented a fully functional shell capable of executing built-in and external commands

Designed to model how modern shells interact with the Linux kernel

Focused on process lifecycle management, file descriptor control, and memory safety

This project demonstrates practical experience in low-level C programming and Linux systems engineering.

⚙️ Core Capabilities

Command parsing and execution

Built-in command handling

External program execution via fork, exec, and wait

Pipelines (|) for inter-process communication

Input / Output redirection (<, >)

Background execution (&)

Defensive error handling and resource cleanup

🧩 Technical Highlights

Direct use of POSIX APIs

Explicit file descriptor management

Careful handling of edge cases (invalid commands, failed syscalls)

Modular design enabling easy extension

Minimal dependencies, portable across UNIX-like systems

🏗 Design & Architecture

The shell follows a clear separation of concerns:

Lexer — Tokenizes raw user input

Parser — Converts tokens into executable command structures

Executor — Manages process creation, piping, redirection, and execution

Built-ins — Handles internal commands without spawning new processes

This structure mirrors production-grade system software design.

🛠 Build & Run
Requirements

Linux / UNIX environment

GCC or Clang

Build
gcc -Wall -Wextra -O2 -o custom-shell *.c

Run
./custom-shell

🧪 Sample Usage
ls -l
cat file.txt | grep "error"
echo "Hello World" > output.txt
sleep 5 &

📈 Engineering Focus

This project prioritizes:

System correctness over shortcuts

Performance-aware C programming

Explicit resource management

Readable, maintainable low-level code

Skills demonstrated are directly applicable to:

Systems / Platform Engineering

Embedded Linux

Backend Infrastructure

Performance-critical services

🔮 Planned Enhancements

Job control (fg, bg)

Signal handling (SIGINT, SIGCHLD)

Command history

Environment variable expansion

👤 Author

Mahammad Shahid Karaneshi

GitHub: https://github.com/ahmakengineer

LinkedIn: https://linkedin.com/in/mahammadsk
