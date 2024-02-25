#include "macro.h"

/*this function gets two names of files, one to read and one to write to. the function reads from the file as and
switch the macros in the read file by writing their content to the write file(other sentences in file as that are
not macros are also copied write file).*/
int switch_macro(FILE *as , char * name_of_file_am)
{
	FILE *am;
	/*first_field - the first word in line.
	macro name - a string for the name of the macro.
	macro_lines - the content of macro.
	ptr - a pointer to the line that is read from the file.
	space - a string of all the white chars (used to strtok).
	line - the line read from the file.
	copied_line - the copy of line*/
	char *first_field,*macro_name,*macro_lines,*ptr,*space=" \t\v\f\r\n",/*first_field is the first word in line. macro_lines is the content of macro*/
	*line=(char*)malloc(LEN_LINE*sizeof(char)),
	*copied_line=(char*)malloc(LEN_LINE*sizeof(char));
	
	int is_mcr=0,num_line=1,is_proper=1;/*is_mcr is a flag to check if we are now in macro defination. num_line is the index of line.is_proper is a flag to check if macro defines are legal*/
	node *head=NULL;/*head of the linked list of macros*/
	field new_macro;
	if(line==NULL || copied_line==NULL)
	{
		fprintf(stderr,"Error: allocation failed\n");
		exit(0);
	}
	am=fopen(name_of_file_am,"w+");
	if(am==NULL)
	{
		fprintf(stderr,"Cannot open the file %s\n",name_of_file_am);
		return 0;
	}
	fseek(as,0L,SEEK_END);/*checks if there are chars in the files*/
	if(ftell(as)==0)
	{
		fprintf(stderr,"The file is empty\n");
		return 0;
	}
	
	fseek(as,0,SEEK_SET);
	if((check_num_of_chars(&as))>(LEN_LINE-1))/*checks that the number of chars in the first line isn't bigger than 80*/
	{
		fprintf(stdout,"More than 80 chars in line %d\n",num_line);
		is_proper=0;
	}
	while(get_next_line(as,&line)==1)
	{
		num_line++;
		if(is_empty_line(line))/*if this is an empty line-continue*/
			continue;
		memset(copied_line,'\0',LEN_LINE);
		strcpy(copied_line,line);
		strcat(copied_line,"\0");
		
		first_field=strtok(copied_line,space);/*get the first word in line*/
		if((macro_lines=find_mcr(head,first_field))!=NULL)/*if first field is a name of a macro. get its content to macro_lines*/
		{
			fputs(macro_lines,am);
		}
		
		else if(strcmp(first_field,"mcr")==0)/*if first_field is mcr*/
		{	
			is_mcr=1;/*we start macro defination*/
			macro_name=strtok(NULL,space);/*get the name of the macro*/
			if(check_macro_name(macro_name)==0)/*check if macro name is not a saved word*/
				is_proper=0;
			if(strtok(NULL,space)!=NULL)/*checks if there are more chars in line after the name of macro*/
			{
				fprintf(stdout,"Invalid macro: text after the name of macro\n");
				is_proper=0;
			}
			new_macro.name=(char*)calloc('\0',strlen(macro_name)*sizeof(char));
			strcpy(new_macro.name,macro_name);
			new_macro.lines=(char*)calloc('\0',sizeof(char));
			
		}
		else if(is_mcr==1 && strcmp(first_field,"endmcr")!=0)/*if this line is from the content of a macro defination*/
		{
			new_macro.lines=(char*)realloc(new_macro.lines,(strlen(new_macro.lines)+strlen(line)+2)*sizeof(char));
			strcat(new_macro.lines,line);/*add the current line to macro_lines*/
			strcat(new_macro.lines,"\n\0");
		}			
		else if(strcmp(first_field,"endmcr")==0)/*if first_field is endmcr*/
		{
			ptr=strstr(line,"endmcr");
			ptr+=strlen("endmcr");/*set ptr to point on the first char after "endmcr"*/
			while(*ptr!='\0')
			{
				if(!isspace(*ptr))/*if there is a char that is not a space after "endmcr"*/
				{
					fprintf(stdout,"Invalid macro: text after 'endmcr'\n");
					is_proper=0;
				}
			}
			
			add_mcr(&head,new_macro);/*we arrive to the end of macro defination - add this macro*/
			is_mcr=0;
			free(new_macro.name);
			free(new_macro.lines);
		}
		else if(is_mcr==0)/*if the current line is regular*/
		{
			fputs(line,am);/*copy the line to the file .am*/
			fputs("\n",am);
		}
		
		if((check_num_of_chars(&as))>(LEN_LINE-1))/*if number of chars in line is bigger than 80*/
		{
			fprintf(stdout,"More than 80 chars in line %d\n",num_line);
			is_proper=0;
		}
	}
	if(!is_proper)/*if there are errors in this file*/
		remove(name_of_file_am);
	fclose(am);
	free_macro(head);
	free(line);
	free(copied_line);
	return is_proper;
}

