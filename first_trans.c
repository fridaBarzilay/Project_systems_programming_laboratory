#include "first_trans.h"

/*this function is the main function of the first transition. it sends to all the functions that encode the assembly file*/
int main_first_trans(FILE *am,label_details **labels,int *len_labels)
{
	/*is label - a flag to check if there is a label in line
	is_operand - a flag to check if in the current command sentence the operands are from jump addressing method
	is_proper - a flag to check if there are errors in file or not.
	is_register - a flag to check if the first operand is a register (in order to encode two registers in the same word.
	num_opcode - the opcode of a command.
	return_value - a value returned from data_sentence function.*/
	int is_label=0,i=0,is_operand=0,is_proper=1,is_register=0,num_opcode,return_value;
	
	char *line=(char*)malloc(sizeof(char)*LEN_LINE),/*the current line that is read from the file*/
	*copied_line=(char*)malloc(sizeof(char)*LEN_LINE),/*a copy of line*/
	*name_of_label,/*string saves the name of a label*/
	*check_line,/*a pointer to line. it runs over the content of line*/
	*arg;/*string saves the name of argument in command sentence.*/
	
	node_inst_word *inst_temp;/*a pointer to the linked list saving the encoding of instruction words*/
	node_data_word *data_temp;/*a pointer to the linked list saving the encoding of data words*/
	
	if(line==NULL || copied_line==NULL)
	{
		fprintf(stderr,"Error in line %d : allocation failed\n",num_line);
		exit(0);
	}
	instruction_code=NULL;
	data_code=NULL;
	symbols=NULL;
	inst_temp=instruction_code;
	data_temp=data_code;
	IC=0;
	DC=0;
	num_line=0;
	fseek(am,0,SEEK_SET);

	while(get_next_line(am,&line)==1)
	{	
		num_line++;
		is_label=0;
		if(is_empty_line(line))/*if this is an empty line*/
			continue;
		strcpy(copied_line,line);
		strcat(copied_line,"\0");
		
		check_line=copied_line;
		quit_spaces(&check_line);
		if(*check_line==';')
			continue;
		name_of_label = strtok(copied_line,":");/*check if there is a label*/
		if(strcmp(name_of_label,line)!=0)
		{
			is_label=1;
		}	
		
		return_value=data_sentence(line,&check_line,name_of_label,is_label,&data_temp);/*check if this label is data label*/
		if(return_value==0)/*there is an error with the label*/
		{
			is_proper=0;
			is_label=0;
			continue;
		}
		else if(return_value==2)/*this is a data sentence*/
		{
			is_label=0;
			continue;
		}
		
		else if(is_label)/*there is a label in command sentence*/
		{
			if(tabel_symbol(name_of_label,instruc,regulars)==0)/*if there are errors in entering the label to list of symbols*/
					is_proper=0;
			check_line=line;
			check_line+=(strlen(name_of_label)+1);/*check_line point to the char after the ':'*/
			if(proper_space(check_line)==0)
			{
				fprintf(stderr,"Error in line %d: no space between name of label to the next word\n",num_line);
				is_proper=0;
			}
		}
		else
			check_line=line;/*no label- check_line points to the beggining of line*/
		add_inst_node(&inst_temp,regular);
		set_on_last_inst_node(&inst_temp);
		/*encode name of command*/
		quit_spaces(&check_line);
		arg=(char*)calloc('\0',sizeof(char));
		if(arg==NULL)
		{
			fprintf(stderr,"Error in line %d : allocation failed\n",num_line);
			exit(0);
		}
		get_word(&check_line,&arg);/*arg gets the name of command*/
		if(arg!=NULL)
		{
			num_opcode=encode_opcode(arg,&inst_temp);/*get opcode of command*/
			if(num_opcode==-1)/*if there is no such a command*/
			{
				fprintf(stderr,"Error in line %d: undefined command\n",num_line);
				is_proper=0;
				continue;
			}
		}
		
		if(general_check(num_opcode,check_line)==0)
			is_proper=0;
		zero_regular_word(&inst_temp);/*zero all the fields of instruction word of regular type*/
		i=0;
		/*encode arguments*/
		while(strcmp(check_line,"\0")!=0 && num_opcode!=14 && num_opcode!=15)
		{	
			quit_spaces(&check_line);
				
			arg=(char*)calloc('\0',sizeof(char));
			if(arg==NULL)
			{
				fprintf(stderr,"Error in line %d : allocation failed\n",num_line);
				exit(0);
			}
			get_word(&check_line,&arg);/*get name of argument*/
			if(is_operand==1 && is_proper)/*if this is one of the operands of jump addressing method that are in parenthesis*/
				/*encode this argument, i-1=0 if the arg is source or 1 if its destination*/
				is_operand=get_arg_type(arg,&inst_temp,i-1,check_line,1,&is_register,labels,len_labels);
			else if(is_proper)
				/*this is not jump addressing method*/
				is_operand=get_arg_type(arg,&inst_temp,i,check_line,0,&is_register,labels,len_labels);
			if(is_operand==-1)/*the return value from the function is -1 means that there is an error*/
				is_proper=0;
			quit_spaces(&check_line);

			/*if(is_operand==0 && check_line[0]!='\0' && check_line[0]!=',')*//*this is not jump addressing method*/
			/*{
				fprintf(stderr,"Error in line %d : missing comma between arguments\n",num_line);
				is_proper=0;
			}*/
			if(*check_line!='\0')
				check_line++;
			if(is_empty_line(check_line))
			{
				free(arg);
				break;
			}
			free(arg);
			i++;
		}
		is_label=0;
		is_operand=0;
		is_register=0;
	}
	update_symbol_add();
	if(is_proper && (IC+DC)>256)
	{
		fprintf(stderr,"Error: the code and data image is larger than 256 bytes\n");
		exit(0);
	}
	free(line);
	free(copied_line);
	return is_proper;
}
/*this function treats directive_sentences (.data/.string/.extern/.entry)*/
int data_sentence(char *line,char **check_line,char *name_of_label,int is_label,node_data_word **data_temp)
{
	int is_proper=1;/*flag to check if the line is legal*/
	if((*check_line=strstr(line,".string"))!=NULL)
	{
		is_proper=2;/*this is data sentence*/
		if(is_label)
			if(tabel_symbol(name_of_label,data,regulars)==0)/*put the label in the list of symbols*/
				is_proper=0;
		*check_line = (char*)(strchr(line,'"'));/*put the pointer check_line to point on the first '"'*/
		if(*check_line==NULL)
		{
			fprintf(stderr,"Error in line %d: missing apostrophes\n",num_line);
			return 0;
		}
		if(check_string_islegal(*check_line)==0)/*check if the data is legal*/
			is_proper=0;
		encode_data_string(*check_line+1,data_temp);/*encode the data*/		
	}
	
	else if((*check_line=strstr(line,".data"))!=NULL) 
	{
		is_proper=2;/*this is data sentence*/
		if(is_label)
			if(tabel_symbol(name_of_label,data,regulars)==0)/*put the label in the list of symbols*/
				is_proper=0;
		*check_line+=strlen(".data");/*put the pointer check_line to point on the first char after .data*/
		if(valid_data(*check_line)==0)/*check if the data is legal*/
			is_proper=0;
		quit_spaces(check_line);
		encode_data_data(*check_line,data_temp);/*encode the data*/				
	}
			
	else if((*check_line=strstr(line,".extern"))!=NULL) 
	{
		is_proper=2;/*this is data sentence*/
		if(is_label)
		{
			fprintf(stderr,"Error in line %d: .extern defined as label\n",num_line);
			is_proper=0;
		}
		*check_line+=(strlen(".extern"));/*put check_line to point on the first char after .extern*/
		if(check_entry_or_extern(*check_line)==0)/*check if extern defination is legal*/
			is_proper=0;
		quit_spaces(check_line);
		if(tabel_symbol(*check_line,data,externs)==0)/*put the symbol of extern in the list of symbols*/
			is_proper=0;
	}
	else if((*check_line=strstr(line,".entry"))!=NULL) 
	{
		is_proper=2;/*this is data sentence*/
		if(is_label)
		{
			fprintf(stderr,"Error in line %d: .entry defined as label\n",num_line);
			is_proper=0;
		}
		*check_line+=(strlen(".entry"));/*put check_line to point on the first char after .entry*/
		if(check_entry_or_extern(*check_line)==0)/*check if entry defination is legal*/
			is_proper=0;
	}
	return is_proper;
}
/*this function gets an operand, a pointer to a node_inst_word and the type of operand and encode all the fields 
that are connected to this operand. if the encodeing finished successfully- return 1, otherwise- return 0.
if the argument is of addresing method 2 (jump) 2 will be returned.*/
int get_arg_type(char *arg,node_inst_word **temp,int oper_type,char *check_line,int is_operand,int *is_register,label_details **labels,int *len_labels)
{
	char *ptr;/*for strtol function*/
	/*arg_type - the type of the argument (num/label/jump/reg).
	number - can be the immediate number or the number of register.
	is_jump - a flag to check if we meet a jump addressing method.*/
	int arg_type,number=0,is_jump=0;
	if(oper_type==source && *check_line=='\0')
		oper_type=alone;/*there is only one operand in the sentence*/
	
	if(arg[0]=='#')/*if arg is an immediate number*/
	{
		arg++;/*set arg to point on the number*/
		number=strtol(arg,&ptr,10);
		arg_type=num;/*this argument is number*/
		if(oper_type!=dest && !is_operand)/*if this is source or alone operand and we are not in jump*/
		{
			/*temp points on the regular word. encode the regular word*/
			(*temp)->data.regular.param1=0;
			(*temp)->data.regular.param2=0;
			(*temp)->data.regular.are=0;
		}
		*is_register=0;
		
	}
	else if(arg[0]=='r' && (((number=strtol(arg+1,&ptr,10))>0 && number<=7) || (number=arg[1])=='0'))/*this operand is register*/
	{
		arg++;/*set arg to point on the number of register*/
		arg_type=reg;
		if(oper_type!=dest && !is_operand)/*if this is source or alone operand and we are not in jump*/
		{
			/*temp points on the regular word. encode the regular word*/
			(*temp)->data.regular.param1=0;
			(*temp)->data.regular.param2=0;
			(*temp)->data.regular.are=0;
		}
		else if(oper_type==dest && !*is_register)/*if this is destination operand and we are not in jump*/
			*is_register=0;
	}
	else if(isalpha(*arg))/*the operand is label*/
	{
		if(*check_line=='(')/*if we are in jump addressing method*/
		{
			arg_type=jump;
			(*temp)->data.regular.dest=arg_type;
			(*temp)->data.regular.source=0;
			is_jump=1;
			
		}
		else/*regular label*/
		{
			arg_type=label;
			if(oper_type!=dest && !is_operand)/*if this is source or alone operand and we are not in jump*/
			{
				/*temp points on the regular word. encode the regular word*/
				(*temp)->data.regular.param1=0;
				(*temp)->data.regular.param2=0;
			}
		}
		*is_register=0;
		/*enter this label to the array labels*/
		*labels=(label_details*)realloc(*labels,sizeof(label_details)*(*len_labels+1));
		(*labels)[*len_labels].name=(char*)malloc(sizeof(char)*strlen(arg+1));
		strcpy((*labels)[*len_labels].name,arg);
		strcat((*labels)[*len_labels].name,"\0");
		(*labels)[*len_labels].num_of_node=IC+1;
		(*labels)[*len_labels].line=num_line;
		(*len_labels)++;
	}
	
	else
	{	
		return -1;
	}
	if(is_operand)/*if we are in jump addressing method*/
	{
		is_jump=1;
		if(oper_type==source)/*if this is the first operand in the apostrophes*/
		{
			/*the regular word is in the previous word*/
			get_prev_inst_word(temp);
			(*temp)->data.regular.param1=arg_type;
			*temp=(node_inst_word*)((*temp)->next);
		}
		else if(oper_type==dest)/*if this is the second operand in the apostrophes*/
		{
			/*the regular word is in the previous to previous word*/
			get_prev_inst_word(temp);
			get_prev_inst_word(temp);
			(*temp)->data.regular.param2=arg_type;
			*temp=(node_inst_word*)((*temp)->next);
			*temp=(node_inst_word*)((*temp)->next);
		}
	}	
	else if(oper_type==source && !is_jump)/*if this is source operand but not the label in jump addressing method*/
		(*temp)->data.regular.source=arg_type;
	else if(oper_type==alone)
	{
		(*temp)->data.regular.dest=arg_type;
		(*temp)->data.regular.source=0;
	}			
	else if(oper_type==dest)
	{	
		get_prev_inst_word(temp);
		(*temp)->data.regular.dest=arg_type;
		*temp=(node_inst_word*)((*temp)->next);		
	}
	if(!*is_register)/*if the previous and the current operands are not registers together*/
	{
		/*add new word*/
		if(arg_type==num || arg_type==label ||arg_type==jump)
			add_inst_node(temp,num_or_add);
		if(arg_type==reg)
			add_inst_node(temp,regs);
		set_on_last_inst_node(temp);
	}
	if(arg_type==num)
		(*temp)->data.num_or_add.value=number;
	if(arg_type==reg)
	{
		if(oper_type==source)
		{
			(*temp)->data.regs.reg_source=number;
			(*temp)->data.regs.reg_dest=0;
			*is_register=1;
		}
	
		else
		{
			(*temp)->data.regs.reg_dest=number;
			if(!*is_register)
				(*temp)->data.regs.reg_source=0;
		}
	}
	if(arg_type==label || arg_type==jump )
	{
		(*temp)->data.num_or_add.are=1;
		(*temp)->data.num_or_add.value=0;/*the value is 0 till the second transport, then we change it*/
	}
	return is_jump;
}

