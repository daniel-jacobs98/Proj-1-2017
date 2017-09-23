/*
	CITS2002 Project 1 2017
	Name: Daniel Jacobs, Roger Li
	Student Number: 21952584, 21955968
	Date: 22/09/2017	
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

void read_address_file(char OUIaddress[], char filename[],char choice_flag[]); 
int number_of_lines(char filename[]); 
void OUI_compare(char arrayOfAddresses[][18], long arrayOfBytes[], char OUIname[], int num_address_lines, int num_unique_addresses); 
void sort_func_OUI(long arrayOfBytes[], char arrayOfAddresses[][18], char transmitted_vendor_list[][90], int num_unique_addresses, char transmitted_address_prefixes[][9]);
void sort_func_nOUI(long arrayOfBytes[], char arrayOfAddresses[][18], int num_unique_addresses); 

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
		read_address_file(argv[3], filename, argv[1]); //CHANGE THIS WHEN OUI IS IMPLEMENTED
	}
}

/*
	Reads the packet file and extracts the transmitting/receiving address and the bytes transmitted.
	Returns nothing.
	May call OUI_compare if an OUI file is given, or sort_func_nOUI if no OUI if provided.
*/
void read_address_file(char OUIaddress[], char filename[], char choice_flag[]){ 
	FILE *file = fopen(filename, "r"); 
	int numOfLines = number_of_lines(filename);
	char arrayOfLines[numOfLines][200];//specify num of strings and characters per string
	int loopcount = 0;
	char arrayOfAddresses[numOfLines][18];
	int nAddresses = 0;
	long arrayOfBytes[numOfLines];
	int broadcast_address_count = 0;

	for (int i = 0; i < numOfLines; ++i){ //this ensures there is no other characters in this array
		arrayOfBytes[i] = 0;
	}	
	if(file != NULL){
		char currentline[200]; //set a maximum size for the line
		while(fgets(currentline, sizeof currentline, file) != NULL){ //read file line-by-line
			strcpy(arrayOfLines[loopcount], currentline); //puts current line into an array for later manipulation
			loopcount++;
		}
			
	}
	else{
		printf("Error opening file\n");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i<numOfLines; i++){ //loop removes broadcast address from arrayOfLines
		if(strcmp(arrayOfLines[i], "ff:ff:ff:ff:ff:ff") == 0){
			strcpy(arrayOfLines[i], "");
			broadcast_address_count ++;
			for(int j = i; j<numOfLines; j++){
				strcpy(arrayOfLines[j], arrayOfLines[j+1]);	
			}
		}
	}
	numOfLines = numOfLines - broadcast_address_count; //changes the numOfLines by the number of broadcast addresses found
	fclose(file); 
	for(int i = 0; i<numOfLines; i++){
		double time; //store transmit time
		char transmitter[18], receiver[18]; //store current transmitter & receiver address
		long bytes; //store bytes transmitted
		sscanf(arrayOfLines[i], "%lf\t%s\t%s\t%ld", &time, transmitter, receiver, &bytes); //reads arrayOfLines and puts time, transmitter, receiver and bytes into their own variables
		char Unique = 1;
		int iter = 0;
		for (int j = 0; j < nAddresses; ++j) {
			if (!strcmp(choice_flag[0] == 't' ? transmitter : receiver, arrayOfAddresses[j])) { //this checks if the transmitter/receiver address is unique
				Unique = 0;
				iter = j;
				break;
			}
		}
		if(Unique){
			memcpy(arrayOfAddresses[nAddresses], choice_flag[0] == 't' ? transmitter : receiver, 18); //copies transmitter or receiver to unique arrayOfAddresses			
			iter = nAddresses;
			++nAddresses;
		}
		arrayOfBytes[iter] += bytes; //this sums bytes from duplicate addresses
	}

	if(strcmp(OUIaddress, "") != 0){ //if an OUI file is provided, run OUI_compae
		OUI_compare(arrayOfAddresses, arrayOfBytes, OUIaddress, numOfLines, nAddresses);
	}
	else{ //sort unique addresses and bytes immediately
		sort_func_nOUI(arrayOfBytes, arrayOfAddresses, nAddresses); 			
	}	
}

