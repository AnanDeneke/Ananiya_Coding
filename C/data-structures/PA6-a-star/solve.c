#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int k;      //global variable so all functions can access

typedef struct node{            //used for queue, graph and hash table
    int *board;
    struct node *pred, *nextHash;   //predecessor and pointer for hash table
    int move, numOfMoves;                   //move from parent node
} Node;

typedef struct openHashTable{
    int size;
    Node **table;
} OpenHashTable;

typedef struct priorityqueue{
	int capacity;
    int size;
    Node **array;
} Heap;

void generateNeighbors (Node *parentNode, Heap *heap, OpenHashTable *hashTable);
Heap	*initializePriorityQueue(int capacity);
void	deletePriorityQueue( Heap *pq );
int     parent(Heap *myHeap, int index);
void    inserttoHeap(Heap *myHeap, Node *node);
void    upheap(Heap *myHeap, int index);
Node    *getmin(Heap *myHeap);
void	downheap(Heap *myHeap, int index);
int     minChildIndex(Heap *myHeap, int index);

OpenHashTable *initializeHT(int size);
int hash(int *board,int size);
void insertToHT(OpenHashTable *hashTable, Node *node);
int isMember(OpenHashTable *hashTable, int *board);
void deleteHT(OpenHashTable *hashTable);

int isSolvable(int *board);
int manhatten(int *board);
Node *initializeNode(int *board, Node* pred, int move, int numOfMoves);
void printBoard(Node *node);

int main(int argc, char **argv)
{
	FILE *fp_in,*fp_out;
	
	fp_in = fopen(argv[1], "r");
	if (fp_in == NULL){
		printf("Could not open a file.\n");
		return -1;
	}
	
	fp_out = fopen(argv[2], "w");
	if (fp_out == NULL){
		printf("Could not open a file.\n");
		return -1;
	}
	
	char *line = NULL;
    int solvable, solved = 1;
	size_t lineBuffSize = 0;
	size_t lineSize;
    Node *node;
    OpenHashTable *hashTable;
    Heap *heap;
	
	getline(&line,&lineBuffSize,fp_in);//ignore the first line in file, which is a comment
	fscanf(fp_in,"%d\n",&k);//read size of the board
	getline(&line,&lineBuffSize,fp_in);//ignore the second line in file, which is a comment

	int initial_board[k*k]; //get kxk memory to hold the initial board
	for(int i=0;i<k*k;i++)
		fscanf(fp_in,"%d ",&initial_board[i]);
    //printBoard(initial_board, k);           //debugging
	fclose(fp_in);

    //SOLVE PUZZLE
    solvable = isSolvable(initial_board);
    if (solvable){
        hashTable = initializeHT(10000);
        heap = initializePriorityQueue(1000);
        node = initializeNode(initial_board, NULL, 0, 0);
        insertToHT(hashTable, node);
        while (node){
            //printBoard(node);           //debugging
            for (int i = 0; i < k*k - 1; i++){
                if (node->board[i] != i+1) solved = 0;
            }
            if (solved) break;
            generateNeighbors(node, heap, hashTable);
            node = getmin(heap);
            solved = 1;
        }
        // printf("Answer: %d\n", solvable); printBoard(node);      //debugging
    }
    //OUTPUT SOLUTION
	fprintf(fp_out, "#moves\n");
    if (!solvable){
        fprintf(fp_out, "no solution\n");
    }
    else{
        //backtrack from node to find move list
        Node *temp = node;
        int i = 0, numOfMoves = 0;
        while (temp->pred != NULL) {        //calculate number of moves to allocate space
            numOfMoves++;
            temp = temp->pred;
        }
        //printf("%d\n", numOfMoves);
        int moves[numOfMoves];

        while (node->pred != NULL){
            moves[i] = node->move;
            node = node->pred;
            i++;
        }
        for (int i = numOfMoves - 1; i >= 0; i--){
            fprintf(fp_out, "%d ", moves[i]);
        }
        fprintf(fp_out, "\n");
        deleteHT(hashTable);
        deletePriorityQueue(heap);
    }

	fclose(fp_out);

	return 0;
}

void generateNeighbors (Node *parentNode, Heap *heap, OpenHashTable *hashTable){
    int zeroIndex;
    int *board = parentNode->board;
    int Board[k*k];
    Node *node;

    for (int i = 0; i < k*k; i++){
        if (board[i] == 0){
            zeroIndex = i;
            break;
        }
    }
    for (int i = 0; i < k*k; i++){
        Board[i] = board[i];
    }
    if (zeroIndex%k != 0){
        Board[zeroIndex] = Board[zeroIndex - 1];
        Board[zeroIndex - 1] = 0;
        if (!isMember(hashTable, Board)){
            node = initializeNode(Board, parentNode, Board[zeroIndex], parentNode->numOfMoves + 1);
            insertToHT(hashTable, node);
            inserttoHeap(heap, node);
        }
        Board[zeroIndex - 1] = Board[zeroIndex];            //revert swap for next case
        Board[zeroIndex] = 0;
    }
    if (zeroIndex%k != k-1){
        Board[zeroIndex] = Board[zeroIndex + 1];
        Board[zeroIndex + 1] = 0;
        if (!isMember(hashTable, Board)){
            node = initializeNode(Board, parentNode, Board[zeroIndex], parentNode->numOfMoves + 1);
            insertToHT(hashTable, node);
            inserttoHeap(heap, node);
        }
        Board[zeroIndex + 1] = Board[zeroIndex];            //revert swap for next case
        Board[zeroIndex] = 0;
    }
    if (zeroIndex+k < k*k){
        Board[zeroIndex] = Board[zeroIndex + k];
        Board[zeroIndex + k] = 0;
        if (!isMember(hashTable, Board)){
            node = initializeNode(Board, parentNode, Board[zeroIndex], parentNode->numOfMoves + 1);
            insertToHT(hashTable, node);
            inserttoHeap(heap, node);
        }
        Board[zeroIndex + k] = Board[zeroIndex];            //revert swap for next case
        Board[zeroIndex] = 0;
    }
    if (zeroIndex-k >= 0){
        Board[zeroIndex] = Board[zeroIndex - k];
        Board[zeroIndex - k] = 0;
        if (!isMember(hashTable, Board)){
            node = initializeNode(Board, parentNode, Board[zeroIndex], parentNode->numOfMoves + 1);
            insertToHT(hashTable, node);
            inserttoHeap(heap, node);
        }
    }
}