/*this function gets a string of data and a pointer to a node in the linked list represents the encoding of data and 
encode the string into the linked list.*/
void encode_data_string(char *data,node_data_word **temp)
{
	int i=0;
	char c;/*the current char in the string data to encode*/
	while((c=data[i])!='"')/*while we didnt arrive the end of the string data*/
	{
		add_data_node(temp);
		set_on_last_data_node(temp);/*set temp to point on the last node of data_code*/		
		(*temp)->data.value=c;		
		i++;
	}
	add_data_node(temp);
	set_on_last_data_node(temp);		
	(*temp)->data.value='\0';	
}

/*this function gets a pointer to a string of numbers (data) and a pointer to a pointer of a linked list of the
 encodings of data sentences and encodes the numbers to the linked list.*/
void encode_data_data(char *data ,node_data_word **temp)
{
	char *ptr;/*used to strtol function*/
	char *str_num;/*the string version of the numbers*/
	int num;/*the integer version of the numbers*/
	remove_spaces(&data);
	str_num=strtok(data,",");
	while(str_num!=NULL)
	{
		num=strtol(str_num,&ptr,10);
		add_data_node(temp);
		set_on_last_data_node(temp);/*set temp to point on the last node of data_code*/
		(*temp)->data.value=num;
		str_num=strtok(NULL,",");	
	}	
}

