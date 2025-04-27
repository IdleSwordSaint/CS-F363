# Toy Language Compiler and Interpreter

This is a compiler and interpreter for a toy language, implemented as part of the CS-F363 Compiler Construction course.

## Features

- Syntactic analysis using Flex and Bison
- Three-Address Code (TAC) generation
- Simulation of the program with output
- Semantic error checking
- Symbol table generation and printing

## Usage

1. Compile the project:
   ```
   make
   ```

2. Run a program:
   ```
   ./a.out your_program.txt
   ```

3. For debugging with trace information:
   ```
   ./a.out --debug your_program.txt
   ```

## Example Programs

Sample programs are provided in the test files:
- t1.txt - Basic variable assignment and if-else
- t2.txt - Loops and array examples
- tc2.txt - More examples with nested loops

## Semantic Error Checking

The interpreter checks for the following semantic errors:
- Undeclared variables
- Using a variable before initialization
- Array bounds checking
- Division by zero
- Invalid array access

## Symbol Table

After execution, the interpreter prints the symbol table showing:
- Variable names
- Types
- Values (if initialized)
- Array information
