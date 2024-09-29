#include <stdio.h>
#include <stdlib.h>

int edit(){
	printf("Editing\n");
}

int view(){
	printf("Viewing\n");
}

int directory(){
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
