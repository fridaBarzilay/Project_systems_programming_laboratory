#ifndef LINKED_CHECKS_H
#define LINKED_CHECKS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "macro.h"
#include "first_trans.h"
#include "encode.h"
#define MAX_VAL 8191
#define MIN_VAL -8191
/**
this function makes a general checking of a command sentence
@param num_opcode the number of function
@param check_line a pointer to the command sentence
@return 1 if everything is legal and 0 if not 
*/
int general_check(int num_opcode,char *check_line);
/**
the function gets a string str and checks if the first char is a space as required.
@param str a string
@return 1 if there is a space in the beggining of str and 0 if not
*/
int proper_space(const char *str);
/**
The function accepts directives of extern and entry
and checks if a label exists and if it is set correctly
@param line_command - continuation of the received line after the word entry or extern.
@return 1 if the defenition is legal and 0 if not
*/
int check_entry_or_extern(char *line_command);
/**
The function receives the continuation of the line from ".string" and checks whether the content is correct
@param line_command - continuation of the received line after the word string.
@return 1 if the string line_command is legal and 0 if not.
*/
int check_string_islegal(char line_command[]);
/*
The function receives the continuation of the line from ".data" and performs normality checks on it
@param line_command - continuation of the received line after the word data.
@return 1 if the data recived in str is legal and 0 if not
*/
int valid_data(char* str);
/*this function check wether a name of symbol is of chars and numbers only or not
@param name the name of symbols
@return 1 if the name of symbols is of chars and numbers only, and 0 if not*/
int check_name_of_label(char *name);/*check wether a name of symbol is of chars and numbers only or not*/
/*
The method receives the command line with its instruction name receiving no operands at all.
@param line_command - continuation of the received line.
*/
int check_operand_zero(char *command_line);
/*
The method receives the command line with its instruction name receives one operand.
@param operand -Receiving the parameter to check if it is correct.
@param cmd_index-The index number of the operation name.
*/
int check_operand_one(char* operand_one, int cmd_index);
/*
The method receives the command line with its instruction name receives two operands.
@param operand_one -Receiving the first parameter to check if it is correct.
@param operand_two -Receiving the second parameter to check if it is correct.
@param cmd_index-The index number of the operation name.
*/
int check_operand_two(char* operand_one, char *operand_two, int cmd_index);
/**
A method that checks whether the sent operand is a zero-address method.
@param operand -Receiving the parameter to check if it is correct.
*/
int addressingMethodZero(char * operand);
/**
A method that checks whether the sent operand is a one-address method.
@param operand -Receiving the parameter to check if it is correct.
*/
int addressingMethodOne(char * operand);
/**
A method that checks whether the sent operand is a two-address method.
@param operand -Receiving the parameter to check if it is correct.
*/
int addressingMethodTwo(char * operand);
/**
A method that checks whether the sent operand is a three-address method.
@param operand -Receiving the parameter to check if it is correct.
*/
int addressingMethodThree(char * operand);

/*this function gets a pointer to the file .am and go over it searching .entry labels. when finding, the function searches
in the list of symbols if this label was defined as regular.
@param am a pointer to the file.
@return 1 if all the .entry labels was defined also as regulars in the file and 0 if not.*/
int check_entry_defined(FILE *am);
#endif
