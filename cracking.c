#include <stdio.h>
#include <string.h>
#include <ctype.h>

int topLetters(int letterDistrobution[]){
	// Check if A,E,I,N,O,R,S,T
	int top = 0;

	for(int i=0; i<8; i++){
		if(letterDistrobution[i] == 0 ||
		   letterDistrobution[i] == 4 ||
		   letterDistrobution[i] == 8 ||
		   letterDistrobution[i] == 13 ||
		   letterDistrobution[i] == 14 ||
		   letterDistrobution[i] == 17 ||
		   letterDistrobution[i] == 18 ||
		   letterDistrobution[i] == 19){
			top++;
		}
	}
	if(top > 6){
		printf("Letters Found: %d\n", top);
		return 1;
	}

	printf("Letters Found: %d\n", top);
	return 0;
}

int shiftNum(int letterDistro[]){
	int checkForE = 0;
	// Simple Shift
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			int difference = letterDistro[j]-letterDistro[i];
			if(difference < 0){ difference += 26; }
			printf("  %c | %c | %d\n", letterDistro[i]+65,letterDistro[j]+65,difference);
			if(difference == 4 ||
			   difference == 9 ||
			   difference == 11 ||
			   difference == 10 ||
			   difference == 13 ||
			   difference == 14 ||
			   difference == 15 ||
			   difference == 26){
				checkForE++;
			}
		}
		if(checkForE > 5){
			printf("We found E it is %d:%c\n", letterDistro[i], letterDistro[i]+65);
			return letterDistro[i]-4;
		}
	}

	return 0;
}

int unshift(int shift, char* fileName){
	FILE* fileReader;
	FILE* fileWriter;
	char letter;

	if(!(fileReader = fopen(fileName, "r"))){
		printf("ERROR: Failed to open read file to shift.\n");
		return 1;
	}
	if(!(fileWriter = fopen("ciphertexts/decryption.txt", "a"))){
		printf("ERROR: Failed to open write file to shift.\n");
		return 1;
	}

	while(!feof(fileReader)){
		fread(&letter, sizeof(char), 1, fileReader);
		letter = toupper(letter);
		
		letter -= shift;
		if(letter < 65){
			letter+=26;
		}

		printf("%c",letter);

		fwrite(&letter, sizeof(char), 1, fileWriter);
	}

	printf("\n");

	if((fclose(fileReader))){
		printf("ERROR: Failed to close read file after shift.\n");
		if(!(fclose(fileWriter))){
			printf("ERROR: Failed to close write file after shift.\n");
		}
		return 1;
	}
	if((fclose(fileWriter))){
		printf("ERROR: Failed to close write file after shift.\n");
		return 1;
	}
	
	return 0;
}

int vignereKey(char* fileName){
	int wordFound = 0;
	FILE* fileReader;
	int fileSize = 0;
	int wordLength = 0;
	char letter;
	char word[50];

	if(!(fileReader = fopen(fileName, "r"))){
		printf("Failed to open file reader when finding a vignereKey.'n");
		return 1;
	}

	// Read all characters from the encrypted file into an array
	fseek(fileReader, 0, SEEK_END);
	fileSize = ftell(fileReader);
	fseek(fileReader, 0, SEEK_SET);
	fileSize /= (int)sizeof(char);
	
	char encryptedFile[fileSize];

	for(int i=0; i<fileSize; i++){
		fread(&letter, sizeof(char), 1, fileReader);
		encryptedFile[i] = letter;
	}

	// Find repeated words
	for(int i=0; i<fileSize; i++){
		for(int j=i+1; j<fileSize; j++){
			// When you find two characters that match
			while(encryptedFile[i+wordLength] == encryptedFile[j+wordLength]){
				// Increase word length
				wordLength++;
			}
			// Only count words with more than 4 characters
			if(wordLength >= 3){
				for(int k=0; k<wordLength; k++){
					word[k] = encryptedFile[i+k];
				}

				wordFound = 1;
				break;
			}
			else{
				wordLength = 0;
			}
		}
		// Print out the found word
		if(wordFound){
			printf("\nThe Word: ");

			for(int i=0; i<wordLength; i++){
				printf("%c",word[i]);
			}
			wordLength = 0;
			wordFound = 0;
		}
	}

	//printf("\nThe Word: ");

	//for(int i=0; i<wordLength; i++){
		//printf("%c",word[i]);
	//}

	if(fclose(fileReader)){
		printf("Failed to close file reader after finding a vignere key.\n");
		return 1;
	}
	
}

int identifyCrypto(double letterPerc[], int letterDistro[], char* fileName){
	int transposition = 0;

	double topFivePerc = 0.0;

	// Add top five percentages together to check for normal english letter distrobution
	topFivePerc += letterPerc[letterDistro[0]];
	topFivePerc += letterPerc[letterDistro[1]];
	topFivePerc += letterPerc[letterDistro[2]];
	topFivePerc += letterPerc[letterDistro[3]];
	topFivePerc += letterPerc[letterDistro[4]];
	
	// If top five percentages are lower than 35% it is likely not normal english distrobution
	if(topFivePerc < .35){
		printf("This is likely a vignere cipher.\n");
		vignereKey(fileName);
		return 0;
	}
	else if(topLetters(letterDistro)){
		printf("This is likely a permutation cipher.\n");
		return 0;
	}
	else{
		printf("This is likely a shift cipher.\n");
		unshift(shiftNum(letterDistro), fileName);
		return 0;
	}

	printf("Rework the algorithm\n");
	return 0;
}

int sortPercentages(double letterPercentage[], int letterDistrobution[]){
	//int letterDistrobution[26];

	// Initialize array values to check for error later
	for(int i=0; i<26; i++){
		letterDistrobution[i] = -1;
	}

	// Set the first value of the distrobution array
	letterDistrobution[0] = 0;

	// Input array values based on character percentage rates
	for(int i=1; i<26; i++){
		int j = 0;
		while(letterPercentage[i]<letterPercentage[letterDistrobution[j]] && j<i){
			j++;
		}
		for(int k=i; k>j; k--){
			letterDistrobution[k] = letterDistrobution[k-1];
		}
		letterDistrobution[j] = i;
	}

	// Output the array
	for(int i=0; i<26; i++){
		printf("%c | ", letterDistrobution[i]+65);
	}

	return 0;
}

int parseCrypto(char* fileName){
	FILE* fileReader;
	char readChar;
	int alphabet[26];
	int letterDistrobution[26];
	double letterPercentage[26];
	int letter;
	long letterCount = 0;
	fopen(fileName, "r");

	// Initialize all array values
	for(int i=0; i<26; i++){
		alphabet[i] = 0;
	}

	if(!(fileReader = fopen(fileName, "r"))){
		printf("ERROR: Unable to read your file\n");
		return 1;
	}

	while(!feof(fileReader)){
		fread(&readChar, sizeof(char), 1, fileReader);
		if((letter = toupper(readChar)-65)<26){
			alphabet[letter]++;
			letterCount++;
		}
	}
	// Close the read file
	if(fclose(fileReader)){
		printf("ERROR: Unable to close your file\n");
		return 1;
	}

	for(int i=0; i<26; i++){
		letterPercentage[i] = ((double)alphabet[i]/(double)letterCount);
		printf("%c: %d | %f\n", (i+65), alphabet[i], letterPercentage[i]);
	}

	sortPercentages(letterPercentage, letterDistrobution);
	identifyCrypto(letterPercentage, letterDistrobution, fileName);

	return 0;
}

int main(int argc, char *argv[]){
	parseCrypto("ciphertexts/ciphertext3.txt");

	return 0;
}
