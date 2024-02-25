#include "second_trans.h"
/*encodes the address of labels in the instruction words.*/
int main_second_trans(label_details *labels,int len_labels,FILE *am)
{
	node_inst_word *temp=instruction_code;/* a pointer to the current node in the linked list of instructions.*/
	int i=0,j=1,len,address,type_symbol,is_legal=1;
	if(check_entry_defined(am)==0)/*Check if there are any undefined entry symbols in the input file.*/
		is_legal=0;
	while(i<len_labels)/* Loop over the labels array.*/
	{
		len=labels[i].num_of_node;
		while(temp!=NULL && j<len)/*Iterate over the linked list of instructions until we reach the end of the list or the node number that corresponds to the current label.*/
		{
			temp=(node_inst_word*)(temp->next);
			j++;
		}
		address=get_symbol_add(labels[i].name);
		if(address==-1)
		{
			fprintf(stderr,"in line %d : undefined symbol: %s\n",labels[i].line,labels[i].name);
			is_legal=0;
		}
		temp->data.num_or_add.value=address;
		type_symbol=get_type_symbol(labels[i].name);
		if(type_symbol==-1 && is_legal)
		{
			fprintf(stderr,"in line %d : undefined symbol: %s\n",labels[i].line,labels[i].name);
			is_legal=0;
		}
		switch(type_symbol)/*Set the appropriate value of the num_or_add.are field in the instruction based on the type of symbol.*/
		{
			case (regulars):/* If the symbol is a regular symbol, set the are field to 2.*/
				temp->data.num_or_add.are=2;
				break;
			case (entrys):/*If the symbol is an entry symbol, set the are field to 2.*/
				temp->data.num_or_add.are=2;
				break;
			case externs:/*If the symbol is an externs symbol, set the are field to 1.*/
				temp->data.num_or_add.are=1;
				break;
		}
		i++;	
	}
	return is_legal;
}

/*gets a name of symbol and return its address from the list 'symbols'.*/
int get_symbol_add(char *name)
{
	symbol_node *temp=symbols;
	while(temp!=NULL)/*While the temp pointer is not NULL*/ 
        {
        	if(strcmp(name,temp->data.name)==0)/*If the symbol's name matches the name passed as a parameter, return the symbol's address.*/
        		return temp->data.address;	
            	temp=(symbol_node*)temp->next;
        }
        return -1;
}

/*The function takes a string argument representing the name of the file to write to, and it writes the instruction and data code to the file in a specific format.*/
void print_output(char *name_file)
{
	int i=0;
	char *namef;/*A pointer to a string representing a binary representation of a word of data.*/
	FILE *ifp;/*A pointer to a FILE struct representing the file to write to.*/
	node_inst_word *temp_inst = instruction_code;/*A pointer to a node_inst_word struct representing the current instruction code being processed.*/
	node_data_word *temp_data = data_code;/*A pointer to a node_data_word struct representing the current data code being processed.*/
	if(name_file==NULL)
	{
		fprintf(stderr,"Allocation failed\n");
		exit(0);
	}
	if(!(ifp = fopen(name_file,"w+")))
	{
		fprintf(stderr,"Error: cannot open the file %s\n",name_file);
		exit(EXIT_FAILURE);
	}
    	fseek(ifp,0,SEEK_SET);
	fprintf(ifp,"\t%d  %d\n",IC,DC);
	for(i=0; i<IC;i++)/*Iterate over the instruction_code linked list*/
	{
		fprintf(ifp,"0%d\t", i+ADDRESS_START);
		if(temp_inst->type==regular)/*If the instruction type is regular, convert the opcode, source and destination registers, and parameters to binary script format and write to the file.*/
		{
			namef=intToBinaryScript(temp_inst->data.regular.param1,2);
			fprintf(ifp, "%s", namef);
			namef=intToBinaryScript(temp_inst->data.regular.param2,2);
			fprintf(ifp, "%s", namef);
			namef=intToBinaryScript(temp_inst->data.regular.opcode,4);
			fprintf(ifp, "%s", namef);
			namef=intToBinaryScript(temp_inst->data.regular.source,2);
			fprintf(ifp, "%s", namef);
			namef=intToBinaryScript(temp_inst->data.regular.dest,2);
			fprintf(ifp, "%s", namef);
			namef=intToBinaryScript(temp_inst->data.regular.are,2);
			fprintf(ifp, "%s\n", namef);
		}
		else if(temp_inst->type==num_or_add)/*If the instruction type is num_or_add, convert the value and ARE to binary script format and write to the file.*/
		{
			namef=intToBinaryScript(temp_inst->data.num_or_add.value,12);
			fprintf(ifp, "%s", namef);
			namef=intToBinaryScript(temp_inst->data.num_or_add.are,2);
			fprintf(ifp, "%s\n", namef);
		}
		else/*If the instruction type is regs, convert the source and destination registers and ARE to binary script format and write to the file.*/
		{	
			namef=intToBinaryScript(temp_inst->data.regs.reg_source,6);
			fprintf(ifp, "%s", namef);
			namef=intToBinaryScript(temp_inst->data.regs.reg_dest,6);
			fprintf(ifp, "%s", namef);
			namef=intToBinaryScript(temp_inst->data.regs.are,2);
			fprintf(ifp, "%s\n", namef);
		}
		temp_inst=(node_inst_word*)(temp_inst->next);
	}
	for(i=IC; i<DC+IC;i++)/*Iterate over the data_code linked list.*/
	{
		fprintf(ifp,"0%d\t", i+ADDRESS_START);
		namef=intToBinaryScript(temp_data->data.value,14);
		fprintf(ifp, "%s\n", namef);
		temp_data = (node_data_word*)(temp_data->next);
	}
	fclose(ifp);
}
/*return the binary number of n as a string in the length of bit*/
char* intToBinaryScript(int n,int numBits)
{
	int i;
	char* binaryString;
  	binaryString=(char*)malloc((numBits+1)*sizeof(char));/*Allocate memory for the binary string representation*/
  	for (i = numBits-1; i >= 0; i--)/*Iterate over each bit of the integer and assign '1' or '0' to the corresponding bit in the binary string representation*/
   	{
       		binaryString[i] = (n & 1) ? '1' : '0';
        	n >>= 1;
    	}
    	binaryString[numBits] = '\0';
	return binaryScriptConverter(binaryString);
}

