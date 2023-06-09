# Lexical Analyzer implemented in C

This Lexer parses C programs given a input source code(`input.c`) and outputs a stream of tokens in a table format in `output.txt`

## Quick Start
``` console
$ clang main.c -o main
$ ./main <input.c file> <output.txt file>
```
* output.txt is an **optional parameter**, if NOT provided the program will use `stdout` and print the table to the terminal!

### Input Source Code: `input.c`
    while (t < lower) r = 28.00;

### Output Table: `output.txt`
    +------------+---------------------+
    |   Token    |       Lexeme        |
    +------------+---------------------+
    |  KEYWORD   |       while         |
    +------------+---------------------+
    |  SEPERATOR |       (             |
    +------------+---------------------+
    |  SYMBOL    |       t             |
    +------------+---------------------+
    |  OPERATOR  |       <             |
    +------------+---------------------+
    |  SYMBOL    |       lower         |
    +------------+---------------------+
    |  SEPERATOR |       )             |
    +------------+---------------------+
    |  SYMBOL    |       r             |
    +------------+---------------------+
    |  OPERATOR  |       =             |
    +------------+---------------------+
    |  NUMBER    |       28.00         |
    +------------+---------------------+
    |  SEPERATOR |       ;             |
    +------------+---------------------+