/*this function encode the opcode of command.*/
int encode_opcode(char *command_name, node_inst_word **temp)
{
	static func f={
			{"mov",0},
			{"cmp",1},
			{"add",2},
			{"sub",3},
			{"not",4},
			{"clr",5},
			{"lea",6},
			{"inc",7},
			{"dec",8},
			{"jmp",9},
			{"bne",10},
			{"red",11},
			{"prn",12},
			{"jsr",13},
			{"rts",14},
			{"stop",15}};
        int i;
    	for (i=0; i<MAX_FUNC; i++)
        {
        	if (strcmp(command_name, f[i].name)==0)/*if the name is really a name of command*/
            	{
            		(*temp)->data.regular.opcode=f[i].code;/*encode the opcode in temp*/
            		return i;
            	}
        }
   	return -1; /*not found*/    
}

/*this function gets a name of label, the type of command of the label and the type of the label, and enter this
label to the linked list of symbols.*/
int tabel_symbol(char *symbol_name, int type_comma, int type_symb)
{
	symbol_node *new_node=(symbol_node*)malloc(sizeof(symbol_node));/*new node to the list of symbols*/
	
	/*type - get the return value from get_type_symbol function.
	defined - a flag to check if the symbol already defined, but its legal.
	find - get the return value from find_symbol function (the number of times that the label is exist in the list of symbols.
	is_legal - a flag to check if the symbol defination is legal. */
	int type,defined=0,find,is_legal=1;
	
	if(new_node==NULL)
    	{
       		fprintf(stderr,"Error in line %d : allocation failed\n",num_line);
        	exit(0);
    	} 
	if(!check_name_of_label(symbol_name))/*check if the name of label is legal*/
		is_legal=0;
	
	if((find=find_symbol(symbol_name))==2)/*if the symbol is already defined twice*/
	{
		fprintf(stderr,"Error in line %d : the symbol %s is already exist\n",num_line,symbol_name);
		free(new_node);
		return 0;
	}
	else if(find==1)/*if the symbol is already defined once- checl if its legal to enter the new*/
	{	
		type=get_type_symbol(symbol_name);
		/*if the type in the two defination is the same, or the types of two defination are regulars and externs*/
		if(type==type_symb || (type==regulars && type_symb==externs) || (type==externs && type_symb==regulars))
		{
			fprintf(stderr,"Error in line %d : the symbol %s is already exist\n",num_line,symbol_name);
			free(new_node);
			return 0;
		}
		else
			defined=1;
	}	
	if(!defined)/*the symbol wasn't defined*/
    	{
    		new_node->data.name=(char*)malloc(sizeof(char)*strlen(symbol_name+1)); 
    		if(new_node->data.name==NULL)
    		{
       			fprintf(stderr,"Error in line %d : allocation failed\n",num_line);
        		exit(0);
    		}         
		strcpy(new_node->data.name,symbol_name);
		strcat(new_node->data.name,"\0");
		
		new_node->next=NULL;
		if(symbols==NULL)/*if the list is still empty*/
    		{
       			symbols=new_node;
    		}
    		else
    		{
    			symbol_node *temp=symbols;
        		while(temp->next!=NULL)/*set temp to point on the last node*/
        		{
            			temp=(symbol_node*)(temp->next);
        		}
        		temp->next=(ptr)new_node;
    		}
    		if(type_symb==regulars)
		{
			if(type_comma == data)/*if this is data symbol*/
			{
				new_node->data.address=DC+ADDRESS_START;
			}
			else if(type_comma == instruc)/*if this is instruction symbol*/
				new_node->data.address=IC+ADDRESS_START;
		}
		else if(type_symb==externs)/*if this is an extern symbol*/
			new_node->data.address=0;
			
		new_node->data.type_symbol=type_symb;
		new_node->data.type_command=type_comma;
    	}
    	else/*the symbol was defined but its legal*/
    	{
    		symbol_node *temp2=get_symbol_node(symbol_name);/*get the node of the defined symbol*/
    		if(type_symb==regulars)
		{
			if(type_comma == data)/*if this is data symbol*/
			{
				temp2->data.address=DC+ADDRESS_START;
			}
			else if(type_comma == instruc)/*if this is instruction symbol*/
				temp2->data.address=IC+ADDRESS_START;
		}
		else if(type_symb==externs)/*if this is extern symbol*/
			temp2->data.address=0;
		if(type!=entrys && type!=externs)/*if the defined symbol was defined as regular only*/
    		{
    			temp2->data.type_symbol=type_symb;/*change the defined symbol's type of symbol to be the current type of symbol(regular/entry/extern)*/
    		}
    		if(type!=regulars)/*if the label was'nt defined earlier as regular (but as extern or as entry)*/
		{
			temp2->data.type_command=type_comma;/*change the defined symbol's type of command to be the current type of command (data/instruction)*/
		}
    		
    	}
    	return is_legal;   	
}


