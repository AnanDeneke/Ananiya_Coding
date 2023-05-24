#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// TODO: Implement a priority queue data structure to be used in the Huffman
//       Coding algorithim
//       Note: You need to determine what will be the structure of the data to
//       be kept inside the priority queue
typedef struct priorityqueue
{
	int temp;
} PQ;

// TODO: Implement a binary tree data structure to be used in the Huffman
//       Coding algorithm
//       Note: You need to determine what will be the structure of the node in
//       the binary tree
typedef struct binarytree
{
	int temp;
} BT;

// I implemented a basic closed hash table for you to use. I use this for file
// interactions like reading and writing. You may use it however you please
typedef struct huffmancodetable
{
	int		size;
	int	    *freqs;
	char	**codes;

} HCTable;

// These are called function declarations. They are single lines at the top of
// the file to reference functions. It is a common practice in C to list your
// function declarations at the top of your file, then have your full main
// function, then the filled in function definitions at the end of your file.
// You should not need to change any of these top 5 declarations, but feel free
// to
void	encode( FILE *plaintext, FILE *codetable, FILE *encodetxt );
void	decode( FILE *codetable, FILE *encodetxt, FILE *decodetxt );
HCTable	*initializeHuffmanCodeTable();
void	insertToHCTable( HCTable *hct, char c, float f, char *code );
void	deleteHuffmanCodeTable( HCTable *hct );
// This is the bare minimum you need for implementing a priority queue and BT
// You will need to write more functions
PQ		*initializePriorityQueue();
void	deletePriorityQueue( PQ *pq );
BT		*initializeBinaryTree();
void	deleteBinaryTree( BT *bt );