/*
	Counts number of lines in the file provided to it.
	Returns number of lines to calling function.
*/
int number_of_lines(char filename[])
{
	FILE *file = fopen(filename, "r");
	int numOfLines = 0;
        for(char c = getc(file); c != EOF; c = getc(file)){ //Counts the number of lines so we know the array dimensions
                if(c=='\n'){
                        numOfLines ++; //iterate line count
                }
        }
	fclose(file); 
	return numOfLines;	
} 

/*
	Given an arrayOfAddresses and an arrayOfBytes, finds the vendor name for the given addresses and forms an array of Vendor prefix, address and bytes transmitted.
	Returns nothing.
	Calls sort_func_OUI.
*/
void OUI_compare(char arrayOfAddresses[][18], long arrayOfBytes[], char OUIname[], int num_address_lines, int num_unique_addresses){
	FILE *file = fopen(OUIname, "r");
	int loopcount = 0;
	int num_OUI_lines = number_of_lines(OUIname);
	char OuiAddressArray[num_OUI_lines][9], OuiLineArray[num_OUI_lines][100], OuiNameArray[num_OUI_lines][90];
	char OuiAdd[9], OuiName[90];
	char transmitted_vendor_list[num_unique_addresses][90];
	char transmitted_address_prefixes[num_unique_addresses][9];
	char Comparison_oui_array[num_OUI_lines][9]; //this will store the OUI prefixes with : instead of - for easy comparison	
	if(file != NULL){
		char currentline[100]; //set a maximum size for the line
		while(fgets(currentline, sizeof currentline, file) != NULL){ //read file line-by-line
			strcpy(OuiLineArray[loopcount], currentline); //puts current line into an array for later manipulation
			loopcount++;
		}
			
		fclose(file);
	}
	else{
		printf("Error opening file OUI file\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < num_OUI_lines; i++){ //This does not seem to scan text past a space character
		sscanf(OuiLineArray[i], "%s\t%[^0123456789\n]", OuiAdd, OuiName); //reads OuiLineArray and extracts the Address prefix and the vendor name
		strcat(OuiName, "\0"); //add a null terminator to OuiName
		memcpy(OuiAddressArray[i], OuiAdd, 9);
		memcpy(OuiNameArray[i], OuiName, 90);	
		for(int j = 0; j <8; j++){ //this loop makes a OUI prefix array with : instead of - so that it can be compared to MAC addresses easily
			if(isalpha(OuiAddressArray[i][j]) || isdigit(OuiAddressArray[i][j])) {
				strcpy(&Comparison_oui_array[i][j], &OuiAddressArray[i][j]);
			}
			else{
				Comparison_oui_array[i][j] = ':';
			}
		}
	}
	
	
	for(int i = 0; i<num_unique_addresses; i++){
		int flag =0;
		for(int j = 0; j<num_OUI_lines; j++){
			if(strncasecmp(arrayOfAddresses[i], Comparison_oui_array[j], 8)==0){ //case insensitve compaison between first 8 characters of MAC address and vendor prefix
				memcpy(transmitted_vendor_list[i], OuiNameArray[j], 90);
				memcpy(transmitted_address_prefixes[i], arrayOfAddresses[i], 8);
				strcat(transmitted_address_prefixes[i], "\0");
				flag =1;
				break;
			}
		}
		if(flag==0){
			memcpy(transmitted_vendor_list[i], "UNKNOWN-VENDOR\0", 15); //if the MAC address doesn't match any vendor, it is named UNKNOWN-VENDOR
			memcpy(transmitted_address_prefixes[i], "??:??:??\0", 9); //if MAC address unknown fill with ??
		}
	}

	int duplicates = 0; //int to count how many duplicates we remove
	for(int i = 0; i < num_unique_addresses; i++){
		for(int j =i +1; j <num_unique_addresses; j++){
			if(strcmp(transmitted_vendor_list[i], transmitted_vendor_list[j]) == 0){
				arrayOfBytes[i] = arrayOfBytes[i] + arrayOfBytes[j]; //sums bytes from duplicate vendors
				arrayOfBytes[j] = 0;
				for(int k = j ; k <num_unique_addresses; k++){ //this removes duplicates and fills the spaces left
					strcpy(transmitted_vendor_list[k], transmitted_vendor_list[k+1]);
					arrayOfBytes[k] = arrayOfBytes[k+1];
					strcpy(transmitted_address_prefixes[k], transmitted_address_prefixes[k+1]);
				}
				duplicates++;
			}
		}
	}
	num_unique_addresses = num_unique_addresses - duplicates; //reduce the number of unique adddresses by the count of how many duplicated there were
	sort_func_OUI(arrayOfBytes, arrayOfAddresses, transmitted_vendor_list, num_unique_addresses, transmitted_address_prefixes);
}

/*
	Sorts vendor name, address and bytes when an OUI file is provided.
	Prints this sorted data to stdout.
	Returns nothing.
*/
void sort_func_OUI(long arrayOfBytes[], char arrayOfAddresses[][18], char transmitted_vendor_list[][90], int num_unique_addresses, char transmitted_address_prefixes[][9]){
	char transmitted_info_line[num_unique_addresses][109];
	int pid = fork();
	if(pid<0){
		printf("Error forking\n");
		printf("%i\n", pid);
		_exit(EXIT_FAILURE);
	}
	else if(pid == 0 ){
		FILE *vbd = fopen("vendor_byte_data_file", "w");
		for(int i =0; i<num_unique_addresses; i++){
			char str[10];
			sprintf(str, "%li", arrayOfBytes[i]);
			memcpy(transmitted_info_line[i], transmitted_address_prefixes[i],9);
			strcat(transmitted_info_line[i], "\t");
			strcat(transmitted_info_line[i], transmitted_vendor_list[i]);
			strcat(transmitted_info_line[i], "\t"); //seperates vendor name and bytes
			strcat(transmitted_info_line[i], str);
			strcat(transmitted_info_line[i], "\n");	
			fwrite(transmitted_info_line[i], sizeof(char), strlen(transmitted_info_line[i]), vbd);
		}
		fclose(vbd);
		char* sort_arg_array[] = {"", "-g", "-f", "-r", "-o", "vendor_byte_data_file", "-t\t", "-k3", "vendor_byte_data_file", NULL}; 		
		execv("/usr/bin/sort", sort_arg_array); //NOTE INCLUDING LC_ALL=C make duplicates sort properly but not others
		exit(1);
	}
	else if(pid >0){
		wait(NULL);
		FILE *vbdata;
		vbdata = fopen("vendor_byte_data_file", "r");
		if(vbdata != NULL){
			char currentline[110]; //set a maximum size for the line
			while(fgets(currentline, sizeof currentline, vbdata) != NULL){ //read file line-by-line
				fputs(currentline, stdout);				
			}
		}	
		fclose(vbdata);
	}
	else{
		printf("Unable to open vendor_bytes_data_file\n");
	}	
}

/*
	Sorts vendor name, address and bytes when an OUI file is not provided.
	Prints this sorted data to stdout.
	Returns nothing.
*/
void sort_func_nOUI(long arrayOfBytes[], char arrayOfAddresses[][18], int num_unique_addresses){
	char transmitted_info_line[num_unique_addresses][109];
	int pid = fork();
	if(pid<0){
		printf("Error forking\n");
		printf("%i\n", pid);
		_exit(EXIT_FAILURE);
	}
	else if(pid == 0 ){
		FILE *vbd = fopen("vendor_byte_data_file", "w");
		for(int i =0; i<num_unique_addresses; i++){
			char str[10];
			sprintf(str, "%li", arrayOfBytes[i]);
			memcpy(transmitted_info_line[i], arrayOfAddresses[i], 18);
			strcat(transmitted_info_line[i], "\t");
			strcat(transmitted_info_line[i], str);
			strcat(transmitted_info_line[i], "\n");	
			fwrite(transmitted_info_line[i], sizeof(char), strlen(transmitted_info_line[i]), vbd);
		}
		fclose(vbd);
		char* sort_arg_array[] = {"", "-g", "-f", "-r", "-o", "vendor_byte_data_file", "-t\t", "-k2", "vendor_byte_data_file", NULL}; 	
		execv("/usr/bin/sort", sort_arg_array); //NOTE INCLUDING LC_ALL=C make duplicates sort properly but not others
		exit(1);
	}
	else if(pid >0){
		wait(NULL);
		FILE *vbdata;
		vbdata = fopen("vendor_byte_data_file", "r");
		if(vbdata != NULL){
			char currentline[110]; //set a maximum size for the line
			while(fgets(currentline, sizeof currentline, vbdata) != NULL){ //read file line-by-line
				fputs(currentline, stdout);				
			}
		}	
		fclose(vbdata);
	}
	else{
		printf("Unable to open vendor_bytes_data_file\n");
	}	
}
