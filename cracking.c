#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#define ENGLISH_IC 0.068
clock_t start, end;

int unshift(int shift, char* fileName){
	FILE* fileReader;
	FILE* fileWriter;
	char letter;
	int i = 0;

	if(!(fileReader = fopen(fileName, "r"))){
		printf("ERROR: Failed to open read file to shift.\n");
		return 1;
	}
	if(!(fileWriter = fopen("decryption.txt", "w"))){
		printf("ERROR: Failed to open write file to shift.\n");
		return 1;
	}

	printf("\n\n\n\n\n");
	printf("Encryption Type: Caesar\nEncryption Key: %c/%d\n", shift+65, shift);
	printf("Time Taken: %f sec\n", ((double)(clock()-start)/CLOCKS_PER_SEC));
	printf("An excerpt of the deciphered text:\n");

	while(!feof(fileReader)){
		fread(&letter, sizeof(char), 1, fileReader);
		letter = toupper(letter);
		letter -= shift;
		if(letter < 65){
			letter+=26;
		}
		fwrite(&letter, sizeof(char), 1, fileWriter);
		if(i<250){
			printf("%c",letter);
		}
		i++;
	}

	printf("\n\n\n\n\n");

	if((fclose(fileReader))){
		printf("ERROR: Failed to close read file after shift.\n");
		if(!(fclose(fileWriter))){
			printf("ERROR: Failed to close write file after shift.\n");
		}
	}
	if((fclose(fileWriter))){
		printf("ERROR: Failed to close write file after shift.\n");
	}
	
	return 0;
}

int shiftNum(int letterDistro[], char* fileName){
	FILE* fileReader;
	int checkForE = 0;
	int k = 0;
	char letter;
	char userInput;

	if(!(fileReader = fopen(fileName, "r"))){
		printf("ERROR: Failed to open read file to shift.\n");
		return 1;
	}


	// Simple Shift
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			int difference = letterDistro[j]-letterDistro[i];
			if(difference < 0){ difference += 26; }
			//printf("  %c | %c | %d\n", letterDistro[i]+65,letterDistro[j]+65,difference);
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
			fseek(fileReader, 0, SEEK_SET);
			k = 0;
			while(!feof(fileReader) && k<100){
				fread(&letter, sizeof(char), 1, fileReader);
				//printf("%c | %d | ", letter, letterDistro[i]);
				letter -= letterDistro[i]-4;
				if(letter < 65){
					printf("%c", letter+26);
				}
				else{
					printf("%c", letter);
				}
				k++;
			}
			printf("\nDoes the above excerpt look like the cipher has been cracked?(y/n): ");
			while((userInput = fgetc(stdin)) != 'n'){
				if(userInput == 'y'){
					unshift(letterDistro[i]-4, fileName);
					return 0;
				}
				if(userInput == '\n'){

				}
				else{
					printf("\nDoes the above excerpt look like the cipher has been cracked?(y/n): ");
				}
			}
		}
	}
	fclose(fileReader);
	return -2;
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
	int distance[5] = {0,0,0,0,0};
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
		//printf("The Word: %s | %d", word, count);
		for(int i=0; i<5; i++){
		//	printf("| %d ", distance[i]);
		}
		int min = distance[0];
		int iter = 0;
		for(int i=1; i<count-1; i++){
			min = (min < distance[i]) ? min : distance[i];
		}
		for(int i=2; i<min && iter<10; i++){
			if(!(distance[0]%i + distance[1]%i + distance[2]%i + distance[3]%i + distance[4]%i)){
				returnFactors[iter] = i;
				iter++;
			}
		}
		return iter;
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

float indexOfCoincidence(char text[], int fileSize, int factor){
	int subSize = fileSize/factor+1;
	float index;
	float indexDiff = 0;
	char subText[subSize];

	for(int i=0; i<factor; i++){
		int iter = i;
		float difference;
		// Write subText to char array
		for(int j=0; iter<fileSize; j++){
			subText[j] = text[iter];
			iter += factor;
		}
		// Iterate through subtext and calculate the index of coincidence
		for(int j=0; j<26; j++){
			int letterFreq = 0;
			for(int k=0; k<subSize; k++){
				if(subText[k] == j+65){
					letterFreq++;
				}
			}
			index += (letterFreq)*(letterFreq-1);
		}	
		for(int i=0; i<subSize; i++){
			//printf("%c", subText[i]);
		}

		index /= ((float)(subSize)*(float)(subSize-1));
		difference = (ENGLISH_IC-index);
		if(difference < 0){
			difference *= -1;
		}
		indexDiff += difference;
	}
	return indexDiff;
}

