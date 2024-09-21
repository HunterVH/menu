/*
 * Author: Hunter Van Horn
 * Basic Cypher: Take user input and produce an encrypted text
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
 
int encryption(void){
	char* fileName;
	char plaintext[1023];
	char reader;
	FILE* readFile;
	FILE* writeFile;
	int shift = 0,
		fileSize = 0;
	//int i = 0;
	
	printf("Please enter the encryption key: ");
	printf("\n");
	
	if(fgets(plaintext, 1023, stdin)){
		plaintext[strcspn(plaintext, "\n")] = 0; // remove the newline
	}
	
	// If entered key is not within the accepted ASCII values
	while(tolower(plaintext[0]) < 97 || tolower(plaintext[0]) > 122){
		printf("This is not a valid key, please enter a different"
		" key (letters a-z are accepted): ");
		if(fgets(plaintext, 1023, stdin)){
			plaintext[strcspn(plaintext, "\n")] = 0; // remove the newline
			printf("Key: %s\n", plaintext); // Testing input
		}
	}
	
	// Calculate the substitution shift value
	shift = tolower(plaintext[0])-97;
	
	// Open the file to read from
	printf("Enter the name of the file you want to encrypt: \n");
	fgets(plaintext, 1023, stdin);
	plaintext[strcspn(plaintext, "\n")] = 0; // remove the newline
	readFile = fopen(plaintext, "r"); // Open the file
	
	// Verify the file opened
	if(readFile){
		//Get the size of the input file
		fseek(readFile, 0, SEEK_END);
		fileSize = ftell(readFile);
		fseek(readFile, 0, SEEK_SET);
		fileSize = fileSize/(int)sizeof(char);
	}
	else{
		printf("Error Opening File %s\n", plaintext);
		printf("Error: %d (%s)\n", errno, strerror(errno));
		return 1;
	}
	
	// Open the file to write to
	printf("Enter the name of the file you want to output your"
			" decryption to: \n");
	fgets(plaintext, 1023, stdin);
	plaintext[strcspn(plaintext, "\n")] = 0; // remove the newline
	writeFile = fopen(plaintext, "w");
	
	//Verify the file opened
	if(!writeFile){
		printf("Error Opening File %s\n", plaintext);
		printf("Error: %d (%s)\n", errno, strerror(errno));
		return 1;
	}
	
	for(int i=0; i<fileSize; i++){
		printf("Loop #: %d\n", i);
		fread(&reader, sizeof(char), 1, readFile);
		printf("Letter before change: %c\n", reader);
		if(reader > 31 && reader < 127){ // Only convert accepted values
			// loop first to avoid a negative number
			if(reader+shift > 126){
				printf("looping\n");
				reader -= 95; 
			}
			reader += shift; // Shift
			printf("Letter after shift: %c, %d\n", reader, reader);
		}
		printf("Letter after change: %c\n", reader);
		fwrite(&reader, sizeof(char), 1, writeFile);
	}
	
	fclose(readFile);
	fclose(writeFile);
	
	return 0;
}

int decryption(void){
	char cyphertext[1023];
	int shift = 0;
	int i = 0;
	
	printf("Please enter your decryption key: ");
	printf("\n");
	
	if(fgets(cyphertext, 1023, stdin)){
		cyphertext[strcspn(cyphertext, "\n")] = 0; // remove the newline
		printf("Key: %s\n", cyphertext); // Testing input
	}
	
	// If entered key is not within the accepted ASCII values
	while(cyphertext[0] < 32 || cyphertext[0] > 126){
		printf("This is not a valid key, please enter a different"
		" key: ");
		if(fgets(cyphertext, 1023, stdin)){
			cyphertext[strcspn(cyphertext, "\n")] = 0; // remove the newline
			printf("Key: %s\n", cyphertext); // Testing input
		}
	}
	shift = tolower(cyphertext[0])-97; // Calculate the substitution shift value
		
	printf("Please enter the text you want to encrypt: ");
	printf("\n");
	
	if(fgets(cyphertext, 1023, stdin)){
		cyphertext[strcspn(cyphertext, "\n")] = 0; // remove the newline
		printf("Cyphertext: %s\n", cyphertext); // Output user's text
	}
	
	while(cyphertext[i] != '\0'){
		if(cyphertext[i] > 31 && cyphertext[i] < 127){
			// If the value is outside our accepted ASCII values
			if(cyphertext[i]-shift < 32){ 
				cyphertext[i] = cyphertext[i]+94;
			}
			cyphertext[i] = cyphertext[i]-shift;
		}
		i++;
	}
	
	printf("Plaintext: %s\n", cyphertext);
}
	 
int cypher(void){
	char userInput[1023];
	int choice;
	int quit = 0;
	int i = 0;
	
	printf("Welcome to the cypher program.\n\n");
	
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



