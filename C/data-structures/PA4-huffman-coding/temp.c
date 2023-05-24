    // CF    chars[hctSize];  // An array that holds the characters the ordered by frequency
    // for (int i = 0; i < hctSize; i++){      //initialize CF array
    //     chars[i].freq=0;
    // }
    
//void insertToCFArray(CF* array, char c);
    
// BST		*initializeBinarySearchTree();

	// BST		*bst;    // A binary search tree for the Huffman Coding algorithm

	// bst = initializeBinarySearchTree();
    
// BST *initializeBinarySearchTree() {
// 	// TODO: Implement a binary search tree (BST) data structure to be used in
// 	//       the Huffman Coding algorithm
// 	//       Note: You need to determine what will be the structure of the node
// 	//       in the BST
// 	//       Note: You are allowed to change the arguments to this function
// 	printf("Implement me\n");
// }

// typedef struct binarysearchtree{
// 	int temp;
// } BST;

// typedef struct charandfreq{
//     char c;
//     int freq;
// } CF;

// void insertToCFArray(CF* Array, char Char){
//     for (int i = 0; i < hctSize; i++){
//         if (Array[i].freq == 0){
//             Array[i].c = Char;
//             Array[i].freq = 1;
//             return;
//         }
//         if (Array[i].c == Char){
//             Array[i].freq++;
//             int j = i + 1;
//             CF temp = Array[i];
//             while (Array[j].freq < Array[i].freq && Array[j].freq != 0) j++;
//             for (int k = i + 1; k < j; k++){
//                 Array[k - 1] = Array[k];
//             }
//             Array[j - 1] = temp;
//             return;
//         }
//     }
// }