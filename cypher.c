/*
 * Author: Hunter Van Horn
 * Basic Cypher: Encrypt and Decrypt Files w/ a user provided key
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
 
/*
 * This function takes a user's key, input file, and output file then
 * encrypts the contents of the input file into the output file using
 * the provided key.
 */
int encryption(void){
	char plaintext[1023]; // Used for all user input
	char reader; // Used when reading/writing to files
	FILE* readFile; // File to read from
	FILE* writeFile; // File to write to
	int fileSize = 0, // Tracks the length of the input file
		keyLength = 0, // Tracks the length of the key
		i = 0; // Used for iteration
	
	// Request the key
	printf("Please enter the encryption key: ");
	printf("\n");
	if(fgets(plaintext, 1023, stdin)){
		plaintext[strcspn(plaintext, "\n")] = 0; // remove newline
	}
	
	// Check if entered key is within the accepted ASCII values
	while(plaintext[i] != '\0'){
		if(tolower(plaintext[i]) < 97 || 
				tolower(plaintext[i]) > 122){
			// Upon invalid key reset to check from the beginning
			i = 0;
			keyLength = 0;
			// Request a new key from the user
			printf("This is not a valid key, please enter a different"
			" key (letters a-z are accepted): ");
			if(fgets(plaintext, 1023, stdin)){
				plaintext[strcspn(plaintext, "\n")] = 0;//remove newline
				printf("Key: %s\n", plaintext); // Testing input
			}
		}
		else{
			i++;
			keyLength++;
		}
	}
	
	//Initialize the key array based on user input
	int key[i];
	
	for(i=0; plaintext[i] != '\0'; i++){
		key[i] = tolower(plaintext[i])-97;
	}
	
	// Open the file to read from
	printf("Enter the name of the file you want to encrypt: \n");
	fgets(plaintext, 1023, stdin);
	plaintext[strcspn(plaintext, "\n")] = 0; // remove the newline
	
	// Verify the file opened
	if(readFile = fopen(plaintext, "r")){
		//Get the size of the input file
		fseek(readFile, 0, SEEK_END);
		fileSize = ftell(readFile);
		fseek(readFile, 0, SEEK_SET);
		fileSize = fileSize/(int)sizeof(char);
	}
	else{
		printf("Error Opening File: %s\n", plaintext);
		printf("Error: %d (%s)\n", errno, strerror(errno));
		return 1;
	}
	
	// Open the file to write to
	printf("Enter the name of the file you want to output your"
			" encryption to: \n");
	fgets(plaintext, 1023, stdin);
	plaintext[strcspn(plaintext, "\n")] = 0; // remove the newline
	
	//Verify the file opened
	if(!(writeFile = fopen(plaintext, "w"))){
		printf("Error Opening File %s\n", plaintext);
		printf("Error: %d (%s)\n", errno, strerror(errno));
		return 1;
	}
	
	for(i=0; i<fileSize; i++){
		// Read in a char from the requested file
		fread(&reader, sizeof(char), 1, readFile);
		if(reader > 31 && reader < 127){ // Only convert accepted values
			if(reader+key[i%keyLength] > 126){
				reader -= 95; 
			}
			reader += key[i%keyLength]; // Shift
		}
		// Write the encrypted char out to the requested file
		fwrite(&reader, sizeof(char), 1, writeFile);
	}
	
	// Close Files
	fclose(readFile);
	fclose(writeFile);
	
	return 0;
}

/*
 * This Function takes a user's key, input file, and output file then
 * decrypts the contents of the input file and puts it into the output
 * file using the key.
 */
int decryption(void){
	char cyphertext[1023]; // For all user input
	char reader; // For reading/writing to files
	FILE* readFile; // File to read from
	FILE* writeFile; // File to write to
	int fileSize = 0, // Tracks the length of the input file
		keyLength = 0, // Tracks the length of the key
		i = 0; // Used for iteration
	
	// Request a key from the user
	printf("Please enter your decryption key: \n");
	if(fgets(cyphertext, 1023, stdin)){
		cyphertext[strcspn(cyphertext, "\n")] = 0; // remove newline
	}
	
	// Count the key size and verify all chars are acceptable
	while(cyphertext[i] != '\0'){
		// Check if key value is acceptable
		if(tolower(cyphertext[0]) < 97 ||
				tolower(cyphertext[0]) > 122){
			// Reset tracking values if an invlid key was submitted
			i = 0;
			keyLength = 0;
			// Request a user for the new key
			printf("This is not a valid key, please enter a different"
			" key: ");
			if(fgets(cyphertext, 1023, stdin)){
				//remove newline char
				cyphertext[strcspn(cyphertext, "\n")] = 0;
			}
		}
		else{
			i++;
			keyLength++;
		}
	}
	
	// Initialize the key array
	int key[i];
	// Calculate a shift value for each key value
	for(i=0; i<keyLength; i++){
		key[i] = tolower(cyphertext[i])-97;
	}
	
	// Request the file to read from
	printf("Please enter the file you would like to decrypt: \n");
	if(fgets(cyphertext, 1023, stdin)){
		cyphertext[strcspn(cyphertext, "\n")] = 0; // remove the newline
	}
	
	// Verify the file opened
	if(readFile = fopen(cyphertext, "r")){
		fseek(readFile, 0, SEEK_END);
		fileSize = ftell(readFile);
		fseek(readFile, 0, SEEK_SET);
	}
	else{
		printf("Error Opening File: %s\n", cyphertext);
		printf("Error: %d (%s)\n", errno, strerror(errno));
		return 1;
	}
	
	// Request a file to write to
	printf("Please enter the name of the file you want to output your"
			" decryption to: \n");
	if(fgets(cyphertext, 1023, stdin)){
		cyphertext[strcspn(cyphertext, "\n")] = 0; // remove the newline
	}
	
	// Verify the file opened
	if(!(writeFile = fopen(cyphertext, "w"))){
		printf("Error Opening File: %s\n", cyphertext);
		printf("Error: %d (%s)\n", errno, strerror(errno));
		return 1;
	}
	
	// Decrypt File
	for(i=0; i<fileSize; i++){
		// Read chars from requested file
		fread(&reader, sizeof(char), 1, readFile);
		//printf("Before shift: %c, %d\t", reader, reader);
		if(reader > 31 && reader < 127){
			reader -= key[i%keyLength];
			//printf("After shift: %c, %d\t", reader, reader);
			if(reader < 32){
				reader += 95;
			}
			//printf("Final: %c, %d\n", reader, reader);
		}
		// Write chars to requested file
		fwrite(&reader, sizeof(char), 1, writeFile);
	}
	
	// Close Files
	fclose(readFile);
	fclose(writeFile);
	
	return 0;
}
	 
int cypher(void){
	char userInput[1023]; // For all user input
	int choice, // User's Menu choice
		quit = 0; // Tracks when the user quits through the menu
	
	// Welcome Message
	printf("Welcome to the cypher program.\n\n");
	
	// Run menu until user quits
	while(!quit){
		printf("Please choose an option:\n");
		printf("1) Encrypt Text\n");
		printf("2) Decrypt Text\n");
		printf("3) Main Menu\n");

		fgets(userInput, 1023, stdin);

		while(!((choice = atoi(userInput))>0 && choice<4)){
			printf("Please enter a valid number: ");
			printf("\n");
			fgets(userInput, 1023, stdin);
		}
			
		switch(choice){
			case 1:
				encryption();
				break;
			case 2:
				decryption();
				break;
			case 3:
				quit = 1;
				break;
		}
	}
	
	return 0;
}



