#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10000 // Buffer size for the entire file per the docs ("Format of the File to be Encryped")

int main(int argc, char **argv)
{

	// Check for incorrect number of parameters entered
	if (argc < 3)
	{
		fprintf(stderr, "Error.  Missing parameters.");
		return 0;
	}

	char *keyFile = argv[1]; // Txt file including the key is the first parameter
	char *txtFile = argv[2]; // Txt file including the cipher text is the second parameter

	// Declare file pointers to the key file and the cipher text file to be opened
	FILE *pKeyFile = fopen(keyFile, "r");
	FILE *pTxtFile = fopen(txtFile, "r");

	char buffer[BUFFER_SIZE];
	int c; // Character variable
	int size; // Variable to store the size of the matrix
	int *key; // Variable to store the members of the matrix (has to be an array)
	int index = 0; // Global index for position in key array
	const char delimiter[2] = " ";
	char *token = NULL;

	if (pKeyFile == NULL || pTxtFile == NULL) // If the file isn't found or something happened
	{
		fprintf(stderr, "Error opening file.");
		return 0;
	}
	else
	{
		fgets(buffer, sizeof(buffer), pKeyFile); // Read the first line of the key file
		size = atoi(buffer); // Set the size equal to the integer value of the first line (per the specs)

		key = malloc(sizeof(int) * (size * size)); // Malloc an array the size of size

		while ((fgets(buffer, sizeof(buffer), pKeyFile)) != NULL) // Read each line of the key file
		{
			char *bufferPtr = buffer;
			token = strtok(bufferPtr, delimiter);
			for (int i = 0; i < size; i++)
			{
				key[index] = atoi(token);
				token = strtok(NULL, delimiter);
				index++;
			}
		}

		printf("Key Matrix:\n");
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				printf("%i  ", key[i * size + j]);
			}
			printf("\n");
		}

		printf("\n");

		index = 0;  // RESET!

		// Start the magic and read in the plaintext file
		// Parse the characters and change all uppercase to lowercase
		while ((c = fgetc(pTxtFile)) != EOF)
		{
			if (isalpha(c)) // Verify that all the characters are alphabetic characters
			{
				if (c <= 'Z') // Deal with any that aren't
				{
					c -= 'A' - 'a'; // Only pass in the ones that are
				}
				buffer[index++] = (char)c; // Send that character to the buffer array
			}
		}

		if (index % size != 0)
		{
			int padding = size - (index % size);

			for (int i = 0; i < padding; i++)
			{
				buffer[index++] = 'x';
			}
		}
		buffer[index] = '\0';

		printf("Plaintext:\n");
		for (int i = 0; buffer[i] != NULL; i++) // Loop through the ciphertext
		{
			if (i != 0 && i % 80 == 0)
			{
				printf("\n");
			}
			printf("%c", buffer[i]); // Print each character
		}
		printf("\n");

		char cipherText[BUFFER_SIZE]; // Create the array for the cipher text

		index = 0; // RESET...again

		while (buffer[index] != NULL) // Execute once for each block while the block isn't at the end
		{
			for (int i = 0; i < size; i++) // Execute once for each letter
			{
				cipherText[index + i] = 0;
				for (int j = 0; j < size; j++)
				{
					cipherText[index + i] += (key[i * size + j] * (buffer[index + j] - 'a')) % 26;
					cipherText[index + i] %= 26;
				}
				cipherText[index + i] += 'a';
			}
			index += size; // Increment the block by the size of the key
		}
		cipherText[index] = '\0';

		printf("\n\n");
		printf("Ciphertext:\n");
		for (int i = 0; cipherText[i] != NULL; i++) // Loop through the ciphertext
		{
			if (i != 0 && i % 80 == 0)
			{
				printf("\n");
			}
			printf("%c", cipherText[i]); // Print each character
		}
		printf("\n");
	}
	fclose(pKeyFile);
	fclose(pTxtFile);
	return 0;
}
