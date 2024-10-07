/*
 * Author: Hunter Van Horn
 * CS5740 - SetUID
 * 
 * This program allows a user to interact with a directory.
 * To modify the directory the user must enter a password.
 * 
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "menu.h"
#include "permission.h"
#include "password.h"

int addUser(void){
	char userInput[1023];
	char comma = ',';
	char newLine = '\n';
	FILE* writer;
	
	// Open and verify file stream
	if(!(writer = fopen("directory.txt", "a"))){
		printf("ERROR: Unable to open directory.txt\n", writer);
		return 1;
	}
	
	// Get the first name and write it to the document
	printf("Enter the user's last name: ");
	fgets(userInput, 1023, stdin);
	for(int i=0; userInput[i] != '\n'; i++){
		fwrite(&userInput[i], sizeof(char), 1, writer);
	}
	// Write a comma to the document
	fwrite(&comma, sizeof(char), 1, writer);
	
	// Get and write last name
	printf("Enter the user's first name: ");
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
	
	return 0;
}

int removeUser(void){
	char userInput[1023];
	FILE* oldDirectory;
	FILE* newDirectory;
	char transfer;
	int i=1,
		entry;
	
	// Open directory for reading
	if(!(oldDirectory = fopen("directory.txt", "r"))){
		printf("ERROR: reader for directory.txt failed to open\n");
		return 1;
	}
	// Open copy for writing and reading, create file if non-existent
	if(!(newDirectory = fopen("directory_copy.txt", "w+"))){
		printf("ERROR: writer for directory.txt failed to open\n");
		return 1;
	}
	
	//Ask the user which entry they would like to remove
	printf("Enter the # of the user you would like to remove: ");
	fgets(userInput, 1023, stdin);
	while(!(entry = atoi(userInput)) || (entry<0)){
		printf("Please enter a valid number: ");
		fgets(userInput, 1023, stdin);
	}
	printf("%d\n", entry);
	
	//Write all entries up to the requested deleted line
	while(i < entry && !feof(oldDirectory)){
		fread(&transfer, sizeof(char), 1, oldDirectory);
		fwrite(&transfer, sizeof(char), 1, newDirectory);
		if(transfer == '\n'){
			i++;
		}
	}
	
	// Test to see if their entry was valid
	if(feof(oldDirectory)){
		printf("ERROR: Your Entry was not found in the directory.txt\n");
		return 1;
	}
	
	// Skip Deleted Line
	do{
		fread(&transfer, sizeof(char), 1, oldDirectory);
	}while(transfer != '\n');
	
	fread(&transfer, sizeof(char), 1, oldDirectory);
	// Write all entries after the deleted line
	while(!feof(oldDirectory)){
		fwrite(&transfer, sizeof(char), 1, newDirectory);
		fread(&transfer, sizeof(char), 1, oldDirectory);
	}
	
	// delete the old directory file
	remove("directory.txt");
	// rename the edited directory file
	rename("directory_copy.txt", "directory.txt");
	
	// Close file streams
	fclose(oldDirectory);
	fclose(newDirectory);
	
	return 0;
}

int modifyUser(void){
	char userInput[1023];
	FILE* oldDirectory;
	FILE* newDirectory;
	char transfer;
	int entry,
		i = 1;
	
	// Repeat the modification prompt until the user exits
	while(1){
		// Request which entry the user would like to modify
		printf( "Please enter the entry # you would like to edit(enter "
				"-1 to exit): ");
		
		// Get user entry input
		fgets(userInput, 1023, stdin);
		
		// Verify a valid number
		while(!(entry=atoi(userInput)) || entry < -1){
			printf("Please enter a valid number: ");
			fgets(userInput, 1023, stdin);
		}
		// Exit the program based on user input
		if(entry == -1){
			break;
		}
		// Open file streams
		else{
			// Verify directory.txt can be opened
			if(!(oldDirectory = fopen("directory.txt", "r"))){
				printf("ERROR: Could not open directory.txt for reading");
				return 1;
			}
			// Verify directory_modified.txt can be opened/created
			if(!(newDirectory = fopen("directory_modified.txt", "w+"))){
				printf("ERROR: Could not open directory_modified.txt");
			}
		}
		
		i=1;
		while(i < entry && !feof(oldDirectory)){
			fread(&transfer, sizeof(char), 1, oldDirectory);
			fwrite(&transfer, sizeof(char), 1, newDirectory);
			if(transfer == '\n'){
				i++;
			}
		}
		
		fread(&transfer, sizeof(char), 1, oldDirectory);
		
		// Check if the end of the file was reached
		if(feof(oldDirectory)){
			// Close file streams
			fclose(oldDirectory);
			fclose(newDirectory);
			
			// Remove the modified directory file
			remove("directory_modified.txt");
			
			printf("\nERROR: No such entry exists\n");
			return 1;
		}
		else{
		}
		
		
		// Ask the user which value they would like to modify
		printf(	"Please choose a value to modify\n"
				"1) Last Name\n"
				"2) First Name\n"
				"3) Position\n"
				"4) Employee ID\n"
				"5) Phone Number\n");
				
		fgets(userInput, 1023, stdin);
		entry = atoi(userInput);
		
		while(!((entry = atoi(userInput))>0 && entry < 6)){
			printf("Please enter a valid option(1-5): ");
			fgets(userInput, 1023, stdin);
		}
		
		// Copy all data up to the value the user wants to replace
		i=1;
		// Protects formatting against a blank first value
		if(transfer == ','){
			entry-=1;
		}
		
		while(i<entry){
			printf("I: %d\n", i);
			printf("WRITING: %c\n", transfer);
			fwrite(&transfer, sizeof(char), 1, newDirectory);
			fread(&transfer, sizeof(char), 1, oldDirectory);
			if(transfer == ','){
				printf("\n");
				i++;
			}
		}
		
		// Adds the comma before the user's value
		if(i>1){
			printf("WRITING2: %c", transfer);
			fwrite(&transfer, sizeof(char), 1, newDirectory);
			fread(&transfer, sizeof(char), 1, oldDirectory);
		}
		
		// Read past the data that needs to be overwritten
		//fread(&transfer, sizeof(char), 1, oldDirectory);
		while(transfer != '\n' && transfer != ','){
			printf("SKIPPING: %c", transfer);
			fread(&transfer, sizeof(char), 1, oldDirectory);
		}
		printf("\n");
		//fwrite(&transfer, sizeof(char), 1, newDirectory);
		
		// Request the necessary information from the user
		switch(i){
			case 1:
				printf("Enter the new Last Name value: ");
				break;
			case 2:
				printf("Enter the new First Name value: ");
				break;
			case 3:
				printf("Enter the new Position value: ");
				break;
			case 4:
				printf("Enter the new Employee ID value: ");
				break;
			case 5:
				printf("Enter the new Phone Number value: ");
				break;
			default:
				printf("Enter your replacement value: ");
		}
		
		// Get the user's new input value
		fgets(userInput, 1023, stdin);
		
		// Write the user's new input
		for(i=0; userInput[i] != '\n'; i++){
			printf("USERINPUT: %c", userInput[i]);
			fwrite(&userInput[i], sizeof(char), 1, newDirectory);
		}

		// Write the remaining contents of the old directory to the
		// new directory
		while(!feof(oldDirectory)){
			printf("WRITINGF: %c\n", transfer);
			fwrite(&transfer, sizeof(char), 1, newDirectory);
			fread(&transfer, sizeof(char), 1, oldDirectory);
		}
		
		// Close file streams
		fclose(oldDirectory);
		fclose(newDirectory);
		
		// Remove the old directory
		remove("directory.txt");
		// Rename the modified directory
		rename("directory_modified.txt", "directory.txt");
	}
	
	return 0;
}



int edit(void){
	char userInput[1023];
	int choice,
		quit = 0;
		
	printf("Please enter the directory password: ");
	fgets(userInput, 1023, stdin);
	
	if(passwordCheck(userInput)){
		return 1;
	}
	
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
	
	defaultPermissions();
	
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
		return 1;
	}
	
	// Get the directory file size
	fseek(directory, 0, SEEK_END);
	fileSize = ftell(directory);
	fseek(directory, 0, SEEK_SET);
	
	
	// Read the directory and output fromatted content to the user
	while(ftell(directory) != fileSize){
		if(newEntry){ // If at the beginning of a line print the line #
			printf("%4.4d\t", entries);
			entries++;
			newEntry = 0;
		}
		do{ // Read user values into the buffer, up to 1022 bytes
			fread(&reader, sizeof(char), 1, directory);
			buffer[i] = reader;
			i++;
		}while(reader != ',' && reader != '\n' && i<1022);
		
		// If an entry fills the buffer iterate the file stream to the
		// next value as to avoid undefined behavior
		if(i==1022){
			while(reader != ',' && reader != '\n'){
				fread(&reader, sizeof(char), 1, directory);
			}
		}
		
		buffer[i-1] = '\0'; // Add a string terminater to end of string
		printf("%12.12s", buffer); // Print formatted buffer value
		
		if(reader == ','){ // Enter a tab between directory values
			printf("\t");
		}
		else if(reader == '\n'){ // Enter a NL if at the end of line
			newEntry = 1; // Identify moving to the next line
			printf("\n", entries);
		}
		else{
			printf("\nERROR: UNKNOWN VALUE PRESENT IN DIRECTORY\n");
			return 1;
		}
		
		i=0;
	}
	
	fclose(directory);
	
	return 0;
}

int changePass(void){
	char userInput[1023];
	
	printf("Please enter the current password: ");
	fgets(userInput, 1023, stdin);
	userInput[1023] = '\n';
	
	if(passwordCheck(userInput)){
		return 1;
	}
	else{
		printf("Please enter the new password: ");
		fgets(userInput, 1023, stdin);
	
		// In case the user inputs more than 1022 characters
		userInput[1023] = '\n';
	
		passwordChange(userInput);
	}
	return 0;
}

int directory(void){
	char userInput[1023];
	int choice = 0,
		quit = 0;
	
	// Welcome message
	printf("\nWelcome to the Employee Directory!\n");
	
	// Menu persists until user exits
	while(!quit){
		printf(	"Please choose an option:\n"
				"1) Edit Directory\n"
				"2) View Directory\n"
				"3) Change Directory Password\n"
				"4) Exit Directory\n");
		
		fgets(userInput, 1023, stdin);
		
		// Input validation
		while(!(choice = atoi(userInput)) > 0 && choice < 5){
			printf("Please enter a valid number: \n");
			fgets(userInput, 1023, 0);
		}
		
		// run function based on user input, or quit the menu
		switch(choice){
			case 1:
				edit();
				break;
			case 2:
				view();
				break;
			case 3:
				changePass();
				break;
			case 4:
				quit = 1;
				break;
		}
	}
	
	return 0;
}
