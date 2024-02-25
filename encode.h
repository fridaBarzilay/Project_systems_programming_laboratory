#ifndef LINKED_ENCODE_H
#define LINKED_ENCODE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ADDRESS_START 100
enum type_command{data,instruc};/*type of sentences*/
enum type_symbol{regulars,entrys,externs};/*type of the symbol*/
enum struct_type{regular,num_or_add,regs};/*type of word of encoding*/
int IC;
int DC;

/*a struct that represent a command by its name and its opcode*/
typedef struct{
		
		char* name;
		int code;		
		}func[];

/*a struct represents a defined symbol*/
typedef struct {
		char* name;
		int address;
		int type_command;
		int type_symbol;
		}symbol;
		
typedef struct symbol *ptr;

/*linked list of symbols*/	
typedef struct{
			symbol data;
			ptr next;		
		}symbol_node;
/*a global head to linked list symbol_node*/	
symbol_node *symbols;	

/*a struct to the regular words of instruction words*/
typedef struct{
		unsigned are:2;/*A/R/E*/
		unsigned dest:2;/*method of destination operand*/
		unsigned source:2;/*method of source operand**/
		unsigned opcode:4;/*opcode of command*/
		unsigned param1:2;
		unsigned param2:2;
		}type1;
/*a struct to the words represents a code of number or an address of label*/
typedef struct{
		unsigned are:2;/*A/R/E*/
		signed value:12;/*value of number or address of label*/
		}type2;
/*a struct to the words represents a code of registers in instructino words*/
typedef struct{
		unsigned are:2;/*A/R/E*/
		unsigned reg_dest:6;/*value of destination register*/
		unsigned reg_source:6;/*value of source register*/
		}type3;
/*union of the three optional types of instruction words*/
typedef union{
		type1 regular;
		type2 num_or_add;
		type3 regs;
		}instruct_word;
typedef struct node_inst_word *inst_ptr;
/*linked list of instruction words*/	
typedef struct {
			int type;
			instruct_word data;
			inst_ptr next;
		
		}node_inst_word;
/*global head to the linked list of node_inst_word*/		
node_inst_word *instruction_code;	
		
/*struct to a data word*/
typedef struct{
		unsigned value:14;/*value of data (char or number)*/
	}data_word;
	
typedef struct node_data_word *data_ptr;
/*linked list of data words*/
typedef struct  {
				data_word data;
				data_ptr next;
			}node_data_word;		
/*global head of linked list node_data_word*/		
node_data_word *data_code;

/*struct of details about labels that are in use in command sentences, for example: mov r3,MAIN
MAIN is a label to be saved in this struct label_details*/		
typedef struct{
		char *name;/*name of the label*/
		int num_of_node;/*the index of node in the linked list of instruction words*/
		int line;/*the line that the label apears*/
		}label_details;		
#endif
