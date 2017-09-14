/*ITS2002 Project 1 2017
	Name: Daniel Jacobs, Roger Li
	Student Number: 21952584
	Date:	
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void collect_transmitters(char OUIaddress[], char filename[]);
void read_address_file(char OUIaddress[], char filename[],char choice_flag[]);
int number_of_lines(char filename[]); //this function may not be necessary


int main(int argc, char *argv[])
{
	char filename[strlen(argv[2]+1)]; //Initialize string (array) to hold filename
	strcpy(filename, argv[2]);
	if(argc<3 || argc>4){  //Check if there are enough inputs 
		printf("Please enter 2 or 3 input parameters\n");
		exit(EXIT_FAILURE);
	}
	
	if(argc==3){
		read_address_file("", filename, argv[1]);
	} 
	else 
	{
		read_address_file("", filename, argv[1]); //CHANGE THIS WHEN OUI IS IMPLEMENTED
	}
}

void read_address_file(char OUIaddress[], char filename[], char choice_flag[]){
	FILE *file = fopen(filename, "r");
	int numOfLines = number_of_lines(filename);
	char arrayOfLines[numOfLines][200];//specify num of strings and characters per string
	int loopcount = 0;
	char arrayOfAddresses[numOfLines][18];
	char arrayOfBytes[numOfLines][3];
		
	if(file != NULL){
		char currentline[200]; //set a maximum size for the line
		while(fgets(currentline, sizeof currentline, file) != NULL) //read file line-by-line
		{
			strcpy(arrayOfLines[loopcount], currentline); //puts current line into an array for later manipulation
			loopcount++;
		}
			
		fclose(file);
	}
	else{
		printf("Error opening file\n");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i<numOfLines; i++){
		int k = 0; //This will change depending on whether transmitters or receivers are being fetched.
		for(int j = 0; j<18; j++){ //this loop fetches the MAC addresses
			if(strcmp(choice_flag, "t") == 0){ //This if statement decides whether to fetch receivers of transmitters
				k = j+18;
			} 
			else if(strcmp(choice_flag, "r") == 0){
				k = j+36;
			}
			else{
				printf("Choice flag is invalid. Please enter 'r' or 't'.\n");
			}
			arrayOfAddresses[i][j] =  arrayOfLines[i][k];			
			printf("%c", arrayOfAddresses[i][j]);
		}
		printf("     ");
		for(int j = 0; j<10;j++){ //This loop will extract the bytes transmitted/received into arrayOfBytes
			int k = j + 54;
			if(isdigit(arrayOfLines[i][k])!=0){
				arrayOfBytes[i][j] = arrayOfLines[i][k];
				printf("%c", arrayOfBytes[i][j]);
			}
			else{
				continue;
			}		
		}
		printf("\n");
	}
}

void collect_transmitters(char OUIaddress[], char filename[])
{
	printf("%s\n", filename);	
}
int number_of_lines(char filename[])
{
	FILE *file = fopen(filename, "r");
	int numOfLines = 0;
        for(char c = getc(file); c != EOF; c = getc(file)){ //Counts the number of lines so we know the array dimensions
                if(c=='\n'){
                        numOfLines ++; //iterate line count
                }
        }
	return numOfLines;	
} 