/*put instead of every / in binaryString s- 1 and instead every * - 0*/
char* binaryScriptConverter(char* binaryString) 
{
	int i,len = strlen(binaryString);
	char* newString = (char*)malloc(len+1);

	for (i = 0; i < len; i++)/*Loop through each character in the input string.*/	
	{
		if (binaryString[i] == '0') /*If the character is '0', replace it with a '.' in the new string.*/
		{
		   	newString[i] = '.';
		} 
		else if (binaryString[i] == '1') /* If the character is '1', replace it with a '/' in the new string.*/
		{
			newString[i] = '/';
		}
		else 
		{
			free(newString);
			return NULL;
		}
	}
	newString[len] = '\0';
	return newString;
}

/*this function prints the details of extern and entry symbols to the files ps.ext and ps.ent*/
void print_extern_entry(char *name_file_ext,char* name_file_ent, label_details *labels,int len)
{
	symbol_node *temp=symbols;/* A pointer to a symbol_node struct representing the symbol table*/
	int i,is_ent=0,is_ext=0;
	FILE *ext,*ent;
	if(!(ext = fopen(name_file_ext,"w+")))
	{
		fprintf(stderr,"Error: cannot open the file %s\n",name_file_ext);
		exit(EXIT_FAILURE);
	}
    	fseek(ext,0,SEEK_SET);
    	
    	if(!(ent = fopen(name_file_ent,"w+")))
	{
		fprintf(stderr,"Error: cannot open the file %s\n",name_file_ent);
		exit(EXIT_FAILURE);
	}
    	fseek(ent,0,SEEK_SET);
	while(temp!=NULL)/*Loop through the symbol table*/
	{
		if(temp->data.type_symbol==entrys)/*If the symbol is an entry symbol, print it to the entry file and set is_ent flag*/
		{
			fprintf(ent,"%s		%d\n",temp->data.name,temp->data.address);
			is_ent=1;
		}
		temp=(symbol_node*)(temp->next);		
	}
	
	for(i=0;i<len;i++)/*Loop through the labels array*/
	{
		if(get_type_symbol(labels[i].name)==externs)/*If the label is an external label, print it to the external file and set is_ext flag*/
		{	
			fprintf(ext,"%s		%d\n",labels[i].name,labels[i].num_of_node+ADDRESS_START-1);
			is_ext=1;
		}
	}
	if(!is_ext)/*If there are no external symbols, delete the external file*/
	{
		remove(name_file_ext);
	}
	if(!is_ent)/*If there are no entry symbols, delete the entry file*/
	{
		remove(name_file_ent);		
	}
	fclose(ext);
	fclose(ent);
}

/*this function frees the linked list of instruction words code*/
void free_instruction_code()
{
	node_inst_word *temp;
	while (instruction_code != NULL)/*Traverse the linked list and free each node one at a time*/
    	{
       		temp=instruction_code;
       		instruction_code=(node_inst_word*)instruction_code->next;
		free(temp);
	}
}

/*this function frees the linked list of data words code*/
void free_data_code()
{
	node_data_word *temp;/*Declare a temporary node pointer to hold the current node being freed*/
	while (data_code != NULL)/*Loop through the linked list until the end is reached (i.e. data_code is NULL)*/
    	{
       		temp=data_code;/*Assign temp to the current node being freed*/
       		data_code=(node_data_word*)data_code->next;/*Move data_code to the next node in the linked list*/
		free(temp);
	}
}

/*this functino frees the linked list of symbols.*/
void free_symbols()
{
	symbol_node *temp;/*Declare a temporary node pointer to hold the current node being freed*/
	while (symbols != NULL)/*Loop through the linked list until the end is reached (i.e. symbols is NULL)*/
    	{
       		temp=symbols;
       		symbols=(symbol_node*)symbols->next;
       		free(temp->data.name);
		free(temp);
	}
}
/*This function frees the memory allocated to the array of label_details structure pointed by labels. It also frees the memory allocated to the name member of each element of the array.*/
void free_labels(label_details *labels,int len_labels)
{
	int i;
	for(i=0;i<len_labels;i++)/* Loop through each element of the array and free the memory allocated to its name member.*/
    	{
       		free(labels[i].name);
	}
	free(labels);
}
/*This function prints the name and the node number of each element in the array of label_details structure.*/
void printf_labels(label_details *labels,int len)
{
	int i=0;
	for(i=0;i<len;i++)/*Loop through each element of the array and print its name and node number.*/
	{
		printf("name: %s  node: %d\n",labels[i].name,labels[i].num_of_node);
	}
}
