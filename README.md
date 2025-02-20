# Minishell Project

## Overview

Minishell is a simple shell implementation written in C. The primary purpose of this project was to to learn the inner workings of a shell, including but not limited to: parsing input, managing file descriptors, piping, forking and executing commands and binaries.

Standard C practices apply, all memory is managed manually and file descriptors are closed once used. No leaks.

## Features

- **Command Execution**: Execute built-in and external commands.
- **Environment Variable Management**: Set, get, and unset environment variables.
- **Input/Output Redirection**: Support for redirecting input and output using `>`, `<`, and `>>`.
- **Piping**: Ability to pipe the output of one command to the input of another using `|`.
- **Custom Built-in Commands**: Includes basic built-in commands like `cd`, `env`, `pwd`, `export`, `unset` and `exit`.

## Requirements

To build and run Minishell, you will need:

- A C compiler
- Readline installed (brew install readline)

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/tinsights/minishell.git
   cd minishell
   ```

2. **Build the Project**:
   ```bash
   make
   ```

3. **Run the Shell**:
   ```bash
   ./minishell
   ```

## Usage

Once the shell is running, you can enter commands directly into the command line interface.

### Basic Commands

Here are some examples of commands you can use:

- **Change Directory**:
  ```bash
  cd /path/to/directory
  ```

- **List Files**:
  ```bash
  ls -l
  ```

- **Print Environment Variables**:
  ```bash
  env
  ```

- **Redirect Output**:
  ```bash
  echo "Hello World" > output.txt
  ```

- **Pipe Commands**:
  ```bash
  ls -l | grep ".c"
  ```

### Built-in Commands

Minishell supports several built-in commands:

- `cd [directory]`: Change the current directory.
- `exit`: Exit the shell.
- `env`: Display the current environment variables.

---
