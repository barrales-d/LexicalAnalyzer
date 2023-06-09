# Lexical Analyzer implemented in C

This Lexer parses C programs given a input source code(`input.c`) and outputs a stream of tokens in a table format in `output.txt`

## Quick Start
``` console
$ clang main.c -o main
$ ./main
```
* *Note*: Right now, the input and output files are hard coded into main

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
