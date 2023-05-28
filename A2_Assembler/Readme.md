# Assignment 2
> Building a 2-pass Assembler

```
Name - Anindya Vijayvargeeya
Roll - 200101015
```
---

## Assumptions
The description of the assembler and the assembly language that it works over is taken from the book `System Software, Leland L. Beck`.  

- Instructions' Opcodes are taken from the Appendix A of the book.
- Indexed addressing is indicated  by the modified `,X` following the operand.
- Lines beginning with `.` are comments only.  
- We have 6 _assembler directives_ : 
  - `START`, `END`, `BYTE`, `WORD`, `RESB`, `RESW`

---
## Working of the Assembler
- In the first pass the assembler builds the symbol table and saves the location counter value for each line, which it then uses to create the object code in the second pass.
- In the object code, 3 types of records are created - Header, Text, and End. Header contains the program name, starting address and lenght. Text records contain the translated instructions and data of the program. The End record marks the end and specifies the address in the program where execution is to begin.
- After the first pass, symbol table and intermediate code is written to files.

---
## Running the code

First compile with g++
```
g++ code.cpp
```
Run with input file name passed as command line arguments-
```
./a.out sample_input.txt
```
