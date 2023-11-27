## Pseudo-Assembly interpreter

## Table of Contents
1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
    - [Usage](#usage)
3. [Roadmap](#roadmap)
4. [License](#license)
## Introduction

Simple project recreationally made in C. Goal is to make interpreted and simplified assembly like language.
It is my starting point to get into the world of compilers and interpreters.

## Getting Started

### Prerequisites

- GTK's glib2.0 for hash tables—possible to replace with any hashtable lib
- any C compiler

### Installation

Install dotnet template using CLI or just follow MonoGame's tutorial:

```shell
gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -pthread -O3 -Wall -Wextra -march=native src/* main.c -o interpreter -lglib-2.0
```
## Usage

For any syntax explanation, refer to pdfs inside the Docs folder.
Currently only file interpreting is working.
Invoke application with file name as command line argument.

```shell
./interpreter testFile
```
File with name testFile contains example usage of the interpreter.
Program should find root of simple 1-grade polynomial.
Use BR instruction to inject breakpoint inside your code, it will display the whole state of the machine.

## Roadmap

Progress in this repository may be slower due to demanding university duties.
However, future plans include the incorporation of the following features:

- [x] Working interpreter
- [x] Dynamic memory allocation syntax
- [x] Debugging instruction added
- [ ] Actual translation to assembly to compare with real code
- [ ] Interactive code writing in shell
- [ ] Stacks and possibly functions
- [ ] Detailed documentation

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.
