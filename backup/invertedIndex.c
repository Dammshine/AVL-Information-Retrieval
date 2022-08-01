// COMP2521 Assignment 1
// z5359322 Yi Zhou 2/03/2022

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invertedIndex.h"
#include "FileNodeHelper.h"
#include "invertedIndexBSTHelper.h"
#include "TfIdfNodeHelper.h"
#include "invertedIndexRoot.h"
#include "textQuery.h"
#include "record.h"


// Defined Key word can be found in record.h
// #define NORMALISE     true
// #define ORIGINAL      false
// #define FREE_RECORD   true
// #define KEEP_RECORD   false



// Auxilary function for printInvertedIndex
void printInvertedIndexHelper(InvertedIndexBST tree, FILE *filename);


// Insert Query into the tree
void QueryInsertToIIBST(InvertedIndexRoot tree, textQuery collection, char *filename);


// Debug Toool
void debug(InvertedIndexBST tree) {
	if (tree == NULL) {
		return;
	}
	printf("%s leftchild[%p]  rightchild[%p]\n", tree->word, tree->left, tree->right);
	debug(tree->left);
	debug(tree->right);
}


// ==========================================================
//                     Part 1
// ==========================================================
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
	// Create TextQuery for filename collection
	textQuery filesNameQuery = createTextQuery(collectionFilename, ORIGINAL);
	Collection filesNameCollection = queryReturnAll(filesNameQuery);

	// Create root for invertedIndexBST
	InvertedIndexRoot root = InvertedIndexRootCreate();
 
	// Inspect individual's filename
	// temp is used to inspect filesNameCollection
	Collection temp = filesNameCollection;
	
	while (temp != NULL) {
		// Read current file's info
		char *currFilename = RecordGetKey(temp->record);
		textQuery currFileQuery = createTextQuery(currFilename, NORMALISE);

		// Chuck current file's info into InvertedIndexBST
		QueryInsertToIIBST(root ,currFileQuery, currFilename);
		
		// Clean current file's Query
		currFileQuery = freeQuery(currFileQuery, FREE_RECORD);

		// Advance to next file
		temp = temp->next;
	}
	// Free up query and collection
	filesNameCollection = freeCollection(filesNameCollection, KEEP_RECORD);
	filesNameQuery = freeQuery(filesNameQuery, FREE_RECORD);
	
	InvertedIndexBST ret = InvertedIndexRootUnpack(root);
	return ret;
}


void printInvertedIndex(InvertedIndexBST tree, char *filename) {
	FILE *output = fopen(filename, "w");
	printInvertedIndexHelper(tree, output);
	fclose(output);
}

// Uses post-order traverse
void freeInvertedIndex(InvertedIndexBST tree) {
	// Base case 1
	if (tree == NULL) {
		return;
	}
	// Recursive case
	freeInvertedIndex(tree->left);
	freeInvertedIndex(tree->right);
	// Base case 2: Free current node
	freeFileList(tree->fileList);
	free(tree->word);
	free(tree);
}




// ==========================================================
//                     Part 2
// ==========================================================
TfIdfList searchOne(InvertedIndexBST tree, char *searchWord, int D) {
	// Retrive corresponding node for tree
	InvertedIndexBST searchNode = searchKey(tree, searchWord);
	if (searchNode == NULL) return NULL;

	// Calculate Idf
	int fileCount = FileListCount(searchNode->fileList);
	double Idf = IdfCalculation(D, fileCount);
	
	// Create root(head) for TfIdfList
	TfIdfList root = TfIdfListCreate();

	// Insert node into root
	FileList temp = searchNode->fileList;
	while (temp != NULL) {
		double tfIdf = temp->tf * Idf;
		root = TfIdfListInsert(root, temp->filename, tfIdf);
		temp = temp->next;
	}

	return root;
}

TfIdfList searchMany(InvertedIndexBST tree, char *searchWords[], int D) {
	// Create root
	TfIdfList root = TfIdfListCreate();

	// Scan through searchWord
	int counter = 0;
	while (searchWords[counter] != NULL) {
		printf("%s \n", searchWords[counter]);
		TfIdfList list = searchOne(tree, searchWords[counter], D);
		root = TfIdfListMerge(root, list);
		printf("===================\n");
		counter++;
	}

	return root;
}

void freeTfIdfList(TfIdfList list) {
	while (list != NULL) {
		TfIdfList temp = list;
		TfIdfListFree(temp);
		list = list->next;
	}
}





void printInvertedIndexHelper(InvertedIndexBST tree, FILE *filename) {
	// Base Case - Hit leaf
	if (tree == NULL) {
		return;
	}
	printInvertedIndexHelper(tree->left, filename);
	// Current case
	fputs(tree->word, filename);
	for (FileList temp = tree->fileList; temp != NULL; temp = temp->next) {
		char digit[256];
		sprintf(digit, " %s (%.7lf) ", temp->filename, temp->tf);
		fputs(digit, filename);
	}
	fputs("\n", filename);
	// Left/Right case
	printInvertedIndexHelper(tree->right, filename);
}



void QueryInsertToIIBST(InvertedIndexRoot tree, textQuery query, char *filename) {
	// Create Collection for read all info in query
	Collection collection = queryReturnAll(query);
	Collection temp = collection;
	int total = sizeOfQuery(query);

	// Traverse Collection
	while (temp != NULL) {
		Record rec = temp->record;
		double tf = (double)RecordGetValue(rec) / total;
		char *wordName = RecordGetKey(rec);
		FileList addedFile = FileListCreate(filename, tf);

		// Insert fileNode to InvertedIndexBST
		invertedIndexBSTInsert(tree, wordName, addedFile);
		temp = temp->next;
	}

	// Clean collection
	collection = freeCollection(collection, KEEP_RECORD);
}

