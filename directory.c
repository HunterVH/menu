#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int edit(void){
	printf("Editing\n");
}

int view(void){	
	FILE* directory;
	char reader;
	char buffer[1023];
	int fileSize,
		i = 0,
		iter = 0,
		leadIter = 0,
		stringSize = 0;
	
	printf(	"   Last Name\t"
			"  First Name\t"
			"    Position\t"
			" Employee ID\t"
			"Phone Number\n");
	
	if(!(directory = fopen("directory.txt", "r"))){
		printf("Error opening directory.txt\n");
		printf("Error: %d\n", errno);
		return 1;
	}
	
	// Get the directory file size
	fseek(directory, 0, SEEK_END);
	fileSize = ftell(directory);
	fseek(directory, 0, SEEK_SET);
	
	while(ftell(directory) != fileSize){
		do{
			fread(&reader, sizeof(char), 1, directory);
			buffer[i] = reader;
			leadIter += sizeof(char);
			i++;
		}while(reader != ',' && reader != '\n' && i<1022);
		
		buffer[i-1] = '\0';
		printf("%12.12s", buffer);
		
		if(reader == ','){
			printf("\t");
		}
		else if(reader == '\n'){
			printf("\n");
		}
		else{
			printf("\nUNDEFINED BEHAVIOR\n");
		}
		
		i=0;
		iter = leadIter+1;
	}
	
	fclose(directory);
}

int directory(void){
	char userInput[1023];
	int choice = 0,
		quit = 0;
	
	printf("\nWelcome to the Employee Directory!\n");
	
	while(!quit){
		printf("Please choose an option:\n");
		printf("1) Edit Directory\n");
		printf("2) View Directory\n");
		printf("3) Exit Directory\n");
		
		fgets(userInput, 1023, stdin);
		
		while(!(choice = atoi(userInput)) > 0 && choice < 4){
			printf("Please enter a valid number: \n");
			fgets(userInput, 1023, 0);
		}
		
		switch(choice){
			case 1:
				edit();
				break;
			case 2:
				view();
				break;
			case 3:
				quit = 1;
				break;
		}
	}
	
	return 0;
}