/*this function gets a pointer to the head of the linked list of macros, a macro name and a pointer to a string 
represents the content of the macro. the function searches for the macro in the list of macros and when finding it- 
copies the content of this macro to 'lines'.*/
char* find_mcr(node *head,char *name)
{
	node *temp = head;
	while(temp!=NULL)
	{ 
		if(strcmp((temp->data).name,name)==0)/*we find the macro in the list of macros*/
		{
			return (temp->data).lines;			
		}
		temp = (node *)(temp->next);	
	}
	
	return NULL;	
}

/*this function counts the number of chars in line.*/
int check_num_of_chars(FILE **as)
{
	char tav;
	int num_of_chars=0;
	while((tav=fgetc(*as))!='\n' && tav!=EOF)/*if we didnt get the end of line*/
	{
		num_of_chars++;
	}
	fseek(*as,(num_of_chars+1)*(-1),SEEK_CUR);/*return to the previous line in order to continue the program correctly*/
	return num_of_chars;
}

/*this function gets a pointer to the head of the linked list of macros and a field f (data of node of macro)
and add the macro of the data f to the list of macros.*/
void add_mcr(node ** head, field f )
{
	node * new_node = (node *) malloc(sizeof(node));/*the new node to add*/
	if(new_node==NULL)
	{
		fprintf(stderr,"Allocation failed\n");
		return;
	}
	if (*head==NULL)/*if there are no nodes in the list*/
	{
		*head = new_node;
	}
	else
	{
		node * temp = *head;
		while(temp->next!=NULL)/*set temp to point on the last node*/
		{
			temp = (node *)temp->next;
		}
		temp->next =(pointer)new_node;
	}
	new_node->next=NULL;
	(new_node->data).name=(char*)malloc(strlen(f.name)*sizeof(char));
	(new_node->data).lines=(char*)malloc(strlen(f.lines)*sizeof(char));
	if((new_node->data).name==NULL || (new_node->data).lines==NULL)
	{
		fprintf(stderr,"Error: allocation failed\n");
		exit(0);
	}
	strcpy((new_node->data).name , f.name);
	strcpy((new_node->data).lines , f.lines);
	strcat((new_node->data).name,"\0");
	strcat((new_node->data).lines,"\0");
}

/*this function checks if the name of macro is a saved word or not*/
int check_macro_name(char *name)
{
	/*an array of all the saved words*/
	char *arr[]={".extern",".entry",".string",".data","mov","cmp","add","sub","not","clr","lea","inc","dec","jmp",
	"bne","red","prn","jsr","rts","stop","mcr","endmcr",NULL};
	int i=0;
	while(arr[i]!=NULL)
	{
		if(strcmp(name,arr[i])==0)/*if name is a saved word*/
		{
			fprintf(stderr,"Error: name of macro %s is a saved word\n",name);
			return 0;
		}
		i++;
	}
	return 1;
}
/*this function frees the linked list of macros.*/
void free_macro(node* head)
{
	node* temp = head;
	while (temp != NULL)
	{
		node* temp2 = temp;
		temp = (node*)temp->next;
		free(temp2->data.name);
		free(temp2->data.lines);
		free(temp2);
	}
}