int mutualIC(char text[], int fileSize, int factor, char key[]){
	int subSize = fileSize/factor+1;
	float maxIndex = 0;
	float indexDiff = 0;
	char subText[subSize];
	float normalDistrobution[26] = {.082, .015, .028, .043, .127, .022, .020, .061, .070, .0077, .040, .024, .067, .075, .019, .0001, .060, .063, .091, .028, .0098, .024, .0015, .020, .0007};

	for(int i=0; i<factor; i++){
		int iter = i;
		float difference;
		maxIndex = 0;
		// Write subText to char array
		for(int j=0; iter<fileSize; j++){
			subText[j] = text[iter];
			//printf("Text: %d | %c\n", iter, text[iter]);
			iter += factor;
		}
		//printf("\n");
		for(int j=0; j<subSize; j++){
			//printf("%c", subText[j]);
		}
		//printf("\n");
		// Iterate through the subText and find the likey key
		for(int j=0; j<26; j++){
			float index = 0;
			//printf("\n");
			if(j > 0){
				for(int k=0; k<subSize; k++){
					subText[k] = subText[k]-1;
					if(subText[k] < 65){
						subText[k] = subText[k]+26;
					}
					//printf("%c", subText[k]);
				}
			}
			//printf("\n");
			for(int k=0; k<26; k++){
				int letterFreq=0;
				for(int it=0; it<subSize; it++){
					if(subText[it] == k+65){
						letterFreq++;
					}
				}
				//printf("Freq of %c: %d\n", k+65, letterFreq);
				index += (letterFreq)*normalDistrobution[k];
			}
			index /= (float)subSize;
			if(index > maxIndex){
				//printf("\nINDEX: %f - %f | %d - %d\n", maxIndex, index, i, j);
				maxIndex = index;
				key[i] = j+65;
			}
		}
	}
}

