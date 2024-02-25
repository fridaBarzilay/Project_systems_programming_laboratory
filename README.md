Systems-programming-laboratory-project
This project simulates how the assembler works

#Assembler This assembler program translates instructions in assembly language to binary machine code and then into unique base 2 code.

## General Info
Final project of the course 'Systems programming laboratory' in The Open University of Israel.

## Technologies
Project is created with ANSI C language on Ubuntu 16.04 operating system.

## Input files
Source file of instructions with extension ".as", for example ps.as.

## Output files
There are at most 4 output files: ".am" file of instructions after expanding macro statements. ".ob" file contains two columns table with unique base code. ".ext" optional file, created if only there are external labels defined in source file. Contains name of external label and the address of command sentence they are used in. ".ent" optional file, created if only there are entry labels defined in source file. Contains name of entry label and it's address.

## More details about running
Program can receive multiple ".as" files as an input, but will refer to each file separately. If there is error in line of instruction, program will print error type into terminal and continue execution. Binary machine code in this program is memory word that contains 14 bits (4 bits for opcode, 2 bits for source operand, 2 bits for destination operand, 2 bits for the type of word, 2 bits for parameter1 and 2 bits for parameter2). Each line can be represented by 1 to 4 memory words. ##Running steps## Preprocessor: Receives ".as" file and creates ".am" after expanding macro statements. : First step Read, save instructions and their parameters, separate by type: command or directive (create symbol tables). Check different errors like typo's, wrong command names, wrong parameters etc. Second step: Check label definitions and types. Handling possible errors in labels list (error for example: label is defined as entry and extern in the same source file). finish step: prints the output to the output files. ##files in the project## main.c - is the main function that gets the name of files in command line and sends to other functions macro.c - is the responsible of the macro expanding . first_trans.c - is the responsible of the function in the first transport. second_trans.c - is the responsible of the functions in the second transport. help_func.c - a file of help funcions that are used through all the project. encode.h - a main header of all the data structures. macro.h - header of macro.c. first_trans.h - header of first_trans.c. second_trans.h - header of second_trans.c. help_func.h - header of help_func.c. ##Example files## input.as export the files: input.am, input.ob, input.ext, input.ent tester.as export the files: tester.am, tester.ob, tester.ext, tester.ent

## Error examples
error examples are in the file called error, and the output of it is in the screen shot "running error".
