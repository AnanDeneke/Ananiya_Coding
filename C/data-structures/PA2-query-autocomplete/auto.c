#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

typedef struct wordAndWeight{
	int weight;
    char word[BUFSIZE];} Word;

void binaryRangeSearch (Word **data, char *queryWord, int n_bins, int *begin, int *end) {
    int start = 0, stop = n_bins - 1, middle;
    while(start<=stop){
        middle = start + (stop - start)/2;
        if (!strncmp(data[middle]->word, queryWord, strlen(queryWord)) 
            && (middle == 0 || strncmp(data[middle-1]->word, queryWord, strlen(queryWord)))) *begin = middle;
        if (strncmp(data[middle]->word, queryWord, strlen(queryWord)) > 0 
            || !strncmp(data[middle]->word, queryWord, strlen(queryWord))) stop = middle - 1;
        else start = middle + 1;
    }
    start = 0;
    stop = n_bins - 1;
    while(start<=stop){
        middle = start + (stop - start)/2;
        if (!strncmp(data[middle]->word, queryWord, strlen(queryWord)) 
            && (middle == n_bins - 1 || strncmp(data[middle+1]->word, queryWord, strlen(queryWord)))) *end = middle;
        if (strncmp(data[middle]->word, queryWord, strlen(queryWord)) > 0) stop = middle - 1;
        else start = middle + 1;
    }
}

int partition (Word **data, int start, int stop){
    Word *temp, *pivot = data[stop];
    int i = start;
    for (int j = start; j < stop; j++){
        if (strcmp(data[j]->word, pivot->word) <= 0 ){
            temp = data[j];
            data[j] = data[i];
            data[i] = temp;
            i++;
        }
    }
    temp = data[i];
    data[i] = data[stop];
    data[stop] = temp;
    return i;
}

void quicksort (Word **data, int start, int stop){
    if (start < stop){
        int p = partition(data, start, stop);
        quicksort(data, start, p - 1);
        quicksort(data, p + 1, stop);
    }
}

// int cmpfunc (const void * a, const void * b) {           //use in qsort(debugging)
//     const Word *a_ = *(const Word **)a;
//     const Word *b_ = *(const Word **)b;
//     return strcmp(a_->word, b_->word);
// }

int main(int argc, char **argv) {
    char *inputFilePath = argv[1]; //this keeps the path to input file
    char *queryWord = argv[2]; // this keeps the query word
    int lineCount=0; //this variable will tell us how much memory to allocate
    
    FILE *fp = fopen(inputFilePath, "r");
    char *line = NULL; //variable to be used for line counting
    size_t lineBuffSize = 0; //variable to be used for line counting
    size_t lineSize; //variable to be used for line counting
    if(fp == NULL){
        fprintf(stderr, "Error opening file\n");
        return -1;
    }

    //let's count number of lines
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1){
        lineCount++;
    }
    
    //Read the file once more, this time to fill in the data into memory
    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    Word **data = malloc(sizeof(Word *)*lineCount);
    Word *cur;

    for(int i = 0; i < lineCount; i++){
        cur = malloc(sizeof(Word));
        fscanf(fp, "%s %d\n",cur->word,&(cur->weight));
        data[i] = cur;
    }

    //qsort(data, lineCount, sizeof(Word *), cmpfunc);          //to see if quicksort() works! (debugging)
    quicksort(data, 0, lineCount - 1);

    // for(int i = 0; i < lineCount; i++){
    //     printf("%s %d\n",data[i]->word,data[i]->weight);     //print data stored (debugging)
    // }

    fclose(fp);


    //OUTPUT SPECS:
    int start = -1, end = -1;
    Word *out[10];

    binaryRangeSearch (data, queryWord, lineCount, &start, &end);
    //printf ("%d %d\n", start, end);       //check if binaryRangeSearch worked (debugging)

    if (start == -1) {
        printf("No suggestion!\n");
    }
    else {
        Word Temp = {.weight = 0, .word = ""};                //create dummy variable to compare values to
        Word *temp = &Temp;                                   //make a pointer to it for ease of set, reset and comparison
        for (int i = start; i <= end; i++){
            if (data[i]->weight > temp->weight) temp = data[i];
        }
        out[0] = temp;
        for (int i = 1; i < 10 && i <= end - start; i++){
            temp = &Temp;
            for (int j = start; j <= end; j++){
                if (data[j]->weight > temp->weight && data[j]->weight < out[i - 1]->weight) temp = data[j];
            }
            out[i] = temp;
        }
        for(int i = 0; i < 10 && i <= end - start; i++){
            printf("%s,%d\n",out[i]->word,out[i]->weight);
        }
    }
    

    for(int i = 0; i < lineCount; i++) free(data[i]);
    free(data);
    
    return 0;
}