int vignereFound(char encryptedText[], int fileSize, char key[], int keySize, int tries){
	FILE* fileWriter;
	char suffix[2];
	char letter;

	printf("\n\n\n\n\n");

	if(!(fileWriter = fopen("decryption.txt", "w"))){
		printf("ERROR: Failed to open write file in vignere.\n");
		return 1;
	}

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


	printf("Encryption Type: Vignere\nEncryption Key: ");
	for(int i=0; i<keySize; i++){
		printf("%c", key[i]);
	}

	printf("\nThis was deciphered on the %d%c%c try\n", tries, suffix[0], suffix[1]);
	printf("Time Taken: %f sec\n", ((double)(clock()-start)/CLOCKS_PER_SEC));
	printf("An excerpt of the deciphered test:\n");
	for(int i=0; i<fileSize; i++){
		if((letter = encryptedText[i]-key[i%keySize]) < 0){
			letter += 91;
		}
		else{
			letter += 65;
		}
		if(i<250){
			printf("%c", letter);
		}
		fwrite(&letter, sizeof(char), 1, fileWriter);
	}
	printf("\n\n\n\n\n\n");
	fclose(fileWriter);
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
	int factors[10] = {0,0,0,0,0,0,0,0,0,0};
	int numFactors = 0;

	if(!(fileReader = fopen(fileName, "r"))){
		printf("Failed to open file reader when finding a vignereKey.\n");
		return 1;
	}

	// Read all characters from the encrypted file into an arrayO | D 
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
	for(int iter=6; iter>0 && !numFactors; iter--){
		for(int i=0; i<fileSize; i++){
			for(int j=i+1; j<fileSize; j++){
				// When you find two characters that match
				while(encryptedFile[i+wordLength] == encryptedFile[j+wordLength]){
					// Increase word length
					wordLength++;
				}
				// Only count words with more than 4 characters
				if(wordLength >= iter){
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
				if(numFactors = count(word, fileName, factors)){
					break;
				}
				wordLength = 0;
				wordFound = 0;
			}
		}
	}
	
	if(wordFound == 0){
		return -2;
	}

	int likelyhoodOrder[numFactors];
	float possibleIndex[numFactors];


	for(int i=0; i<numFactors; i++){
		possibleIndex[numFactors] = indexOfCoincidence(encryptedFile, fileSize, factors[i]);
	}

	sortPercentages(possibleIndex, likelyhoodOrder, numFactors);


	// Mutual Index of Coincidence
	for(int i=numFactors-1; i>=0; i--){
		char userInput;
		char key[factors[i]];
		mutualIC(encryptedFile, fileSize, factors[i], key);
		for(int j=0; j<fileSize && j< 100; j++){
			if(encryptedFile[j]-key[j%factors[i]] < 0){
				//printf("\nTest: %d | %d | %d\n", encryptedFile[j], j%factors[i], key[j%factors[i]]);
				printf("%c", encryptedFile[j]-key[j%factors[i]]+91);
			}
			else{
				//printf("\nTest: %c | %d | %c\n", encryptedFile[j], j%factors[i], key[j%factors[i]]);
				printf("%c", encryptedFile[j]-key[j%factors[i]]+65);
			}
		}
		printf("\nDoes the above excerpt look like the cipher has been cracked?(y/n): ");
		while((userInput = fgetc(stdin)) != 'n'){
			if(userInput == 'y'){
				vignereFound(encryptedFile, fileSize, key, factors[i], numFactors-i);
				return 0;
			}
			if(userInput == '\n'){

			}
			else{
				printf("\nDoes the above excerpt look like the cipher has been cracked?(y/n): ");
			}
		}
	}

	printf("6\n");

	if(fclose(fileReader)){
		printf("Failed to close file reader after finding a vignere key.\n");
		return -1;
	}
	return -2;
}

int columnarFound(char decipheredText[], int fileSize, int key, int tries){
	FILE* fileWriter;
	char suffix[2];
	tries = 10-tries;
	
	if(!(fileWriter = fopen("decryption.txt", "w"))){
		printf("ERROR: Failed to open write file in columnar.\n");
		return 1;
	}


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
	printf("Time Taken: %f sec\n", ((double)(clock()-start)/CLOCKS_PER_SEC));
	printf("An excerpt of the deciphered test:\n");
	for(int i=0; i<fileSize; i++){
		fwrite(&decipheredText[i], sizeof(char), 1, fileWriter);
		if(i<250){
			printf("%c", decipheredText[i]);
		}
	}
	printf("\n\n\n\n\n\n");

	fclose(fileWriter);
	return 0;
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

	return -2;
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
		return 1;
	}
	return 0;
}

int identifyCrypto(float letterPerc[], int letterDistro[], char* fileName){
	int transposition = 0;
	int outcome = 0;

	double topFivePerc = 0.0;

	// Add top five percentages together to check for normal english letter distrobution
	topFivePerc += letterPerc[letterDistro[0]];
	topFivePerc += letterPerc[letterDistro[1]];
	topFivePerc += letterPerc[letterDistro[2]];
	topFivePerc += letterPerc[letterDistro[3]];
	topFivePerc += letterPerc[letterDistro[4]];
	
	// If top five percentages are lower than 35% it is likely not normal english distrobution
	if(topFivePerc < .4){
		outcome = vignereKey(fileName);
	}
	else if(topLetters(letterDistro)){
		outcome = columnarKey(fileName);
	}
	else{
		outcome = shiftNum(letterDistro, fileName);
	}

	if(outcome == -2){
		printf("\n\n\n\n\nThis cipher does not seem to be a Caesar, Vignere, or Columnar cipher and therefore cannot be cracked by this program.\n\n\n\n\n");
	}
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
	}

	sortPercentages(letterPercentage, letterDistrobution, 26);
	identifyCrypto(letterPercentage, letterDistrobution, fileName);

	return 0;
}

int decipher(void){
	start = clock();
	char fileName[1024];
	int i=0;
	int loop = 1;
	
	while(loop){
		i = 0;
		printf("Please enter the name of the file you would like to decrypt(enter \"quit\" if you want to quit): ");
		fgets(fileName, 1024, stdin);
		while(fileName[0] == '\n'){
			fgets(fileName, 1024, stdin);
		}
			while(fileName[i] != '\n'){
				i++;
			}
			
			fileName[i] = '\0';
	
			if(strcmp(fileName,"quit") == 0){
				loop = 0;
			}
			else{	
				parseCrypto(fileName);
			}
	}
	return 0;
}