// My main function only handles the preparation steps of the code. You will not
// need to edit this function. encode and decode is where you need to write code
int main( int argc, char **argv) {
	// I list all my variables at the top of my functions. I do this for many
	// reasons:
	//      1. It makes my code easier to read
	//      2. It makes my code easier to debug
	//      3. I have less segfaults and bugs in my code since I know exactly
	//           what memory and pointers I have available
	//      4. I write smarter and more efficient code since I can see easily
	//           how many variables I'm using
	//      5. I also get a chance to comment and describe each variable and
	//           what they do for the function up here
	char *mode;            // Encode/Decode argument
	char *plaintext_path;  // plainText.txt file path argument
	FILE *plaintext;       // plainText.txt file pointer
	char *codetable_path;  // codeTable.txt file path argument
	FILE *codetable;       // codeTable.txt file pointer
	char *encodetxt_path;  // encodedText.txt file path argument
	FILE *encodetxt;       // encodedText.txt file pointer
	char *decodetxt_path;  // decodedText.txt file path argument
	FILE *decodetxt;       // decodedText.txt file pointer
	// For this assignment, try to avoid declaring any variables below this line
	// Hopefully, this simple challenge will force you to think about your data
	// structures more. You may find you will spend less time debugging

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


// TODO: You need to fill in this function
void encode( FILE *plaintext, FILE *codetable, FILE *encodetxt ) {
	// As stated in main, I list all my variable declarations at the top of my
	// functions. The list is incomplete. Feel free to add, remove, or change
	// these
	PQ		*pq;     // A priority queue for the Huffman Coding algorithm
	BT		*bt;     // A binary  tree for the Huffman Coding algorithm
	HCTable	*hct;    // A closed hash table for storing huffman values
	char	c;       // Temporary int/char used for iteration
	float	f;       // A temporary float variable used for iteration
	// For this assignment, try to avoid declaring any variables below this line
	// Hopefully, this simple challenge will force you to think about your data
	// structures more. You may find you will spend less time debugging

	pq = initializePriorityQueue();
	bt = initializeBinaryTree();
	hct = initializeHuffmanCodeTable();

	// This loop will read the file character by character
	while( (c = fgetc(plaintext)) != EOF && c!='\n') {
		// TODO: calculate the frequency of each character
		//       c is the variable that stores the character
	}

	// TODO: Implement Huffman Coding algorithm, as described in the referenced
	//       text in the previous section, by using the priority queue and BT
	//       you implemented

	// TODO: Run the Huffman Coding algorithm on the set of character
	//       frequencies that you calculated to obtain the Huffman Code tree

	// TODO: Parse that tree to generate a Huffman Code table, where each
	//       character is assigned to a binary code
	//       Note: I have already created the table for you
	//       Use the insertToHCTable function to fill the table

	// This encodes the plain text file using the huffman code values and writes
	// it to the encoding file
    fseek( plaintext, 0, SEEK_SET );
	while( (c = fgetc(plaintext)) != EOF && c!='\n')
		fprintf( encodetxt, "%s", hct->codes[c]);

	// This writes the codetable file
	for( c = 0; c < 128; c++ ) {
		if( hct->freqs[c] != 0 )
			fprintf( codetable, "%c,%s,%d\n", c, hct->codes[c], hct->freqs[c]);
	}

	// TODO: Calculate compression statistics and print it to standard output.
	//       Your statistics should consist of the size of the original file in
	//       terms of bits (number of characters * 8, as we assume they were
	//       encoded using ASCII), the size of the encoded file in bits, and the
	//       compression ratio (the ratio of these two numbers in percentage)
    //      Print statement can look like as follows, assuming that necessary
    //      variable hold the information:
    printf("Original: %d bits\n", uncompressed*8);
    printf("Compressed: %d bits\n", compressed_size);
    printf("Compression Ratio: %.2f%%\n", (float)compressed_size/((float)uncompressed*8)*100);

	deleteHuffmanCodeTable( hct );
	deletePriorityQueue( pq );
	deleteBinaryTree( bt );
}


// TODO: You need to fill in this function
void decode( FILE *codetable, FILE *encodetxt, FILE *decodetxt ) {
	HCTable	*hct;        // A closed hash table used for storing huffman values
	int		c, ce, cd;   // Temporary int/char used for iteration
	int	    f;           // A temporary float variable used for iteration
	char	buf[128];    // A temporary buffer used for iteration
	// For this assignment, try to avoid declaring any variables below this line
	// Hopefully, this simple challenge will force you to think about your data
	// structures more. You may find you will spend less time debugging

	hct = initializeHuffmanCodeTable();

	// This reads the codetable file into a huffman code table. You can use this
	// table for the remainder of the decoding
	while( fscanf(codetable, "%c,%[^,],%d", &c, buf, &f) != EOF )
    {
		if ( c < 32 )
			continue;
		insertToHCTable( hct, c, f, buf );
	}

	// TODO: Read the Huffman Code Table and reconstruct the Huffman Coe tree
	//       (which will be a binary  tree!)

	// TODO: Implement the huffman decoding algorithm

	cd = -1;
	while( (ce = fgetc(encodetxt)) != EOF ) {
		// TODO: Decode the characters in the file using your algorithm

		// This if statement will print the decoded character to the decode file
		if( cd != -1 ) {
			fputc( cd, decodetxt );
			cd = -1;
		}
	}

	deleteHuffmanCodeTable( hct );
}

HCTable *initializeHuffmanCodeTable() {
	// This function is self explanatory. You've seen all this before
	HCTable *hct;
	int i;

	hct = malloc( sizeof(HCTable) );
	hct->size = 128;
	hct->codes = malloc( hct->size * sizeof(char *) );
	hct->freqs = malloc( hct->size * sizeof(int) );

	for( i = 0; i < hct->size; i++ )
		hct->freqs[i] = 0;

	return hct;
}

void insertToHCTable( HCTable *hct, char c, int f, char *code ) {
	// Another easy function. The main thing to note is that I do not allow
	// collisions at all. This should not affect your algorithm
	// My "hash function" is just using the ASCII value of char c as an index
	if( hct->freqs[c] != 0 )
		free(hct->codes[c]);

	hct->freqs[c] = f;
	hct->codes[c] = malloc( sizeof(char) * strlen(code) + 1 );
	strcpy( hct->codes[c], code );
}

void deleteHuffmanCodeTable( HCTable *hct ) {
	// Self explanatory, you've seen this all before
	int i;

	for( i = 0; i < hct->size; i++ )
		if( hct->freqs[i] != 0 )
			free( hct->codes[i] );

	free( hct->codes );
	free( hct->freqs );
	free( hct );
}


PQ *initializePriorityQueue() {
	// TODO: Implement a priority queue data structure to be used in the Huffman
	//       Coding algorithim
	//       Note: You need to determine what will be the structure of the data
	//       to be kept inside the priority queue
	//       Note: You are allowed to change the arguments to this function

	printf("Implement me\n");
}

void deletePriorityQueue( PQ *pq ) {
	// Remember to use this function to free all the data you malloc for your
	// priority queue

	printf("Implement me\n");
}



BT *initializeBinaryTree() {
	// TODO: Implement a binary  tree (BT) data structure to be used in
	//       the Huffman Coding algorithm
	//       Note: You need to determine what will be the structure of the node
	//       in the BT
	//       Note: You are allowed to change the arguments to this function

	printf("Implement me\n");
}


void deleteBinaryTree( BT *bt ) {
	// Remember to use this function to free all the data you malloc for your
	// binary  tree

	printf("Implement me\n");
}



