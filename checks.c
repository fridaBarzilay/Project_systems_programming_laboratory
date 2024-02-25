#include "checks.h"
int general_check(int num_opcode,char *check_line)/*This function is responsible for validating the operands of an instruction, based on the number of operands it has and the type of the instruction. It receives the opcode of the instruction and the line to be checked as input.*/
{
	char *param1, *param2, *copy;/*Pointer variables for string manipulation */
	int is_legal=1;
	if(num_opcode==0||num_opcode==1||num_opcode==2||num_opcode==3||num_opcode==6)/* Opcode types that have two operands*/
	{
		quit_spaces(&check_line);
		copy = (char*)malloc(strlen(check_line)+1);/*Allocate memory for a copy of the check_line.*/
		if(copy == NULL)
		{
			fprintf(stderr,"Error: allocation failed");
			exit(0);
		}
		strcpy(copy, check_line);
		if((param1 = strtok(copy,",")) == NULL) /* Get first operand */
		{
			fprintf(stderr,"Error in line %d: missing argument\n", num_line);
			return 0;
		}
				
		param1[strlen(param1)] = '\0';
		remove_spaces(&param1);/*Remove any spaces from param1.*/
		if((param2 = strtok(NULL,"\t")) == NULL) /* Get second operand */

		{	
			fprintf(stderr,"Error in line %d: missing argument\n", num_line);
			return 0;
		}
		param2[strlen(param2)] = '\0';
		remove_spaces(&param2);/*emove any spaces from param2.*/
		if(check_operand_two(param1,param2,num_opcode)==-1)/*Call the check_operand_two function to validate the two operands. If the function returns -1, set is_legal to 0.*/
			is_legal=0;
		free(copy);/*Free the memory allocated for copy.
*/
						}
	else if(num_opcode==4||num_opcode==5||num_opcode==7||num_opcode==8||num_opcode==9||num_opcode==10||num_opcode==11||num_opcode==12||num_opcode==13)/* Opcode types that have one operand*/

	{
		quit_spaces(&check_line);
		copy = (char*)malloc(strlen(check_line)+1);/*Allocate memory for a copy of the check_line.
*/
		if(copy == NULL)
		{
			fprintf(stderr,"Error: allocation failed\n");
			exit(0);
		}
		strcpy(copy, check_line);
		if((param1 = strtok(copy,"\0")) == NULL)/*Get the operand using strtok and copy it to param1.
*/
		{
			fprintf(stderr,"Error in line %d: missing argument\n", num_line);
			free(copy);
			return 0;
		}
		param1[strlen(param1)] = '\0';
		if(check_operand_one(param1,num_opcode)==-1)/*Call the check_operand_one function to validate the operand. If the function returns -1, set is_legal to 0.*/
			is_legal=0;
		free(copy);/*Free the memory allocated for copy.
*/
	}
	else if(num_opcode==14||num_opcode==15)/* Opcode types that have zero operands*/
	{	
		copy = (char*)malloc(strlen(check_line)+1);/*llocate memory for a copy of the check_line.*/
		if(copy == NULL)
		{
			fprintf(stderr,"Error: allocation failed\n");
			exit(0);
		}
		strcpy(copy, check_line);
		if(check_operand_zero(copy)==-1)/*Call the check_operand_zero function to validate the operand. If the function returns -1, set is_legal to 0.
*/
			is_legal=0;
		free(copy);
	}
       	return is_legal;
}
/*This function is responsible for checking whether there is a space after .extern or .entry or label in the line. It receives a string as input and checks if the first character is a space or a tab.*/
int proper_space(const char *str)
{
	if(*str!=' ' && *str!='	')
        {
	    	return 0;
	}
	return 1;	
}
/*The function checks if there is a space after the directive, if the label name is less than 30 characters, if the label name starts with a capital letter, and if there are no spaces in the label name.*/
int check_entry_or_extern(char *line_command)
{
	char *str= line_command;
	int len;
	if(!proper_space(str))/*The function then removes any extra spaces at the beginning of the label name*/
	{
		fprintf(stderr,"Error in line %d: no space between .extern or .entry to name of label\n",num_line);
		return 0;
	}
	quit_spaces(&str);
	len = strlen(str);
	if(len>30)
	{	
		fprintf(stderr,"Error in line %d: name of label is longer than 30 chars\n",num_line);
		return 0;
	}
	if(isalpha(*str)==0)
	{	
		fprintf(stderr,"Error in line %d: name of label doesn't start with a letter\n",num_line);
		return 0;	
	}	

	return 1;
}
/*This function is responsible for checking if the string is valid. It receives a string as input, which is the string to be checked.*/
int check_string_islegal(char line_command[])
{
	char *string= line_command;/*a pointer to a character array that is initialized with line_command and is used to manipulate the string during the validation process.*/
	int len;
	len = strlen(string);
	if (len < 2)/*If it is less than 2, the function prints an error message indicating that the string is missing quotation marks*/
	{
		fprintf(stderr,"Error in line %d: Missing quotation marks\n",num_line);
		return 0;
	}
	
	if(string[0] != '"' || string[len-1] != '"')/*Checking whether the character " " is present at the beginning and end of the string*/
	{
		fprintf(stderr,"Error in line %d: Missing quotation marks\n",num_line);
		return 0;
	}
	if(*string != '\0' || *string != EOF||*string !='\n')/*If the string passes both of the above checks, the function checks whether the first character of the string is not '\0', EOF, or '\n'.*/
	{
		return 1;
	}
	fprintf(stderr,"Error in line %d: extraneous text after end of command\n",num_line);
	return 0;
}
/*This function is responsible for validating a list of data values. It receives a string as input, which is the list of data values to be checked.*/
int valid_data(char* str)
{
    char* token;/*pointer to the current token being processed*/
    int num;/*integer representation of the token*/
    char* endptr;/*used by strtol to check for invalid tokens*/
    int i,len;
    char* copy = malloc(strlen(str) + 1);/* create a copy of the input string*/
    if (copy == NULL) {
        return 0;
    }
		while(isspace(*str))
			str++;
	if(*str==',')/*Check for an illegal comma at the beginning of the string*/
	{
		fprintf(stderr, "Error in line %d: illegal comma\n", num_line);
		return 0;

	}
	remove_spaces(&str);
	strcpy(copy, str);/*Make a copy of the input string and get its length*/
	len=strlen(copy);
    token = strtok(copy, ",");/*Split the copy into tokens separated by commas and validate each token*/

    while (token != NULL) /* check if token is empty*/
	{
        if (strlen(token) == 0) {
			fprintf(stderr, "Error in line %d: cannot allocate space\n", num_line);
            free(copy);
            return 0;
        }
        for (i= 0; i < strlen(token); i++) 
	{
            if (!isdigit(token[i]) && token[i] != '+' && token[i] != '-'&& isspace(*token))
	    {
			fprintf(stderr, "Error in line %d: invalid member - not an integer\n", num_line);
                free(copy);
                return 0;
            }
        }
		while(isspace(*token))
			token++;
        if (strchr(token, '+') || strchr(token, '-')) /*Convert the token to an integer, checking for invalid characters*/
	{
            num = strtol(token, &endptr, 10);/*convert token to integer using strtol*/
            if (*endptr != '\0') /*check if there are invalid characters in the token*/
		{
		fprintf(stderr, "Error in line %d: expiration in a number\n", num_line);
                free(copy);
                return 0;
		}
        } else {
            num = atoi(token);
        }
        if (num < MIN_VAL || num > MAX_VAL)/*Check that the integer value is within a specified range.*/ 
	{
	    fprintf(stderr, "Error in line %d: invalid member - value out of range\n", num_line);
            free(copy);
            return 0;
        }
        token = strtok(NULL, ",");/*Get the next token.*/
    }
	remove_spaces(&copy);/*Remove spaces from the copy of the input string and check for extraneous text after */
	if(!isdigit(copy[len-1]))
		{
			fprintf(stderr,"Error in line %d: extraneous text after end of command\n", num_line);
			free(copy);
			return 0;
		}
    	free(copy);

    return 1;
}
/*This function takes a string as input and checks whether it is a valid label name. */
int check_name_of_label(char *name)
{
	if(strlen(name)>30)/*Check if label name is longer than 30 characters*/
	{
		fprintf(stderr,"Error in line %d: name of label is longer than 30 chars\n",num_line);
		return 0;
	}
	if(!isalpha(*name))/*Check if label name starts with a letter*/
	{
		fprintf(stderr,"Error in line %d: name of label doesn't start with a letter\n",num_line);
		return 0;
	}
	while(*name!='\0')/*Check if label name only contains letters and numbers*/
	{
		if(isalpha(*name)==0 && isdigit(*name)==0)
		{
			fprintf(stderr,"Error in line %d: name of label is not of chars and numbers only\n",num_line);
			return 0;
		}
		name++;
	}
	return 1;
}
/*The function checks if the two operands we received are valid*/
int check_operand_two(char* operand_one, char *operand_two, int cmd_index)
{
	int operand1, operand2,add_method;
	if(cmd_index!=6) /*If the command index is not 6, check the addressing method of the first operand.*/
	{
		if((addressingMethodZero(operand_one) == -1))/*Checks if it is not a zero addressing method*/
		{
			fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
			return -1;
		}
		else if(addressingMethodZero(operand_one) == 1) /*Checks if it is a zero addressing method*/
		{
			operand1 = 0;
			++operand_one;
		}
		else if(addressingMethodThree(operand_one) == 1) /*Checks if it is a three addressing method*/
		{
			operand1 = 3;
			++operand_one;
		}			
	}

	if(operand1 != 0 && operand1 != 3 && addressingMethodOne(operand_one) == -1) /*If operand1 is not 0 or 3, check the addressing method of the first operand.*/
	{
		fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
		return -1;
	}
	else if(addressingMethodOne(operand_one) == 1) /*Checks if it is a one addressing method*/
		operand1 = 1;
	

	
	/*operand two check*/
	if(operand_two[strlen(operand_two)-1]==','||operand_two[0]==',')
	{
	fprintf(stderr, "Error in line %d: illegal comma\n", num_line);
	return 0;

	}

	if(cmd_index==1)/*Check the addressing method of the second operand for command index 1 (mov command)*/
	{
		if((add_method=addressingMethodZero(operand_two)) == -1)/*Checks if it is not a zero addressing method*/
		{
			fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
			return -1;
		}
		else if(add_method == 1) 
		{
			operand2 = 0;
			++operand_two;
		}
		else
			operand2 = 0;
	}
	
	if(addressingMethodThree(operand_two) == 1)/*Checks if it is a three addressing method*/
		operand2 = 3;
	else if(addressingMethodOne(operand_two) == -1)/*Checks if it is a not one addressing method*/ {
		fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
		return -1;
	}
	else if(addressingMethodOne(operand_two) == 1)/*Checks if it is a one addressing method*/
		operand2= 1;

	return operand1*10 + operand2;
}
/*The function checks if the one operands we received are valid*/
int check_operand_one(char* operand, int cmd_index)
{
    	char *str = (char*)malloc(strlen(operand) + 1);/* Allocate memory for a copy of the operand */
	int i;
	if(cmd_index==12)
	{
		if((addressingMethodZero(operand) == -1))/*Checks if it is a not zero addressing method*/
		{
			fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
			return -1;
		}
		else if(addressingMethodZero(operand) == 1) /*Checks if it is a zero addressing method*/
		{
			return 0;
		}
	}

	if(addressingMethodThree(operand) == 1) /*Checks if it is a three addressing method*/
		return 3;

	if(cmd_index==9 || cmd_index==10 || cmd_index==13)
	{
		if(!isalpha(*operand))/*If the first character is not a letter, print an error message and return -1*/
		{
			fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
			return -1;
		}
		for(i=0;isalpha(*operand) || isdigit(*operand);i++) /* Copy the label to a new string */
		{
			str[i]=*operand;
			operand++;
		}
		str[i]='\0';

		if(*operand != '(')/*Checking whether there are no jump addressing parameters*/
		{
			if(*operand == ',')/*Checking whether there is unnecessary text*/
			{
				fprintf(stderr, "Error in line %d: illegal comma\n", num_line);
				return 0;

			}

			if(addressingMethodThree(str) == 1)/*Checks if it is a three addressing method*/
			{
				free(str);
				i=3;
			}
			if(addressingMethodOne(str) == 1)/*Checks if it is a one addressing method*/
			{
				free(str);
				i=1;
			}
			if(i!=3&&i!=1)/* If the label has neither addressing mode one nor addressing mode three, print an error message and return -1 */
			{
				fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
				return -1;
			}

			while(isspace(*operand))
				operand++;
			if(*operand != '\0' || *operand != EOF||*operand !='\n')/*If there is no extraneous text after the end of the command */
			{
				if(i==0)
					return 0;
				else
					return 1;
			}
			else /* If there is extraneous text after the end of the command, print an error message and return -1 */
			{
				fprintf(stderr,"Error in line %d: extraneous text after end of command\n", num_line);
				return -1;
			}
		}
		else if(addressingMethodThree(str) == 1)/*Checks if it is a three addressing method*/
		{
			free(str);
			fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
			return -1;
			
		}
		else if(addressingMethodOne(str) == 1)/*Checks if it is a one addressing method*/
		{
			if(addressingMethodTwo(++operand) != -1)/*Checks if it is a two addressing method*/
			{
				return 2*100 + addressingMethodTwo(operand);
			}
			else 
			{
				return -1;
			}
		}
		else
		{
			fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
			return -1;
		}
 			
	}
	

	if(addressingMethodOne(operand) == -1)/*Checks if it is a one addressing method*/ {
		fprintf(stderr,"Error in line %d: operand is illegal\n", num_line);
		return -1;
	}
	else /*addressingMethodOne(operand) == 1*/
	{
		return 1;
	}
	
}
/*The function checks if the zero operands we received are valid*/
int check_operand_zero(char *command_line)
{
	remove_spaces(&command_line);
	while(isspace(*command_line))
		command_line++;
	if((*command_line=='\n')||(*command_line==EOF)||*command_line=='\0')/*check there is no extraneous text after the end of the command */
	{
		return 0;
		
	}
	fprintf(stderr,"Error in line %d: extraneous text after end of command\n", num_line);
	return -1;

}
/*The function checks whether the received string belongs to a zero-address method*/
int addressingMethodZero(char * operand)
{
	if(*operand=='#')/*Check if the operand starts with the '#' symbol*/
	{
		if(!(isdigit(*(++operand))||((*operand=='-' ||*operand=='+') && isdigit(*(++operand)))))/*If the first character is '#' then, check if the next character(s) are a valid constant value*/
			return -1;	
		return 1;
	}
		if(isdigit(*operand))/*If the operand does not start with the '#' symbol, then check if the operand is a valid label or symbol.*/
			return -1;
	return 2;
}
/*The function checks whether the received string belongs to a one-address method*/
int addressingMethodOne(char * operand)
{
	if(!isalpha(*operand))/*Check if the first character of the operand is alphabetic.*/
		return -1;
	while(isalpha(*(operand)) || isdigit(*operand))/*Check if the rest of the operand contains only alphabetic characters or digits.*/
	{
		operand++;
	}
	if(*operand=='\0' || *operand==EOF)/*Check if the end of the operand has been reached.*/
	{
		return 1;	
	}
	else 
		return -1;
}
/*The function checks whether the received string belongs to a two-address method*/
int addressingMethodTwo(char * operand)
{
	char *param1, *param2, *copy,*ptr;
	int param1_method, param2_method,j;
	copy = (char*)malloc(strlen(operand)+1);
	if(copy == NULL)
	{	
		fprintf(stderr,"Error: allocation failed\n");
		exit(0);
	}
	ptr=operand;
	remove_spaces(&ptr);
	if(ptr[strlen(ptr)-1]!=')')/*check if there is ( in the end of the command sentence*/
	{
		fprintf(stderr,"Error in line %d: missing closing parenthesis\n", num_line);
		free(copy);
		return -1;
	}
	if(ptr[0]==',')/*check if there is a first argument in the parenthesis*/
	{
		fprintf(stderr,"Error in line %d: missing argument\n", num_line);
		free(copy);
		return -1;
	}
	
	if(strlen(ptr)>1 && ptr[strlen(ptr)-2]==',')/*check if there is a secind operand in the parenthesis*/
	{
		fprintf(stderr,"Error in line %d: missing argument\n", num_line);
		free(copy);
		return -1;
	}
	strcpy(copy, operand);
        if(strcmp((param1 = strtok(copy,",")),operand) == 0)/*Extract param1 from copy*/
	{
        	fprintf(stderr,"Error in line %d: missing comma\n", num_line);
        	return -1;
    	}
	param1[strlen(param1)] = '\0';/*Add null character to end of param1*/
	for(j=0;j<strlen(param1);j++)/*Check for unnecessary whitespace in param1*/
		{
			if(isspace(param1[j]))
			{
        			fprintf(stderr,"Error in line %d: there is an illegal space\n", num_line);
				return -1;
			}
		}
/*Determine addressing mode of param1*/
	if(param1[strlen(param1)-1]==','||param1[0]==',')
	{
		fprintf(stderr, "Error in line %d: illegal comma\n", num_line);
		return 0;

	}
	if(addressingMethodZero(param1) == -1)/*Checks if it is not a zero addressing method*/
		return -1;
	else if(addressingMethodZero(param1) == 1) /*Checks if it is a zero addressing method*/
		param1_method = 0;
	else if(addressingMethodThree(param1) == 1) /*Checks if it is a three addressing method*/
		param1_method = 3;
	else if(addressingMethodOne(param1) == -1)/*Checks if it is not a one addressing method*/
		return -1;
	else 
		param1_method = 1;

	/*if((*/param2 = strtok(NULL,")");/*) == NULL)*//* Extract param2 from copy*/
	/*{
		fprintf(stderr,"Error in line %d: missing closing parenthesis\n", num_line);
		free(copy);
		return -1;
	}*/
	
	for(j=0;j<strlen(param2);j++)/*Check for unnecessary whitespace in param2*/
		{
			if(isspace(param2[j]))
			{
        			fprintf(stderr,"Error in line %d: there is an illegal space\n", num_line);
				return -1;
			}
		}

	/*Determine addressing mode of param2*/
	if(param2[strlen(param2)-1]==','||param2[0]==',')
	{
		fprintf(stderr, "Error in line %d: illegal comma\n", num_line);
		free(copy);
		return 0;

	}
	if(addressingMethodZero(param2) == -1)/*Checks if it is not a zero addressing method*/{
		free(copy);
		return -1;
	}
	else if(addressingMethodZero(param2) == 1) /*Checks if it is a zero addressing method*/{
		param2_method = 0;
		free(copy);
		return param1_method*10 + param2_method;
	}
	else if(addressingMethodThree(param2) == 1)/*Checks if it is a three addressing method*/ {
		param2_method = 3;
		free(copy);
		return param1_method*10 + param2_method;
	}
	if(addressingMethodOne(param2) == -1)/*Checks if it is not a one addressing method*/ {
		free(copy);
		return -1;
	}
	else {
		free(copy);
		param2_method = 1;
		return param1_method*10 + param2_method;
	}
}
/*The function checks whether the received string belongs to a three-address method*/
int addressingMethodThree(char * operand)
{
	if ((operand[0] == 'r') && (operand[1] >= '0' )&& (operand[1] <= '6' )&& (strlen(operand) == 2))/*Checking whether the first character is 'r' followed by a number between 0-9 and no more characters after that*/
	{
		return 1;
    	}
	else 
	{
        	return 0;
    	}
	return 0;
}

