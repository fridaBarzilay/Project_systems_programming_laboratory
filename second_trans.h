#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "encode.h"
#include "help_func.h"
#include "checks.h"
#define MAX_BIT 16384
#define END_OB ".ob\0"
#define END_ENT ".ent\0"
#define END_EXT ".ext\0"


/*this function is the main function of the second transportation.
it encodes the address of labels in the instruction words (that weren't enoded in the first transportation.
@param labels a pointer to array of struct "label details".this array saves details about labels that are in use in command sentence: 
the name, the number of node in 'instruction_code' and the number of line it apears.
@param len_labels the length of the array labels.
@return 1 if there is no errors and 0 if there are */
int main_second_trans(label_details *labels,int len_labels,FILE *am);


/*This function takes an integer and a number of bits and converts the integer into a binary string representation of the specified number of bits.
@param n An integer value to convert to binary representation.
@param numBits The number of bits for the binary representation.
@return A pointer to the binary string representation of the integer with the specified number of bits.*/
char* binaryScriptConverter(char* binaryString);


/*This function takes a binary string as input and converts it into a new string
where each '0' is replaced with a '.' and each '1' is replaced with a '/'.
The resulting string is returned as a char*.
@param binaryString The binary string to be converted.
@return The converted string as a char* or NULL if the input string contains*/
char* intToBinaryScript(int n,int numBits);


/*prints the encodings that are in the linked list of instruction words and data words.
@param name_file the name of .ob file to open and print to.*/
void print_output(char *name_file);

/*this function generates two files, one containing the external labels and their addresses and the other containing the entry labels and their addresses.
If no external or entry labels were found, the respective file is deleted.
@param name_file_ext: name of the file to write the external labels and addresses.
@param name_file_ent: name of the file to write the entry labels and addresses.
@param labels: an array of label_details structs containing the label name and its corresponding node number.
@param len: the length of the labels array.*/
void print_extern_entry(char *name_file_ext,char* name_of_file_ent, label_details *labels,int len);


/*This function is responsible for freeing the dynamically allocated memory used to store
the instruction code linked list. It does this by traversing the list and freeing each
node one at a time until the entire list has been freed.*/
void free_instruction_code();


/*This function frees the memory allocated for the linked list of node_data_word structs pointed to by data_code.
It does so by traversing the linked list using a while loop and freeing each node one by one.*/
void free_data_code();


/*This function frees the memory allocated for the linked list of symbol_node structs pointed to by symbols.
It does so by traversing the linked list using a while loop and freeing each node one by one.
Additionally, it also frees the memory allocated for the name of each symbol stored in the symbol_node structs.*/
void free_symbols();


/*This function frees the memory allocated to the array of label_details structure pointed by labels. It also frees the memory allocated to the name member of each element of the array.
@param labels: A pointer to an array of label_details structure.
@param len_labels: The length of the array of label_details structure.*/
void free_labels(label_details *labels,int len_labels);


/*this fuction gets a name of symbol and return its address from the list 'symbols'.
@param name the name of the symbol.
@return the address of the symbol.*/
int get_symbol_add(char *name);


/*This function prints the name and the node number of each element in the array of label_details structure.
@param labels: A pointer to an array of label_details structure.
@param len: The length of the array of label_details structure.*/
void printf_labels(label_details *labels,int len);

