#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

int primeFactorization(int number, int factors[]){
	// A list of the first 10 prime numbers
	int primes[10] = {2,3,5,7,11,13,17,19,23,29};	
	// An int to identify the current factor
	int factorCount = 0;
	int primeCount = 0;

	//printf("\nnumber: %d\n", number);
	
	// Loop through the prime numbers to determine the factors
	while(primeCount < 10 && number != 1){
		if(number%primes[primeCount] == 0){
			//printf("| %d | %d | ", number, primes[primeCount]);
			number /= primes[primeCount];
			factors[factorCount] = primes[primeCount];
			while(number%primes[primeCount] == 0){
				//printf("%d | %d ", number, primes[primeCount]);
				number /= primes[primeCount];
			}
			factorCount++;
		}
		else{
			primeCount++;
		}
	}

	return 0;
}

float trigramCheck(char decryption[], int fileSize){
	char topTen[10][3] = {{'T','H','E'},{'A','N','D'},{'I','N','G'},{'E','N','T'},{'I','O','N'},
			     {'H','E','R'},{'F','O','R'},{'T','H','A'},{'N','T','H'},{'I','N','T'}};
	float expecFreq[10] = {.0187, .0078, .0069, .0036, .0033, .0042, .0028, .0037, .0032, .0029};
	float trigramTotal = 0;

	for(int i=0; i<10; i++){
		float trigram = 0.00;
		for(int j=0; j<fileSize; j++){
			if(decryption[j] == topTen[i][0]){
				j++;
				if(decryption[j] == topTen[i][1]){
					j++;
					if(decryption[j] == topTen[i][2]){
						trigram += 1;
					}
					else{
						j--;
					}
				}
				else{
					j--;
				}
			}
		}
		// Calculate the freq of the given trigram
		trigram /= (fileSize-2);
		// Calculate the difference between trigram freq and expected freq
		trigram -= expecFreq[i];
		if(trigram < 0){
			trigram *= -1;
		}
		// Total all the trigram freq differences
		trigramTotal += trigram;
	}
	return trigramTotal;
}

float bigramCheck(char decryption[], int fileSize){
	char topThree[10][2] = {{'T','H'},{'H','E'},{'I','N'},{'E','R'},{'A','N'},
			        {'R','E'},{'O','N'},{'A','T'},{'E','N'},{'N','D'}};
	float expecFreq[10] = {.0356, .0308, .0243, .0205, .0199, .0185, .0176, .0149, .0145, .0135};
	float bigramTotal = 0;

	for(int i=0; i<10; i++){
		float bigram = 0.00;
		for(int j=0; j<fileSize; j++){
			if(decryption[j] == topThree[i][0]){
				j++;
				if(decryption[j] == topThree[i][1]){
					bigram += 1;
				}
				else{
					j--;
				}
			}
		}
		// Calculate the specified bigram frequency
		bigram /= (fileSize-1);
		// Calculate the difference between the bigram freq and the expected freq
		bigram -= expecFreq[i];
		if(bigram < 0){
			bigram *= -1;
		}
		// Total the freq difference
		bigramTotal += bigram;
	}
	return bigramTotal;
}

int count(char* word, char* fileName, int returnFactors[]){
	FILE* fileReader;
	char* iter = word;
	char letter;
	int count = 0;
	int location = 0;
	int distance[5];
	int factors[10] = {0,0,0,0,0,0,0,0,0,0};

	if(!(fileReader = fopen(fileName, "r"))){
		printf("ERROR: Failed to open fileReader when counting words.\n");
		return -1;
	}

	while(!feof(fileReader)){
		iter = word;
		fread(&letter, sizeof(char), 1, fileReader);
		while(letter == *iter && !feof(fileReader)){
			//printf("Same Letter: %c | %c\n", *iter, letter);
			fread(&letter, sizeof(char), 1, fileReader);
			iter++;
			if(*iter == '\0'){
				if(count >= 1 && count <= 6){
					distance[count-1] = ftell(fileReader)-location;
					location = ftell(fileReader);
				}
				else{
					location = ftell(fileReader);
				}
				count++;
			}
		}
	}
	if(count > 3){
		printf("The Word: %s | %d", word, count);
		for(int i=0; i<count-1; i++){
			printf("\n| %d |", distance[i]);
			primeFactorization(distance[i], factors);
			
			printf("\nPRIMES: ");
			for(int j=0; j<10; j++){
				printf("%d | ", factors[j]);
			}
			printf("\nRETURNED: ");
			for(int j=0; j<10; j++){
				printf("%d | ", returnFactors[j]);
			}

			// Eliminate non-common prime factors
			if(i == 0){
				for(int j=0; j<10; j++){
					returnFactors[j] = factors[j];
					//printf("\nTest: %d\n", returnFactors[j]);
				}
			}
			else{
				int k=0;
				for(int j=0; j<10 && returnFactors[k] != 0; j++){
					if(factors[j] == returnFactors[k]){
						k++;
					}
					else if(factors[j] > returnFactors[k]){
						//printf("\nTEST: %d | %d\n", factors[j], returnFactors[k]);
						for(int temp = k; temp<9; temp++){
							returnFactors[temp] = returnFactors[temp+1];
						}
						factors[9] = 0;
						j--;
					}
				}
			}
			printf("\nFACTORS: ");
			for(int j=0; j<10; j++){
				printf("%d | ", returnFactors[j]);
			}
		}
		printf("\n");

		return 1;
	}

	if(fclose(fileReader)){
		printf("ERROR: Filed to close the fileReader when counting words.\n");
		return -1;
	}

	return 0;
}