/*this function gets a pointer to the file .am and go over it searching .entry labels. when finding, the function searches
in the list of symbols if this label was defined as regular.*/
int check_entry_defined(FILE *am)
{
	char *line=(char*)malloc(sizeof(char)*LEN_LINE),*ptr,*name;/*Allocate memory for the "line" variable.*/
	int num_line=0,find=0;
	symbol_node *temp=symbols;
	num_line=0;
	while(get_next_line(am,&line)==1)/*Get the next line from the input file using "get_next_line" function.*/
	{
		num_line++;
		if((ptr=strstr(line,".entry"))!=NULL)/*If the line contains ".entry" directive, extract the label name and remove leading spaces.*/
		{
			name=(char*)calloc('\0',sizeof(char));
			ptr+=(strlen(".entry"));
			quit_spaces(&ptr);
			get_word(&ptr,&name);
			while(temp!=NULL)/*Search for the label name in the symbol table.*/
			{
				if(strcmp(temp->data.name,name)==0 && temp->data.type_symbol==externs)/*If the label is found and its type is "extern," return an error message and exit the function.*/
				{
					fprintf(stderr,"Error in line %d: entry label %s was defined as extern\n",num_line,name);
					return 0;
				}
				if(strcmp(temp->data.name,name)==0 && temp->data.type_symbol==regular)/*If the label is found and its type is "regular," change the type to "entry."*/
				{
					temp->data.type_symbol=entrys;/*change the type of symbol from regular to entry*/
					find=1;
				}
				temp=(symbol_node*)(temp->next);
			}
			if(!find)/*If the label is not found in the symbol table, return an error message and exit the function.*/
			{
				fprintf(stderr,"Error in line %d: entry label %s is not defined in this file\n",num_line,name);
				return 0;
			}
			free(name);/*Free the memory allocated for the "name" variable.*/
			temp=symbols;/*Set the "temp" variable to the beginning of the symbol table.*/
		}
		find=0;/*zero find to check the next line*/
	}
	return 1;
}

