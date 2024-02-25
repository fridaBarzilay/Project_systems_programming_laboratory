#ifndef LINKED_HELP_H
#define LINKED_HELP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macro.h"
#include "encode.h"

/*this function gets a pointer to a file and a pointer string and puts in line the next line of the file to be read
@param file a pointer to the read file
@param line a pointer to a string the gets the next line from file.*/
int get_next_line(FILE *as,char **line);

/*this function gets a string and remove all the spaces and tabs from it
@param string a pointer to a string that all its spaces would be removed*/
void remove_spaces(char **string);

/*this function gets a pointer to a string and quit all the spaces that are in the begining of it
@param str a pointer to a string.
@return 1 if there is a space and 0 if not.*/
int quit_spaces(char **str);

/*this function gets two pointers to strings and put in word the first word of str. this word is removed from str.
word is finished by space or a comma.
@param str the string to read the word from.
@param word the string to get the read word */
void get_word(char **str,char **word);

/*this function gets a name of a symbol and bring the type of it- entry/extern/regular.
@param name the name of the symbol
@return the type of the symbol: 0- regular, 1-entry, 2-extern.*/
int get_type_symbol(char *name);

/*this function gets a name of symbol and return the node of it in the linked list of symbols- 'symbols'.
@param name the name of the symbol.
@return a pointer to the node of this symbol in the linked list 'symbols'*/
symbol_node * get_symbol_node(char *name);

/*this function gets a pointer to a node in the linked list 'instruction_code' and zero all the fields of it.
@param temp a pointer to a node in the linked list 'instruction_code' that need to be zero*/
void zero_regular_word(node_inst_word **temp);

/*this function gets a string and return true if it has only white spaces and 0 if not
@param str a string
@return 1 if str has only white spaces and 0 if not*/
int is_empty_line(char *str);

/*this function prints all the details of the symbols in the tabel of symbols*/
void print_symbols();
#endif
