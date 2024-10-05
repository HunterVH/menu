#include <stdio.h>

int password(char* pass){
	char password[] = "password";
	
	for(int i=0; password[i] != '\0'; i++){
		if(password[i] != *pass){
			return 1;
		}
		pass++;
	}
	
	return 0;
}



/*
int encryption(char* pass){
	char charKey[3] = "nwt";
	FILE* readFile;
	
	
	//Initialize the key array based on user input
	int key[3];
	
	for(i=0; plaintext[i] != '\0'; i++){
		key[i] = tolower(charKey[i])-97;
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
*/
