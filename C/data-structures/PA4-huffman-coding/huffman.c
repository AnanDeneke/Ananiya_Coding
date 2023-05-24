#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define hctSize 128

typedef struct node{
    int freq;
    char c;
    struct node *left, *right;
} Node;

typedef struct priorityqueue{
	int capacity;
    int size;
    Node **array;
} Heap;

typedef struct huffmancodetable{
	int		size;
	int 	*freqs;
	char	**codes;
} HCTable;

void	encode( FILE *plaintext, FILE *codetable, FILE *encodetxt );
void	decode( FILE *codetable, FILE *encodetxt, FILE *decodetxt );

HCTable *initializeHuffmanCodeTable();
void    insertToHCTable( HCTable *hct, char c );
void 	generateCodeTable(HCTable *hct, Node *bst, char *tempCode);
void	deleteHuffmanCodeTable( HCTable *hct );

void	deleteBinarySearchTree( Node *bst );
Node	*initializeNode(char Char, int Freq, Node *left, Node *right);

Heap	*initializePriorityQueue(int capacity);
void	deletePriorityQueue( Heap *pq );
int     parent(Heap *myHeap, int index);
void    inserttoHeap(Heap *myHeap, Node *node);
void    upheap(Heap *myHeap, int index);
Node    *getmin(Heap *myHeap);
void	downheap(Heap *myHeap, int index);
int     minChildIndex(Heap *myHeap, int index);

int main( int argc, char **argv) {
	char *mode;            // Encode/Decode argument
	char *plaintext_path;  // plainText.txt file path argument
	FILE *plaintext;       // plainText.txt file pointer
	char *codetable_path;  // codeTable.txt file path argument
	FILE *codetable;       // codeTable.txt file pointer
	char *encodetxt_path;  // encodedText.txt file path argument
	FILE *encodetxt;       // encodedText.txt file pointer
	char *decodetxt_path;  // decodedText.txt file path argument
	FILE *decodetxt;       // decodedText.txt file pointer
	if( argc < 5 ) {
		printf("Not Enough Arguments Supplied\n");
		return -1;
	}

	mode = argv[1];

	if( strcmp(mode, "encode") == 0 ) {
		plaintext_path = argv[2];
		codetable_path = argv[3];
		encodetxt_path = argv[4];

		plaintext = fopen(plaintext_path, "r"); // Open for reading
		codetable = fopen(codetable_path, "w"); // Open for writing
		encodetxt = fopen(encodetxt_path, "w"); // Open for writing

		if( (plaintext == NULL) ||
		    (codetable == NULL) ||
			(encodetxt == NULL) ) {
			printf("File Reading Error\n");
			return -1;
		}

		encode( plaintext, codetable, encodetxt );

		fclose( plaintext );
		fclose( codetable );
		fclose( encodetxt );
	}
	else if( strcmp(mode, "decode") == 0 ) {
		codetable_path = argv[2];
		encodetxt_path = argv[3];
		decodetxt_path = argv[4];

		codetable = fopen(codetable_path, "r"); // Open for reading
		encodetxt = fopen(encodetxt_path, "r"); // Open for reading
		decodetxt = fopen(decodetxt_path, "w"); // Open for writing

		if( (codetable == NULL) ||
			(encodetxt == NULL) ||
			(decodetxt == NULL) ) {
			printf("File Reading Error\n");
			return -1;
		}

		decode( codetable, encodetxt, decodetxt );

		fclose( codetable );
		fclose( encodetxt );
		fclose( decodetxt );
	}
	else {
		printf("Not A Valid Mode\n");
		return -1;
	}
}