/*this function gets a pointer to a node_data_word node and add to this linked list another node.*/
void add_data_node(node_data_word **temp)
{
	node_data_word * new_node = (node_data_word *) malloc(sizeof(node_data_word));
	if(new_node==NULL)
	{
		fprintf(stderr,"Error in line %d : allocation failed\n",num_line);
		exit(0);
	}
	if (data_code==NULL)/*if the list is stil empty*/
	{
		data_code = new_node;
	}
	else
	{
		(*temp)->next =(data_ptr)new_node;/*temp always points to the last node, so set temp's next to be new_node*/
	}
	new_node->next=NULL;
	DC++;
}

/*this function gets a pointer to a node_inst_word node and add to this linked list another node.*/
void add_inst_node(node_inst_word **temp,int type)
{
	node_inst_word * new_node = (node_inst_word *) malloc(sizeof(node_inst_word));
	if(new_node==NULL)
	{
		fprintf(stderr,"Error in line %d : allocation failed\n",num_line);
		exit(0);
	}
	if (instruction_code==NULL)/*if the list is stil empty*/
	{
		instruction_code = new_node;
	}
	else
	{
		(*temp)->next =(inst_ptr)new_node;/*temp always points to the last node, so set temp's next to be new_node*/
	}
	new_node->type=type;
	new_node->next=NULL;
	IC++;
}
/*this function set the pointer to node_data_word- temp on the last node of the global variable 'data_code'.*/
void set_on_last_data_node(node_data_word **temp)
{
	*temp=data_code;
	if(*temp==NULL)
		return;
	while((*temp)->next!=NULL)/*advance temp till it points on the last node*/
		*temp = (node_data_word*)((*temp)->next);	
}

