#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int k;      //global variable so all functions can access

typedef struct node{            //used for queue, graph and hash table
    int *board;
    struct node *pred, *nextHash, *nextQueue, *prevQueue;   //predecessor and pointers for hash table/queue
    int move;                   //move from parent node
} Node;

typedef struct openHashTable{
    int size;
    Node **table;
} OpenHashTable;

typedef struct queue{
    Node *head;
    Node *tail;
} Queue;

void generateNeighbors (Node *parentNode, Queue *queue, OpenHashTable *hashTable);
OpenHashTable *initializeHT(int size);
int hash(int *board,int size);
void insertToHT(OpenHashTable *hashTable, Node *node);
int isMember(OpenHashTable *hashTable, int *board);
void deleteHT(OpenHashTable *hashTable);
void insertToQueue (Queue *queue, Node *node);
Node *dequeue (Queue *queue);
int isSolvable(int *board);
Node *initializeNode(int *board, Node* pred, int move);
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
    Queue queue = {NULL, NULL};
	
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
        node = initializeNode(initial_board, NULL, 0);
        insertToHT(hashTable, node);
        while (node){
            //printBoard(node);           //debugging
            for (int i = 0; i < k*k - 1; i++){
                if (node->board[i] != i+1) solved = 0;
            }
            if (solved) break;
            generateNeighbors(node, &queue, hashTable);
            node = dequeue(&queue);
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
    }

	fclose(fp_out);

	return 0;
}

void generateNeighbors (Node *parentNode, Queue *queue, OpenHashTable *hashTable){
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
            node = initializeNode(Board, parentNode, Board[zeroIndex]);
            insertToHT(hashTable, node);
            insertToQueue(queue, node);
        }
        Board[zeroIndex - 1] = Board[zeroIndex];            //revert swap for next case
        Board[zeroIndex] = 0;
    }
    if (zeroIndex%k != k-1){
        Board[zeroIndex] = Board[zeroIndex + 1];
        Board[zeroIndex + 1] = 0;
        if (!isMember(hashTable, Board)){
            node = initializeNode(Board, parentNode, Board[zeroIndex]);
            insertToHT(hashTable, node);
            insertToQueue(queue, node);
        }
        Board[zeroIndex + 1] = Board[zeroIndex];            //revert swap for next case
        Board[zeroIndex] = 0;
    }
    if (zeroIndex+k < k*k){
        Board[zeroIndex] = Board[zeroIndex + k];
        Board[zeroIndex + k] = 0;
        if (!isMember(hashTable, Board)){
            node = initializeNode(Board, parentNode, Board[zeroIndex]);
            insertToHT(hashTable, node);
            insertToQueue(queue, node);
        }
        Board[zeroIndex + k] = Board[zeroIndex];            //revert swap for next case
        Board[zeroIndex] = 0;
    }
    if (zeroIndex-k >= 0){
        Board[zeroIndex] = Board[zeroIndex - k];
        Board[zeroIndex - k] = 0;
        if (!isMember(hashTable, Board)){
            node = initializeNode(Board, parentNode, Board[zeroIndex]);
            insertToHT(hashTable, node);
            insertToQueue(queue, node);
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

void insertToQueue (Queue *queue, Node *node){
    if (!queue->head) queue->tail = node;
    else queue->head->prevQueue = node;
    node->nextQueue = queue->head;
    queue->head = node;
}

Node *dequeue (Queue *queue){
    Node *ret;
    if (!queue->head) return NULL;      //queue is empty
    if (!queue->head->nextQueue){       //1 element
        queue->head = NULL;
        return queue->head;
    }
    //more than 1 element
    ret = queue->tail;
    queue->tail = queue->tail->prevQueue;
    queue->tail->nextQueue = NULL;
    return ret;
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

Node *initializeNode(int *board, Node* pred, int move){
    Node *newNode = malloc(sizeof(Node));
    newNode->pred = pred;
    newNode->move = move;
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