void encode( FILE *plaintext, FILE *codetable, FILE *encodetxt ) {

	Node 	*bst, *node, *leftNode, *rightNode;	 // Binary search tree and temporary node pointers for algorithm
	Heap	*pq;     // A priority queue for the Huffman Coding algorithm
	HCTable	*hct;    // A closed hash table for storing huffman values
	int		c, originalLen=0, encodedLen=0;	// Temporary int/char used for iteration
	float	f;       // A temporary float variable used for iteration

    pq = initializePriorityQueue(hctSize);
	hct = initializeHuffmanCodeTable();

	// This loop will read the file character by character
	while( (c = fgetc(plaintext)) != EOF && c!='\n' ) {
		insertToHCTable(hct, (char) c);
	}
	for (int i = 0; i < hctSize; i++){
		if (hct->freqs[i] == 0) continue;
		node = initializeNode((char) i, hct->freqs[i], NULL, NULL);
		inserttoHeap(pq, node);
		//printf("%c %d\n", (char) i, hct->freqs[i]);			//for debugging purposes
	}

	// Run the Huffman Coding algorithm to generate the code table
	while (pq->size > 1){
		leftNode = getmin(pq);
		rightNode = getmin(pq);
		node = initializeNode('\0', leftNode->freq + rightNode->freq, leftNode, rightNode);
		inserttoHeap(pq, node);		//node will be root of the binary search tree when while loop finishes.
		// printf("size: %d freq: %d %c %c\n", pq->size, node->freq, leftNode->c, rightNode->c);
	}
	bst = node;
	
	generateCodeTable(hct, bst, ""); 

	// This encodes the plain text file using the huffman code values and writes
	// it to the encoding file
    fseek( plaintext, 0, SEEK_SET );
	while( (c = fgetc(plaintext)) != EOF && c!='\n' ) {
		originalLen+=8;
		encodedLen+=strlen(hct->codes[c]);
		fprintf( encodetxt, "%s", hct->codes[c]);
	}

	// This writes the codetable file
	for( c = 0; c < 128; c++ ) {
		if( hct->freqs[c] != 0 )
			fprintf(codetable, "%c,%s,%d\n", c, hct->codes[c], hct->freqs[c]);
	}

	printf("Original: %d bits\n", originalLen);
	printf("Compressed: %d bits\n", encodedLen);
	printf("Compression Ratio: %.2f%%\n", (double)encodedLen/originalLen*100);

	deleteHuffmanCodeTable( hct );
	deletePriorityQueue( pq );
	deleteBinarySearchTree( bst );
}

void decode( FILE *codetable, FILE *encodetxt, FILE *decodetxt ) {
	HCTable	*hct;        // A closed hash table used for storing huffman values
	int		f;           // A temporary float variable used for iteration
	char	c, buf[hctSize];    // A temporary buffer used for iteration
	Node	*bst, *node, *newNode;

	hct = initializeHuffmanCodeTable();
	bst = initializeNode('\0', 0, NULL, NULL);

	// This reads the codetable file into a huffman code table and generates the huffman tree.
	while( fscanf(codetable, "%c,%[^,],%d\n", &c, buf, &f) != EOF ) {
		insertToHCTable( hct, c);
		hct->codes[c] = malloc(sizeof(char)*strlen(buf));
		strcpy(hct->codes[c], buf);
		newNode = initializeNode(c, 0, NULL, NULL);
		node = bst;
		//printf("%c %s\n", c, buf);	//debugging
		for (int i = 0; i < strlen(buf) - 1; i++){
			//printf("%c", buf[i]);		//debugging
			if (buf[i] == '0'){
				if (node->left == NULL){
					node->left = initializeNode('\0', 0, NULL, NULL);
					node = node->left;
				}
				else node = node->left;
				//printf("left ");		//debugging
			}
			else if (buf[i] == '1'){
				if (node->right == NULL){
					node->right = initializeNode('\0', 0, NULL, NULL);
					node = node->right;
				}
				else node = node->right;
				//printf("right ");		//debugging
			}
		}
		if (buf[strlen(buf) - 1] == '0') {
			node->left = newNode;
			//printf("%cleft %c\n", buf[strlen(buf) - 1], node->left->c);
		}
		else if (buf[strlen(buf) - 1] == '1') {
			node->right = newNode;
			//printf("%cright %c\n", buf[strlen(buf) - 1], node->right->c);
		}
	}

	node = bst;
	while( (c = fgetc(encodetxt)) != EOF ) {
		// Writes to decoded file using the binary search tree
		if (c == '0'){
			node = node->left;
			if (node->c != '\0'){
				fprintf(decodetxt, "%c", node->c);
				node = bst;
			}
		}
		else if (c == '1'){
			node = node->right;
			if (node->c != '\0'){
				fprintf(decodetxt, "%c", node->c);
				node = bst;
			}
		}
	}

	deleteHuffmanCodeTable( hct );
	deleteBinarySearchTree( bst );
}

HCTable *initializeHuffmanCodeTable() {
	HCTable *hct;

	hct = malloc( sizeof(HCTable) );
	hct->size = 0;
	hct->codes = malloc( hctSize * sizeof(char *) );
	hct->freqs = malloc( hctSize * sizeof(float) );

	for(int i = 0; i < hctSize; i++ )
		hct->freqs[i] = 0;

	return hct;
}

