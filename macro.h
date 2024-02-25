#ifndef LINKED_MACRO_H
#define LINKED_MACRO_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "help_func.h"
#define END_AS ".as\0"
#define END_AM ".am\0"
#define LEN_LINE 81	

/*a linked list of the macros*/
typedef struct {
		char * name;/*name of macro*/
		char *lines;/*content of macro*/
	}field;

typedef  struct node* pointer;

typedef struct{
		field data;
		pointer next;
	}node;

/*this function gets two names of files, one to read and one to write to. the function reads from the file as and
switch the macros in the read file by writing their content to the write file(other sentences in file as that are
not macros are also copied write file).
@param name_of_file_as the name of the original file.
@param name_of_file_am the name of the file to write to.
@return 1 if the switching finished successfully and 0 if there are errors in the file or in the process of switching.*/
int switch_macro(FILE *as , char * name_of_file_am);

/*this function gets a pointer to the head of the linked list of macros, a macro name and a pointer to a string 
represents the content of the macro. the function searches for the macro in the list of macros and when finding it- 
copies the content of this macro to 'lines'.
@param head a pointer to the first node of the linked list of macros.
@param name the name of the macro.
@param lines a pointer to a string that would get the content of macro 'name' from the linked list of macros.
@return the content of the macro */
char* find_mcr(node *head,char *name);

/*this function counts the number of chars in line.
@param as the source file.
@return the number of chars in line*/
int check_num_of_chars(FILE **as);

/*this function gets a pointer to the head of the linked list of macros and a field f (data of node of macro)
and add the macro of the data f to the list of macros.
@param head a pointer to the first node of the linked list of macros.
@param f a field of data include data about the macro that would be added to the linked list of macros*/
void add_mcr(node ** head, field f );


/*this function checks if the name of macro is a saved word or not.
@param name the name of the macro.
@return 1 if the name of the macro is legal - it is not a saved word, and 0 if it is, and its not legal.*/
int check_macro_name(char *name);


/*this function frees the linked list of macros.
@param head a pointer to the first node of the linked list of macros.*/
void free_macro(node *head);
#endif
