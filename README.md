## Pseudo-Assembly interpreter

Simple project recreationally made in C. Goal is to make interpreted and simplified assembly like language. 
It is my starting point to get into world of compilers and interpreters.

## Compilation:

### Prerequisites

- GTK's glib2.0 for hash tables - possible to replace with any hashtable lib
- any C compiler

### Example compilation

```shell
gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -pthread -O3 -Wall -Wextra -march=native src/errors.c src/interpreter.c src/interpreterCLI.c src/lexer.c src/list.c src/stack.c src/token.c main.c -o interpreter -lglib-2.0
```
## Usage:

For any syntax explanation refer to pdfs inside Docs folder. Currently only file interpreting is working.
Just invoke application with file name as command line argument.

```shell
./interpreter testFile
```

Use BR instruction to inject breakpoint inside of your code. It will display whole state of the machine.

## Features to come:

- actual translation to assembly to compare with real code
- interactive code writing in shell

## License