void insertToHCTable( HCTable *hct, char c ) {
    //Uses ASCII value of character as index to avoid collision.
	if( hct->freqs[c] != 0 ){
        hct->freqs[c]++;
		hct->size++;
        return;
    }
	hct->freqs[c] = 1;
    hct->size++;
}

void generateCodeTable(HCTable *hct, Node *bst, char *tempCode){
	char temp[9];
	if (bst->left == NULL && bst->right == NULL){
		hct->codes[(int) bst->c] = malloc( sizeof(char) * strlen(tempCode) + 1 );
		strcpy(hct->codes[(int) bst->c], tempCode);
		//printf("G %c %d %s\n", bst->c, bst->freq, hct->codes[(int) bst->c]);
		return;
	}
	if (bst->left != NULL){
		strcpy(temp, tempCode);
		strcat(temp, "0");
		generateCodeTable(hct, bst->left, temp);
	}
	if (bst->right != NULL){
		strcpy(temp, tempCode);
		strcat(temp, "1");
		generateCodeTable(hct, bst->right, temp);
	}
}

void deleteHuffmanCodeTable( HCTable *hct ) {
	int i;

	for( i = 0; i < hctSize; i++ )
		if( hct->freqs[i] != 0 )
			free( hct->codes[i] );

	free( hct->codes );
	free( hct->freqs );
	free( hct );
}

Node *initializeNode(char Char, int Freq, Node *left, Node *right){
    Node *myNode = malloc(sizeof(Node));
    myNode->c = Char;
    myNode->freq = Freq;
    myNode->left = left;
    myNode->right = right;
    return myNode;
}

void deleteBinarySearchTree( Node *bst ) {
	if (bst->left == NULL && bst->right == NULL){
		free(bst);
		return;
	}
	if (bst->left != NULL)
		deleteBinarySearchTree(bst->left);

	if (bst->right != NULL)
		deleteBinarySearchTree(bst->right);
	free(bst);
}

Heap *initializePriorityQueue(int capacity) {
	Heap *myHeap = malloc(sizeof(Heap));
    myHeap->capacity = capacity;
    myHeap->size=0;
    myHeap->array = malloc(sizeof(Node*)*capacity);
    return myHeap;
}

void deletePriorityQueue( Heap *pq ) {
	for(int i = 0; i<pq->capacity; i++){
		free(pq->array[i]);
	}
	free(pq->array);
	free(pq);
}

int parent(Heap *myHeap, int index){
    int parentIndex = (int)(index-1)/2;
    return parentIndex;
}

void inserttoHeap(Heap *myHeap, Node *node){
    myHeap->array[myHeap->size]=node;
    upheap(myHeap,myHeap->size);
    myHeap->size++;
}

void upheap(Heap *myHeap, int index){
    if(index == 0) return; //current node is the root
    int parentIndex = parent(myHeap, index);
    if (myHeap->array[parentIndex]->freq <= myHeap->array[index]->freq) return; //current node’s value is larger than its parent’s
	//printf("%c %d\n", myHeap->array[index]->c, index);
    //else, we need to upheap current value by swapping with the parent
	Node* temp = myHeap->array[index];
	myHeap->array[index] = myHeap->array[parentIndex];
	myHeap->array[parentIndex] = temp;
    upheap(myHeap, parentIndex);
}

Node* getmin(Heap *myHeap){
    //swap(myHeap->array,0,myHeap->size-1);
	Node* temp = myHeap->array[0];
	myHeap->array[0] = myHeap->array[myHeap->size - 1];
	myHeap->array[myHeap->size - 1] = temp;
    myHeap->size--;
    downheap(myHeap, 0);
	return temp;
}

void downheap(Heap *myHeap, int index){
    if(myHeap->size-1 < (index+1)*2-1) return; //size-1 is due to last indexed value  //children do not exist
    int minChild = minChildIndex(myHeap,index);
    if(myHeap->array[index]->freq <= myHeap->array[minChild]->freq) return; //parent smaller than children
    //else, we need to downheap current value by swapping with the min child
	Node* temp = myHeap->array[index];
	myHeap->array[index] = myHeap->array[minChild];
	myHeap->array[minChild] = temp;
    downheap(myHeap, minChild);
}

int minChildIndex(Heap *myHeap, int index){
    if(myHeap->size-1 >= (index+1)*2)
	{
        if(myHeap->array[(index+1)*2]->freq >= myHeap->array[(index+1)*2-1]->freq) 
			return (index+1)*2-1;
        else 
			return (index+1)*2;
    }
    else if(myHeap->size-1 == (index+1)*2-1) 
		return (index+1)*2-1;
    else 
		return -1;
}