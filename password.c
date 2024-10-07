#include <stdio.h>
#include <unistd.h>
#include "permission.h"

#define KEYLENGTH 3

int passwordEncrypt(char* pass){
	char charKey[KEYLENGTH] = "wos";
	char reader;
	int key[KEYLENGTH],
		i;
		
	// Create the key
	for(i=0; i<KEYLENGTH; i++){
		key[i] = charKey[i]-97;
	}
	
	i=3;
	while(*pass != '\n'){
		if(*pass > 31 && *pass < 127){ // Only convert accepted values
			if(*pass+key[i%KEYLENGTH] > 126){
				*pass -= 95; 
			}
			*pass += key[i%KEYLENGTH]; // Shift
		}
		pass++;
		i++;
	}
	
	return 0;
}

int passwordCheck(char* userPass){
	FILE* passwordReader;
	char realPass;
	char password[] = "password";
	
	passwordEncrypt(userPass);
	
	rootPermissions();
	if(!(passwordReader = fopen("password.txt", "r"))){
		printf("ERROR: Unable to verify password\n");
		defaultPermissions();
		return 1;
	}
	
	// Compare user password to stored password
	while(!feof(passwordReader) && *userPass != '\n'){
		fread(&realPass, sizeof(char), 1, passwordReader);
		if(realPass != *userPass){
			printf("\nERROR: Incorrect Password\n");
			defaultPermissions();
			return 1;
		}
		userPass++;
	}
	
	// Ensure that both pointers reached the end
	fread(&realPass, sizeof(char), 1, passwordReader);
	if(!(feof(passwordReader) && *userPass == '\n')){
		printf("\nERROR: Incorrect Password\n");
		defaultPermissions();
		return 1;
	}
	
	return 0;
}

int passwordChange(char* pass){
	FILE* writePassword;
	passwordEncrypt(pass);
	
	if(!(writePassword = fopen("password.txt", "w+"))){
		printf("ERROR: Failed to update password\n");
		defaultPermissions();
		return 1;
	}
	
	while(*pass != '\n'){
		fwrite(pass, sizeof(char), 1, writePassword);
		pass++;
	}
	
	fclose(writePassword);
	
	defaultPermissions();
	
	return 0;
}




