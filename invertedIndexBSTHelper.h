#ifndef INVERTED_BST_HELPER_H
#define INVERTED_BST_HELPER_H

#include "invertedIndex.h"
#include "invertedIndexRoot.h"


// Input: string of word
// Output: Empty InvertedIndexBST node with empty fileList, children node
InvertedIndexBST createNewBSTNode(char *new_word);

// Input: A InvertedIndexBST node, a FileList node
// Output: Added FileList node at the begin of InvertedIndexBST that have same key member fileList
void invertedIndexBSTInsert(InvertedIndexRoot root, char *key, FileList addedFile);


// Input: A key and a INvertedIndexBST tree
// Output: The corresponding node for the key in the tree
InvertedIndexBST searchKey(InvertedIndexBST root, char *key);

#endif