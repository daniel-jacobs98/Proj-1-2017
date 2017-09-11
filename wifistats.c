/* 
	CITS2002 Project 1 2017
	Name: Daniel Jacobs, Roger Li
	Student Number: 21952584
	Date:	
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void collect_transmitters(char OUIaddress[], char filename[]);
void collect_receivers(char OUIaddress[], char filename[]);
int number_of_lines(char filename[]); //this function may not be necessary

int main(int argc, char *argv[])
{
	//int lineCount = 0;
	char filename[strlen(argv[2]+1)]; //Initialize string (array) to hold filename
	strcpy(filename, argv[2]);

	if(argc<3){  //Check if there are enough inputs 
		printf("Please enter 2 or 3 input parameters\n");
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(argv[1], "t")==0){ //Decide whether to fetch transmitters or receivers
		if(argc == 3){
			printf("argc = 3, t\n");
			collect_transmitters("", filename);
		}
		else if(argc == 4){
			printf("Argc = 4, t\n");
			collect_transmitters(argv[3], filename);
		}
	}
	else if(strcmp(argv[1], "r")==0){
		if(argc == 3){
			printf("Argc = 3, r\n");
			collect_receivers("", filename);
		}
		else if(argc == 4){
			printf("argc = 4, r\n");
			collect_receivers(argv[3], filename);
		}
	}  
}

void collect_receivers(char OUIaddress[], char filename[])
{
	FILE *file = fopen(filename, "r");
	if(file != NULL){
		char currentline[200]; //set a maximum size for the line
		while(fgets(currentline, sizeof currentline, file) != NULL) //read file line-by-line
		{
			fputs(currentline, stdout); //prints current line to shell
		}
			
		fclose(file);
	}
	else{
		printf("Error opening file\n");
		exit(EXIT_FAILURE);
	}
}

void collect_transmitters(char OUIaddress[], char filename[])
{
	printf("%s\n", filename);	
}
int number_of_lines(char filename[])
{
	return 0;	
} 
