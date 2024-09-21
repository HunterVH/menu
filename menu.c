#include <stdlib.h>
#include <stdio.h>
#include "cypher.h"

int main(int argc, char *argv[]){
	char userInput[1023];
	int choice = 0;
	int quit = 0;
	
	printf("Welcome to the C Multitool please select an option: ");
	
	while(!quit){
		printf("\n1) Encryption/Decryption Tool\n");
		printf("2) Quit\n");
		
		fgets(userInput, 1023, stdin);
		
		while((choice = atoi(userInput)) < 1 || choice > 2){
			printf("Please enter a valid number\n");
			fgets(userInput, 1023, stdin);
		}
		
		switch(choice){
			case 1:
				cypher();
				break;
			case 2:
				quit = 1;
				break;
		}
	}
	
	return 0;
}
