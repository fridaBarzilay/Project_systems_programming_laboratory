#include "help_func.h"

/*this function gets a pointer to a file and a pointer string and puts in line the next line of the file to be read.*/
int get_next_line(FILE *file,char **line)
{
	int len;/*length of the next line*/
	memset(*line,'\0',LEN_LINE);
	if(fgets(*line, LEN_LINE, file) == NULL)
        	return 0;
	len=strlen(*line)-1;
	if((*line)[len]=='\n')
	{	
		(*line)[len]='\0';
	}
	else
		strcat(*line,"\0");
	return 1;
}
/*this function gets a string and remove all the spaces and tabs from it*/
void remove_spaces(char **string)
{
    	int i;
    	int non_space_count = 0;/*count the chars that are not space*/
    	for (i = 0; (*string)[i] != '\0'; i++)
    	{
        	if (!isspace((*string)[i]))
        	{
           	 	(*string)[non_space_count] = (*string)[i];/*put the non_space char in the place of the first space char*/
            		non_space_count++;
        	}    
    	}
    	(*string)[non_space_count] = '\0';
}

/*this function gets a pointer to a string and quit all the spaces that are in the begining of it.*/
int quit_spaces(char **str)
{
	int is_space=0;
	while(**str!='\0' && isspace(**str)!=0)/*while there are spaces in the beggining*/
	{
		is_space=1;
		(*str)++;
	}
	return is_space;
}

/*this function gets two pointers to strings and put in word the first word of str. this word is removed from str.
word is finished by space or a comma.*/
void get_word(char **str,char **word)
{
	int i=0;
	while(**str!='\0' && **str!=' ' && **str!='	' && **str!=',' && **str!='('  && **str!=')' && **str!='\n')
	{
		*word=(char*)realloc(*word,sizeof(char)*(strlen(*word)+1));
		(*word)[i]=**str;/*add current char to word*/
		(*str)++;/*remove current char from str*/
		i++;
	}
	*word=(char*)realloc(*word,sizeof(char)*(i+1));
	strcat(*word,"\0");	
}

/*this function gets a name of a symbol and bring the type of it- entry/extern/regular.*/
int get_type_symbol(char *name)
{
	symbol_node *temp=symbols;
	while(temp!=NULL)
        {
        	if(strcmp(name,temp->data.name)==0)
        		return temp->data.type_symbol;	
            	temp=(symbol_node*)temp->next;
        }
        return -1;
}

/*this function gets a name of symbol and return the node of it in the linked list of symbols- 'symbols'.*/
symbol_node * get_symbol_node(char *name)
{
	symbol_node *temp=symbols;
	while(temp!=NULL)
        {
        	if(strcmp(name,temp->data.name)==0)
        		return temp;
        	temp=(symbol_node*)(temp->next);	
        }
        return NULL;
}

/*this function gets a pointer to a node in the linked list 'instruction_code' and zero all the fields of it.*/
void zero_regular_word(node_inst_word **temp)
{
	(*temp)->data.regular.are=0;
	(*temp)->data.regular.dest=0;
	(*temp)->data.regular.source=0;
	(*temp)->data.regular.param1=0;
	(*temp)->data.regular.param2=0;
}

/*this function gets a string and return true if it has only white spaces and 0 if not.*/
int is_empty_line(char *str) 
{
    	while (*str!='\0') 
    	{
        	if (!isspace(*str)) /*if the current char is not a space*/
        	{
           	 	return 0;
        	}
        	str++;
    	}
    	
   	 return 1;/*all the line is spaces*/
}

/*this function prints all the details of the symbols in the tabel of symbols*/
void print_symbols()
{
	int i=0;
	symbol_node *temp=symbols;	
	while(temp!=NULL)
	{
		printf("the %d's element: the name is:%s , address is: %d , type_command is: %d , type_symbol is: %d\n", ++i,(temp->data).name,(temp->data).address,(temp->data).type_command,(temp->data).type_symbol); 
		temp=(symbol_node *)(temp->next);
	}
}