OpenHashTable* initializeHT(int size){
    OpenHashTable *hashTable = malloc(sizeof(OpenHashTable));
    hashTable->size = size;
    hashTable->table = malloc(sizeof(Node *)*hashTable->size);
    for (int i = 0; i < hashTable->size; i++){
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

int hash(int *board,int size){
    unsigned long total=0;
    for(int i=0; i<k*k; i++){
        total = total + i*board[i];
    }
    return total % size;
}

void insertToHT(OpenHashTable *hashTable, Node *node){
//DOES NOT CHECK IF MEMBER (use tied with isMember() function)
    int position = hash(node->board, hashTable->size);
    node->nextHash = hashTable->table[position];
    hashTable->table[position] = node;
}

int isMember(OpenHashTable *hashTable, int *board){
    int position = hash(board, hashTable->size);
    Node *node = hashTable->table[position];
    int found;
    while(node!=NULL){
        found = 1;
        for (int i = 0; i < k*k; i++){
            if (node->board[i] != board[i]) found = 0;
        }
        if(found) return 1;
        else node=node->nextHash;
        found = 0;
    }
    return 0;
}

void deleteHT(OpenHashTable *hashTable){
    Node *node, *next;
    for (int i = 0; i < hashTable->size; i++){
        node = hashTable->table[i];
        while (node){
            next = node->nextHash;
            free(node->board);
            free(node);
            node = next;
        }
    }
    free(hashTable->table);
    free(hashTable);
}

Heap *initializePriorityQueue(int capacity) {
	Heap *myHeap = malloc(sizeof(Heap));
    myHeap->capacity = capacity;
    myHeap->size=0;
    myHeap->array = malloc(sizeof(Node*)*capacity);
    return myHeap;
}

void deletePriorityQueue( Heap *pq ) {
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
    Node* parent = myHeap->array[parentIndex];
    Node* idx = myHeap->array[index];
    if (parent->numOfMoves+manhatten(parent->board) <= idx->numOfMoves+manhatten(idx->board)) return; //current node’s value is larger than its parent’s
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
    Node* min = myHeap->array[minChild];
    Node* idx = myHeap->array[index];
    if(min->numOfMoves+manhatten(min->board) >= idx->numOfMoves+manhatten(idx->board)) return; //parent smaller than children
    //else, we need to downheap current value by swapping with the min child
	Node* temp = myHeap->array[index];
	myHeap->array[index] = myHeap->array[minChild];
	myHeap->array[minChild] = temp;
    downheap(myHeap, minChild);
}

int minChildIndex(Heap *myHeap, int index){
    Node *left = myHeap->array[(index+1)*2-1];
    Node *right = myHeap->array[(index+1)*2];
    if(myHeap->size-1 >= (index+1)*2)
	{
        if(right->numOfMoves+manhatten(right->board) >= left->numOfMoves+manhatten(left->board)) 
			return (index+1)*2-1;
        else 
			return (index+1)*2;
    }
    else if(myHeap->size-1 == (index+1)*2-1) 
		return (index+1)*2-1;
    else 
		return -1;
}

int manhatten(int *board){
    int temp = 0, dist = 0;
    for (int i = 0; i < k*k; i++){
        if (!board[i] || board[i] == i + 1) continue;
        dist += abs(i%k - (board[i]-1)%k);
        dist += abs(i/k - (board[i]-1)/k);
    }
    return dist;
}

int isSolvable(int *board){
    int zeroIndex;
    for (int i = 0; i < k*k; i++){
            if (board[i] == 0) zeroIndex = i;
    }
    int inversions = 0;
    for (int i = 0; i < k*k-1; i++){
        for (int j = i+1; j < k*k; j++){
            if (board[i] && board[j] && board[i]>board[j]) 
                inversions++;
        }
    }
    if (k%2){           //odd board
        if (!(inversions%2)) return 1;
    }
    else{               //even board
        if (!((zeroIndex/k)%2) == inversions%2) return 1;     //0 on odd row with even inversion or 0 on even row with odd inversion
    }
    return 0;
}

Node *initializeNode(int *board, Node* pred, int move, int numOfMoves){
    Node *newNode = malloc(sizeof(Node));
    newNode->pred = pred;
    newNode->move = move;
    newNode->numOfMoves = numOfMoves;
    newNode->board = malloc(sizeof(int)*k*k);
    for (int i = 0; i < k*k; i++){
        newNode->board[i] = board[i];
    }
    return newNode;
}

void printBoard(Node *node){
    if (!node) {
        printf ("Null Node!!\n");
        return;
    }
    int *board = node->board;
    for(int i=0; i < k*k; i++){
        printf ("%3d ", board[i]);
        if (i % k==k-1) printf("\n");
    }
    printf("\n");
}