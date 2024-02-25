/*
Project made by:
Firda Lea Barzilai -ID 213990062
Miriam Shnitzer -ID 326377066
*/

#include "macro.h"
#include "encode.h"
#include "first_trans.h"
#include "second_trans.h"
#include "checks.h"

/*this project does the work of assembler. it translates the assembly languege to machine language. the source files are given in the command line*/
int main(int argc,char * argv[])
{
	/*the name of all the files*/
	char *name_of_file_as, *name_of_file_am, *name_of_file_ob,*name_of_file_ext,*name_of_file_ent;
	FILE *am,*as;
	
	/*len labels is the length of array labels, is_proper is a flag that say if there are errors in the file or not*/
	int len_labels,i,is_proper;
	if(argc<2)
	{
		fprintf(stderr,"Error: no files\n");
		exit(0);
	}
	for(i=1;i<argc;i++)
	{
		/*labels is an array of struct label details. we use it to get data about labels that are in command sentences
		like: cmp  LABEL, #1 (labels declared and freed in every loop)*/
		label_details *labels;
		len_labels=0,is_proper=1;
		name_of_file_as=(char *)malloc(sizeof(char)*(strlen(argv[i])+strlen(END_AS)+1));
		name_of_file_am=(char *)malloc(sizeof(char)*(strlen(argv[i])+strlen(END_AM)+1));
		name_of_file_ob=(char *)malloc(sizeof(char)*(strlen(argv[i])+strlen(END_OB)+1));
		name_of_file_ext=(char *)malloc(sizeof(char)*(strlen(argv[i])+strlen(END_EXT)+1));
		name_of_file_ent=(char *)malloc(sizeof(char)*(strlen(argv[i])+strlen(END_ENT)+1));
		if(name_of_file_as==NULL || name_of_file_am==NULL || name_of_file_ob==NULL || name_of_file_ext==NULL || name_of_file_ent==NULL)
		{
			fprintf(stderr,"Allocation failed\n");
			exit(0);
		}
		strcpy(name_of_file_as,argv[i]);
		strcpy(name_of_file_am,argv[i]);
		strcpy(name_of_file_ob,argv[i]);
		strcpy(name_of_file_ext,argv[i]);
		strcpy(name_of_file_ent,argv[i]);
		
		strcat(name_of_file_as,END_AS);
		strcat(name_of_file_am,END_AM);
		strcat(name_of_file_ob,END_OB);
		strcat(name_of_file_ext,END_EXT);
		strcat(name_of_file_ent,END_ENT);
		
		fprintf(stderr,"\nIn file %s:\n",name_of_file_am);
		as=fopen(name_of_file_as,"r");
		if(as==NULL)
		{
			fprintf(stderr,"Cannot open the file %s\n",name_of_file_as);
			return 0;
		}
		
		/*switching macro*/
		if(switch_macro(as,name_of_file_am)==0)
			return 0;
		fclose(as);
		
		/*first transport*/
		am=fopen(name_of_file_am,"r+");
		if(am==NULL)
		{
			fprintf(stderr,"Cannot open the file %s\n",name_of_file_am);
			return 0;
		}
		labels=(label_details*)malloc(sizeof(label_details));
		if(main_first_trans(am,&labels,&len_labels)==0)
			is_proper=0;
		fclose(am);
		/*second transport*/
		am=fopen(name_of_file_am,"r+");
		if(main_second_trans(labels,len_labels,am)==0)
			is_proper=0;
		fclose(am);
		
		/*print output files*/
		if(is_proper)/*if there are no errors in the source file*/
		{
			print_output(name_of_file_ob);
		}

		free_instruction_code();
		free_data_code();
		if(is_proper)/*if there are no errors in the source file*/
		{
			print_extern_entry(name_of_file_ext,name_of_file_ent,labels,len_labels);
			fprintf(stderr,"no errors\n");
		}
		free_labels(labels,len_labels);
		free_symbols();
		free(name_of_file_as);
		free(name_of_file_am);
		free(name_of_file_ob);
		free(name_of_file_ext);
		free(name_of_file_ent);
		
	}
	return 1;
}
