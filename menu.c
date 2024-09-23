#include <stdlib.h>
#include <stdio.h>
#include "cypher.h"

int main(int argc, char *argv[]){
	char userInput[1023]; // Used to take all user input
	int choice = 0, // User's menu choice
		quit = 0; // Tracks when the user quit's through the menu
	
	// Welcom Message
	printf("Welcome to the C Multitool please select an option: ");
	
	// Run menu until user quits
	while(!quit){
		printf("\n1) Encryption/Decryption Tool\n");
		printf("2) Quit\n");
		
		// Get user's menu selection
		fgets(userInput, 1023, stdin);
		while((choice = atoi(userInput)) < 1 || choice > 2){
			printf("Please enter a valid number\n");
			fgets(userInput, 1023, stdin);
		}
		
		// Run what the user chooses
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