/*this function set the pointer to node_inst_word- temp on the last node of the global variable 'instruction_code'
@param temp a pointer to the linked list 'instruction_code'*/
void set_on_last_inst_node(node_inst_word **temp)
{
	*temp=instruction_code;
	while((*temp)->next!=NULL)/*advance temp till it points on the last node*/
		*temp = (node_inst_word*)((*temp)->next);
}

/*this function gets a pointer to node_inst_word and sets this pointer to point on the previous node.*/
void get_prev_inst_word(node_inst_word **temp)
{
	node_inst_word *pointer=*temp;
	*temp=instruction_code;
	while((node_inst_word*)((*temp)->next) != pointer)
		*temp = (node_inst_word*)((*temp)->next);	
}

/*this function gets a name of symbol and return 1 if this symbol is exist in the tabel of symbols and 0 if not.*/
int find_symbol(char *symbol_name)
{
	symbol_node *temp=symbols;
	int sum=0;
	while(temp!=NULL)
        {
        	if(strcmp(symbol_name , temp->data.name)==0)/*if we find the symbol in the list of symbols*/
        	{
        		sum++;
        	}	
            	temp=(symbol_node*)temp->next;
        }       
        return sum;
}

/*this function adds to every symbol that appears in data sentences the last IC*/
void update_symbol_add()
{
	symbol_node *temp=symbols;
	while(temp!=NULL)
	{
		if(temp->data.type_command==data && temp->data.type_symbol!=externs)/*if this is a data symbol*/
			temp->data.address+=IC;/*add the final IC to the address of symbol*/
		temp=(symbol_node*)(temp->next);
	}

}
