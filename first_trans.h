#ifndef LINKED_FIRST_H
#define LINKED_FIRST_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "encode.h"
#include "help_func.h"
#include "checks.h"
#define MAX_FUNC 16
#define LEN_FUNC 3
enum argument_type{num,label,jump,reg};
enum operand_type{source,dest,alone};
int num_line;/*the index of line in the file*/

/*this function is the main function of the first transition. it sends to all the functions that encode the assembly file
@param am a pointer to the read file
@param labels a pointer to array of struct "label details".this array saves details about labels that are in use in command sentence: 
	the name, the number of node in 'instruction_code' and the number of line it apears.
@param len_labels a piointer to int that count the number of labels in the array labels. in the beggining it is 0
@return 1 if the content of the file am is legal and everything is done correctly, and 0 if not. */
int main_first_trans(FILE *am,label_details **labels,int *len_labels);

/*this function treats directive_sentences (.data/.string/.extern/.entry)
@param line the line from the read file
@param check line a pointer to the string line
@param name_of_label the name of the label defined in the directive sentence
@return 2 if the sentence in line is a directive sentence and the definition of sentence is legal and 0 if it is 
not legal and 1 if it is not a directive sentence*/
int data_sentence(char *line,char **check_line,char *name_of_label,int is_label,node_data_word **data_temp);

/*this function gets an operand, a pointer to a node_inst_word and the type of operand and encode all the fields 
that are connected to this operand. if the encodeing finished successfully- return 1, otherwise- return 0.
if the argument is of addresing method 2 (jump) 2 will be returned.
@param arg the argument to encode.
@param temp a pointer to a node in linked list 'instruction_code'.
@param oper_type the type of the operand: source or destination.
@param check_line the rest of the line after the name of operand.
@param is_operand if we encode argument in jump addressing method - 1, if not - 0.
@param is_register a pointer to int that is 0 in the beggining. if we see that the argument is register- would become 1.
@param labels a pointer to array of struct "label details". if the argument is label- enter its details to the array (name, number of node in list and number of line.
@param len_labels a pointer to int represents the length of array labels.
@return 1 if the encodeing finished successfully, otherwise- return 0.
if the argument is of addresing method 2 (jump)- return 2.
*/
int get_arg_type(char *arg,node_inst_word **temp,int oper_type,char *check_line,int is_operand,int *is_register,label_details **labels,int *len_labels);

/*this function gets a string of data and a pointer to a node in the linked list represents the encoding of data and 
encode the string into the linked list.
@param data a string include the string of chars to encode.
@param temp a pointer to the linked list 'data_code'.*/
void encode_data_string(char *data,node_data_word **temp);

/*this function gets a pointer to a string of numbers (data) and a pointer to a pointer of a linked list of the
 encodings of data sentences and encodes the numbers to the linked list
 @param data a string include the string of numbers to encode
 @param temp a pointer to the linked list 'data_code'.*/
void encode_data_data(char *data,node_data_word **data_temp);

/*this function encode the opcode of command.
@param command name the name of the command to encode.
@param temp a pointer to the linked list 'instruction_code'.*/
int encode_opcode(char *command_line, node_inst_word **temp);

/*this function gets a name of label, the type of command of the label and the type of the label, and enter this
label to the linked list of symbols.
@param symbol_name the name of the symbol.
@param type_comma the type of the sentence the symbol is defined in-instruction or data.
@param type_symb the type of the symbol- entry, extern or regular.
@return 1 the name of symbol is legal and 0 if not. */
int tabel_symbol(char *symbo_name, int type_comma, int type_symb);

/*this function gets a pointer to a node_data_word node and add to this linked list another node.
@param temp a pointer to the linked list 'data_code'*/
void add_data_node(node_data_word **temp);

/*this function gets a pointer to a node_inst_word node and add to this linked list another node.
@param temp a pointer to the linked list 'instruction_code'.
@param type the type of the new instruction word- regular, number/address or register.*/
void add_inst_node(node_inst_word **temp,int type);

/*this function set the pointer to node_data_word- temp on the last node of the global variable 'data_code'
@param temp a pointer to the linked list 'data_code'*/
void set_on_last_data_node(node_data_word **temp);

/*this function set the pointer to node_inst_word- temp on the last node of the global variable 'instruction_code'
@param temp a pointer to the linked list 'instruction_code'*/
void set_on_last_inst_node(node_inst_word **temp);

/*this function gets a pointer to node_inst_word and sets this pointer to point on the previous node.
@param temp a pointer to a instruction word*/
void get_prev_inst_word(node_inst_word **temp);

/*this function gets a name of symbol and return 1 if this symbol is exist in the tabel of symbols and 0 if not.
@param symbol_name the name of the symbol to find.
@return the number of times that this symbol exist in the list 'symbols'*/
int find_symbol(char *symbol_name);

/*this function adds to every symbol that appears in data sentences the last IC*/
void update_symbol_add();
#endif
