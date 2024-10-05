#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int addUser(void){
	char userInput[1023];
	char comma = ',';
	char newLine = '\n';
	FILE* writer;
	
	// Open and verify file stream
	if(!(writer = fopen("directory.txt", "r+"))){
		printf("ERROR: Unable to open directory.txt\n", writer);
		return 1;
	}
	
	// Move file pointer to the end of the file to add on to the data
	fseek(writer, 0, SEEK_END);
	
	// Get the first name and write it to the document
	printf("Enter the user's first name: ");
	fgets(userInput, 1023, stdin);
	for(int i=0; userInput[i] != '\n'; i++){
		fwrite(&userInput[i], sizeof(char), 1, writer);
	}
	// Write a comma to the document
	fwrite(&comma, sizeof(char), 1, writer);
	
	// Get and write last name
	printf("Enter the user's last name: ");
	fgets(userInput, 1023, stdin);
	for(int i=0; userInput[i] != '\n'; i++){
		fwrite(&userInput[i], sizeof(char), 1, writer);
	}
	fwrite(&comma, sizeof(char), 1, writer);
	
	// Get and write position
	printf("Enter the user's position: ");
	fgets(userInput, 1023, stdin);
	for(int i=0; userInput[i] != '\n'; i++){
		fwrite(&userInput[i], sizeof(char), 1, writer);
	}
	fwrite(&comma, sizeof(char), 1, writer);
	
	// Get and write employee ID
	printf("Enter the user's employee ID: ");
	fgets(userInput, 1023, stdin);
	for(int i=0; userInput[i] != '\n'; i++){
		fwrite(&userInput[i], sizeof(char), 1, writer);
	}
	fwrite(&comma, sizeof(char), 1, writer);
	
	// Get and write phone number
	printf("Enter the user's phone number: ");
	fgets(userInput, 1023, stdin);
	for(int i=0; userInput[i] != '\n'; i++){
		fwrite(&userInput[i], sizeof(char), 1, writer);
	}
	
	// Write a new line character to the directory
	fwrite(&newLine, sizeof(char), 1, writer);
	
	// Close the file stream
	fclose(writer);
}

int removeUser(void){
	char userInput[1023];
	FILE* oldDirectory;
	FILE* newDirectory;
	char transfer;
	int i=1,
		entry;
	
	if(!(oldDirectory = fopen("directory.txt", "r"))){
		printf("ERROR: reader for directory.txt failed to open\n");
		return 1;
	}
	if(!(newDirectory = fopen("directory_copy.txt", "w+"))){
		printf("ERROR: writer for directory.txt failed to open\n");
		return 1;
	}
	
	//Ask the user which entry they would like to remove
	printf("Enter the # of the user you would like to remove: ");
	fgets(userInput, 1023, stdin);
	while(!(entry = atoi(userInput))){
		printf("Please enter a valid number: ");
		fgets(userInput, 1023, stdin);
	}
	
	while(i < entry){
		fread(&transfer, sizeof(char), 1, oldDirectory);
		fwrite(&transfer, sizeof(char), 1, newDirectory);
		if(transfer == '\n'){
			i++;
		}
	}
	
	// This makes a broken file DO NOT USE
	//while(transfer != EOF){
		//fread(&transfer, sizeof(char), 1, oldDirectory);
		//fwrite(&transfer, sizeof(char), 1, newDirectory);
	//}
	
	fclose(oldDirectory);
	fclose(newDirectory);
	
}

int modifyUser(void){
	
}

int edit(void){
	char userInput[1023];
	int choice,
		quit = 0;
	
	// Editing Menu
	while(!quit){
		printf( "Please Choose An Option\n"
				"1) Add User\n"
				"2) Remove User\n"
				"3) Modify User\n"
				"4) Exit\n");
				
		fgets(userInput, 1023, stdin);
		
		// Get user input and verify it's validity
		while(!(choice = atoi(userInput))>0 && choice < 5){
			printf("Please enter a valid number: \n");
			fgets(userInput, 1023, stdin);
		}
		
		// Run function based on user choice
		switch(choice){
			case 1:
				addUser();
				break;
			case 2:
				removeUser();
				break;
			case 3:
				modifyUser();
				break;
			case 4:
				quit = 1;
				break;
		}
	}
	return 0;
}

int view(void){	
	FILE* directory;
	char reader;
	char buffer[1023];
	int fileSize = 20,
		i = 0,
		entries = 1,
		stringSize = 0,
		newEntry = 1;
	
	// Print the directory menu
	printf(	"   #\t"
			"   Last Name\t"
			"  First Name\t"
			"    Position\t"
			" Employee ID\t"
			"Phone Number\n"
			"----\t"
			"------------\t"
			"------------\t"
			"------------\t"
			"------------\t"
			"------------\n");
			
	// Verify the file stream opened
	if(!(directory = fopen("directory.txt", "r"))){
		printf("Error opening directory.txt\n");
		printf("Error: %d\n", errno);
		return 1;
	}
	
	// Get the directory file size
	fseek(directory, 0, SEEK_END);
	fileSize = ftell(directory);
	fseek(directory, sizeof(char)*3, SEEK_SET);
	
	
	
	while(ftell(directory) != fileSize){
		if(newEntry){
			printf("%3.3d)\t", entries);
			entries++;
			newEntry = 0;
		}
		do{
			fread(&reader, sizeof(char), 1, directory);
			buffer[i] = reader;
			i++;
		}while(reader != ',' && reader != '\n' && i<1022);
		
		buffer[i-1] = '\0';
		printf("%12.12s", buffer);
		
		if(reader == ','){
			printf("\t");
		}
		else if(reader == '\n'){
			newEntry = 1;
			printf("\n", entries);
		}
		else{
			printf("\nUNDEFINED BEHAVIOR\n");
		}
		
		i=0;
	}
	
	fclose(directory);
}

int directory(void){
	char userInput[1023];
	int choice = 0,
		quit = 0;
	
	printf("\nWelcome to the Employee Directory!\n");
	
	while(!quit){
		printf(	"Please choose an option:\n"
				"1) Edit Directory\n"
				"2) View Directory\n"
				"3) Exit Directory\n");
		
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