int sortPercentages(float letterPercentage[], int letterDistrobution[], int sortSize){
	//int letterDistrobution[26];

	// Initialize array values to check for error later
	for(int i=0; i<sortSize; i++){
		letterDistrobution[i] = -1;
	}

	// Set the first value of the distrobution array
	letterDistrobution[0] = 0;

	// Input array values based on character percentage rates
	for(int i=1; i<sortSize; i++){
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
	for(int i=0; i<sortSize; i++){
		//printf("%c | ", letterDistrobution[i]+65);
		//printf("%d | ", letterDistrobution[i]);
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
	int distance;
	int primeFactors[10] = {0,0,0,0,0,0,0,0,0,0};

	if(!(fileReader = fopen(fileName, "r"))){
		printf("Failed to open file reader when finding a vignereKey.\n");
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
			if(wordLength >= 6){
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
			word[wordLength] = '\0';
			count(word, fileName, primeFactors);
			wordLength = 0;
			wordFound = 0;
		}
	}

	if(fclose(fileReader)){
		printf("Failed to close file reader after finding a vignere key.\n");
		return 1;
	}
	return 0;
}

int columnarFound(char decipheredText[], int fileSize, int key, int tries){
	char suffix[2];
	tries = 10-tries;

	printf("\n\n\n\n\n");

	switch(tries){
		case 1:
			suffix[0] = 's';
			suffix[1] = 't';
			break;
		case 2:
			suffix[0] = 'n';
			suffix[1] = 'd';
			break;
		case 3:
			suffix[0] = 'r';
			suffix[1] = 'd';
			break;
		default:
			suffix[0] = 't';
			suffix[1] = 'h';
	}


	printf("Encryption Type: Columnar\nEncryption Key: %d\nThis was deciphered using the %d%c%c most likely key of 10 attempted keys\n", key, tries, suffix[0], suffix[1]);
	printf("An excerpt of the deciphered test:\n");
	for(int i=0; i<fileSize && i<250; i++){
		printf("%c", decipheredText[i]);
	}
	printf("\n\n\n\n\n\n");
}

int columnarKey(char* fileName){
	FILE* fileReader;
	int fileSize;
	// Tracks the number of prime factors
	int numFactors = 0;
	// Used when calculating common multiples
	int offset = 0;
	int primeFactors[10];
	int commonMultiples[10];
	int likelyhoodOrder[10];
	float ngramFreq[10];
	char letter;

	// Initialize the arrays to have default values
	for(int i=0; i<10; i++){
		primeFactors[i] = 0;
		commonMultiples[i] = 1;
	}
	
	// Attempt to open the file to read, declare an error and exit if unsuccessful
	if(!(fileReader = fopen(fileName, "r"))){
		printf("ERROR: File failed to open in columnarKey\n");
		return -1;
	}
	
	// Determine the number of characters in the file
	fseek(fileReader, 0, SEEK_END);
	fileSize = ftell(fileReader);
	fseek(fileReader, 0, SEEK_SET);

	// Char array to store the cipher text
	char cipher[fileSize];
	char decipher[10][fileSize];

	// Copy the text of the file to the array
	for(int i=0; i<fileSize; i++){
		fread(&letter, sizeof(char), 1, fileReader);
		cipher[i] = letter;
	}

	// Find the prime factors
	primeFactorization(fileSize, primeFactors);

	// Calculate the number of factors
	while(primeFactors[numFactors] != 0){
		numFactors++;
	}

	// Calculate the Least Common Multiple of our factors
	for(int i=0; i<numFactors; i++){
		commonMultiples[0] *= primeFactors[i];
	}

	// Calculate the top nine least common multiples of the prime factors
	for(int i=1; i<10; i++){
		commonMultiples[i] = commonMultiples[(i-1+offset)/numFactors]*primeFactors[(i-1+offset)%numFactors];

		// Verify the multiple is not a repeat
		for(int j=0; j<i; j++){
			if(commonMultiples[i] == commonMultiples[j]){
				offset++;
				i--;
				break;
			}
		}
	}

	// Decrypt the files based on common multiples of prime factors of the file size
	for(int i=0; i<10; i++){
		char unencrypted[fileSize];
		offset = fileSize/commonMultiples[i];
		for(int j=0; j<fileSize/commonMultiples[i]; j++){
			int iter = j;
			for(int k=0; k<commonMultiples[i]; k++){
				decipher[i][k+j*commonMultiples[i]] = cipher[iter];
				iter+=offset;
			}
		}
		ngramFreq[i] = bigramCheck(decipher[i], fileSize)+trigramCheck(decipher[i], fileSize);
	}


	sortPercentages(ngramFreq, likelyhoodOrder, 10);

	for(int i=9; i>=0; i--){
		char userInput;
		for(int j=0; j<fileSize && j<100; j++){
			printf("%c", decipher[likelyhoodOrder[i]][j]);
		}
		printf("\nDoes the above excerpt look like the cipher has been cracked?(y/n): ");
		while((userInput = fgetc(stdin)) != 'n'){
			if(userInput == 'y'){
				columnarFound(decipher[likelyhoodOrder[i]], fileSize, commonMultiples[likelyhoodOrder[i]], i);
				return 0;
			}
			if(userInput == '\n'){

			}
			else{
				printf("\nDoes the above excerpt look like the cipher has been cracked?(y/n): ");
			}
		}
	}

	// Attempt to close the file reader, report an error if unable to close the file
	if(fclose(fileReader)){
		printf("ERROR: File failed to close in columnarKey\n");
		return -1;
	}

	return 0;
}

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

int identifyCrypto(float letterPerc[], int letterDistro[], char* fileName){
	int transposition = 0;

	double topFivePerc = 0.0;

	// Add top five percentages together to check for normal english letter distrobution
	topFivePerc += letterPerc[letterDistro[0]];
	topFivePerc += letterPerc[letterDistro[1]];
	topFivePerc += letterPerc[letterDistro[2]];
	topFivePerc += letterPerc[letterDistro[3]];
	topFivePerc += letterPerc[letterDistro[4]];
	
	// If top five percentages are lower than 35% it is likely not normal english distrobution
	printf("Distrobution: %f\n", topFivePerc);
	if(topFivePerc < .4){
		printf("This is likely a vignere cipher.\n");
		vignereKey(fileName);
		return 0;
	}
	else if(topLetters(letterDistro)){
		printf("This is likely a permutation cipher.\n");
		columnarKey(fileName);
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


int parseCrypto(char* fileName){
	FILE* fileReader;
	char readChar;
	int alphabet[26];
	int letterDistrobution[26];
	float letterPercentage[26];
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
		//printf("%c: %d | %f\n", (i+65), alphabet[i], letterPercentage[i]);
	}

	sortPercentages(letterPercentage, letterDistrobution, 26);
	identifyCrypto(letterPercentage, letterDistrobution, fileName);

	return 0;
}

int main(int argc, char *argv[]){
	parseCrypto("ciphertexts/ciphertext6.txt");	// Vignere
	//parseCrypto("ciphertexts/ciphertext5.txt");	// Permutation
	//parseCrypto("ciphertexts/ciphertext4.txt");	// Vignere
	//parseCrypto("ciphertexts/ciphertext3.txt");	// Unknown
	//parseCrypto("ciphertexts/ciphertext2.txt");	// Permutation
	//parseCrypto("ciphertexts/ciphertext1.txt");	// Shift
	//
	//
	if(0){
		FILE* filereader = fopen("ciphertexts/test.txt", "r");
		int fileSize = 0;
		int spaces = 0;
		char letter;
	
		fseek(filereader, 0, SEEK_END);
		fileSize = ftell(filereader);
		fseek(filereader, 0, SEEK_SET);
	
		char text[fileSize];
	
		for(int i=0; i<fileSize; i++){
			fread(&letter, sizeof(char), 1, filereader);
			printf("%c", letter);
			if(letter == ' ' || letter == '\n'){
				spaces++;
			}
			else{
				text[i] = toupper(letter);
			}
		}
	
		bigramCheck(text, (fileSize-spaces));
		trigramCheck(text, fileSize-spaces);
	}

	return 0;
}
