/*ITS2002 Project 1 2017
	Name: Daniel Jacobs, Roger Li
	Student Number: 21952584
	Date:	
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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
		read_address_file("", filename, argv[1]); //Send blank string if no OUI file is provided
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
	int nAddresses = 0;
	long arrayOfBytes[numOfLines];
	for (int i = 0; i < numOfLines; ++i)
		arrayOfBytes[i] = 0;
//	char UniqueAddressArray[numOfLines][18];
//	int UniqueByteArray[numOfLines];
		
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
		double time;
		char transmitter[18], receiver[18];
		long bytes;
		sscanf(arrayOfLines[i], "%lf\t%s\t%s\t%ld", &time, transmitter, receiver, &bytes);
		char Unique = 1;
		int iter = 0;
		for (int j = 0; j < nAddresses; ++j) {
			if (!strcmp(choice_flag[0] == 't' ? transmitter : receiver, arrayOfAddresses[j])) {
				Unique = 0;
				iter = j;
				break;
			}
		}
		if(Unique){
			memcpy(arrayOfAddresses[nAddresses], choice_flag[0] == 't' ? transmitter : receiver, 18);			
			iter = nAddresses;
			++nAddresses;
		}
		arrayOfBytes[iter] += bytes; 
	//	printf("%s\t%ld\n", arrayOfAddresses[iter], arrayOfBytes[iter]);
		}
		for (int i = 0; i < nAddresses; i++){
			printf("%s\t%ld\n", arrayOfAddresses[i], arrayOfBytes[i]);
		}	
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
