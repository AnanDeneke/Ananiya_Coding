#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

typedef struct node{
	char word[BUFSIZE];
	struct node *next;
} Node;

typedef struct openHashTable{
    int size;
    Node **table;
} OpenHashTable;

Node *initializeNode(char* word,Node *next){
    Node *newNode = malloc(sizeof(Node));
    strcpy(newNode->word, word);
    newNode->next = next;
    return newNode;
}

int hash(char *word,int size)
{
    unsigned long total=0;
    for(int i=0; i<strlen(word); i++)    {
        total = total + word[i]; // adds up the ASCII values of characters in the word
        total = total * 101;
        //printf("%li\n", total);
    }
    //printf ("%lu %d\n", total, total%size);
    return total % size;
}


void insert(OpenHashTable *hashTable, char* word){
    int position = hash(word, hashTable->size);
    Node *ptr = hashTable->table[position];
    while(ptr!=NULL){
        if(!strcmp(word, ptr->word)) return;
        else ptr=ptr->next;
    }
    hashTable->table[position] = initializeNode(word,hashTable->table[position]);
} 

int isMember(OpenHashTable *hashTable, char* word){
    int position = hash(word, hashTable->size);
    Node *ptr = hashTable->table[position];
    while(ptr!=NULL){
        if(!strcmp(ptr->word,word)) return 1;
        else ptr=ptr->next;
    }
    return 0;
}

int main(int argc, char **argv){
	char *dictionaryFilePath = argv[1]; //this keeps the path to the dictionary file file
	char *inputFilePath = argv[2]; //this keeps the path to the input text file
	char *check = argv[3]; // this keeps the flag to whether we should insert mistyped words into dictionary or ignore
	int numOfWords=0; //this variable will tell us how much memory to allocate

	int insertToDictionary;
	if(strcmp(check,"add")==0)
		insertToDictionary = 1;
	else
		insertToDictionary = 0;
    
	////////////////////////////////////////////////////////////////////
	//read dictionary file
    FILE *fp = fopen(dictionaryFilePath, "r");
    char *line = NULL; //variable to be used for line counting
    size_t lineBuffSize = 0; //variable to be used for line counting
    size_t lineSize; //variable to be used for line counting

    //check if the file is accessible, just to make sure...
    if(fp == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    //First, let's count number of words in the dictionary.
    //This will help us know how much memory to allocate for our hash table
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1) numOfWords++;
    //printf("%d\n",numOfWords);            //Print line count for debugging purposes.
    
    //Initialize hash table, with known size of dictionary
    OpenHashTable *hashTable = malloc(sizeof(OpenHashTable));
    hashTable->size = numOfWords;
    hashTable->table = malloc(sizeof(Node *)*numOfWords);
    for(int i=0;i<numOfWords;i++) hashTable->table[i] = NULL;
    //rewind file pointer to the beginning of the file, to be able to read it line by line.
    fseek(fp, 0, SEEK_SET);

    char wrd[BUFSIZE];
    for (int i = 0; i < numOfWords; i++){
        fscanf(fp, "%s \n", wrd);
        //printf("%d: %s\n",i,wrd);
        //HINT: here is a good place to insert the words into your hash table
        insert(hashTable, wrd);
    }
    fclose(fp);
    
	////////////////////////////////////////////////////////////////////
	//read the input text file word by word
    fp = fopen(inputFilePath, "r");
	if(fp == NULL){
		fprintf(stderr, "Error opening file\n");
		return -1;
	}

    //HINT: You can use a flag to indicate if there is a misspleed word or not, which is initially set to 1
	int noTypo=1;

	//read a line from the input file
	while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
	{
		char *word;
        char alternative[BUFSIZE];
        //These are the delimiters you are expected to check for. Nothing else is needed here.
		const char delimiter[]= " ,.:;!\n";
        const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

		//split the buffer by delimiters to read a single word
		word = strtok(line,delimiter); 
		
		//read the line word by word
		while(word!=NULL)
		{
            // You can print the words of the inpit file for Debug purposes, just to make sure you are loading the input text as intended
			//printf("%s\n",word);

            
            // HINT: Since this nested while loop will keep reading the input text word by word, here is a good place to check for misspelled words
            if (!isMember(hashTable, word)) {
                noTypo = 0;
                printf("Misspelled word: %s\n",word);

                printf("Suggestions: "); //the suggested words should follow
                for (int i = 0; i < strlen(word) - 1; i++){
                    strcpy(alternative, word);
                    char temp = alternative[i];
                    alternative[i] = alternative[i+1];
                    alternative[i+1] = temp;
                    if (isMember(hashTable, alternative)) printf("%s ", alternative);
                }
                for (int i = 0; i < 26; i++){
                    alternative[0] = alphabet[i];
                    alternative[1] = '\0';
                    strcat(alternative, word);
                    if (isMember(hashTable, alternative)) printf("%s ", alternative);
                }
                for (int i = 0; i < 26; i++){
                    strcpy(alternative, word);
                    alternative[strlen(word)] = alphabet[i];
                    alternative[strlen(word) + 1] = '\0';
                    if (isMember(hashTable, alternative)) printf("%s ", alternative);
                    //printf("%s\n", alternative);
                }
                if (isMember(hashTable, word + 1)) printf ("%s ", word + 1);
                strcpy(alternative, word);
                alternative[strlen(word) - 1] = '\0';
                if (isMember(hashTable, alternative)) printf("%s ", alternative);
                
                printf("\n");
                
                if (insertToDictionary) insert(hashTable, word);
            }
            
            word = strtok(NULL,delimiter);
		}
	}
	fclose(fp);
    
    //HINT: If the flag noTypo is not altered (which you should do in the loop above if there exists a word not in the dictionary), then you should print "No typo!"
    if(noTypo==1)
        printf("No typo!\n");
    

    // DON'T FORGET to free the memory that you allocated
    
	return 0;
}