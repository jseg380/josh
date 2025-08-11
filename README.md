# jsh

**jsh** is a simple, modern, and educational Unix shell written in pure C. This project was developed as a deep dive into systems programming, memory management, and the core components of how a command-line interface interacts with an operating system.

It is built with a focus on clean, modular, and maintainable code, demonstrating key software architecture principles in a C environment.

---

## Features

- **Interactive REPL:** A stable Read-Eval-Print Loop for entering commands.
- **Built-in Commands:** Essential commands like `cd`, `pwd`, `help`, and `exit` are handled internally.
- **External Command Execution:** Launches any command from the system's `PATH` using the `fork`/`exec` model.
- **Modular Architecture:** Code is logically separated into modules for parsing, execution, built-ins, and process management.
- **Clean Error Handling:** Robust handling of user input and system call failures.
- **Signal Handling:** Gracefully handles `Ctrl+C` (`SIGINT`) to abort input without exiting the shell.

---

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc` or `clang`)
- `make` build utility

### Building

To build the `jsh` executable, simply clone the repository and run `make`:

```bash
git clone https://github.com/jseg380/jsh.git
cd jsh
make
```

### Running

To start the shell, run the compiled binary:

```bash
./jsh
```

---

## Project Structure

The codebase is organized into several modules to promote separation of concerns:

- `main.c`: The main entry point and Read-Eval-Print Loop (REPL) orchestrator.
- `command.h`: Defines the core data structures and types used throughout the shell (`ParsedInput`, etc.).
- `builtins.c/.h`: Encapsulates all logic for commands that are built directly into the shell.
- `process.c/.h`: Handles the creation and management of external child processes (`fork`, `exec`, `wait`).
- `path.c/.h`: A custom opaque type for handling and manipulating filesystem paths.
- `Makefile`: Provides simple build commands for the project.

---

## Development Philosophy

This project was built following a few key principles:

1.  **Modularity over Monolith:** Logic is separated into "black box" modules with clean public APIs, as opposed to a single, large source file.
2.  **Defensive Programming:** Always use braces, check return values from system calls, and handle potential error states explicitly.
3.  **Clarity and Readability:** Code is documented and structured to be understood, not just to work. The "Include What You Use" (IWYU) principle is followed for header files.

This project serves as a practical demonstration of C programming skills applied to a real-world systems problem.
