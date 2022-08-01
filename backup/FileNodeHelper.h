#ifndef FILENODE_HELPER_H
#define FILENODE_HELPER_H

#include "invertedIndex.h"
#include "invertedIndexRoot.h"


// FileNodeHelper.c file --------------------------------------------
// Input: string of word of fileName
// Output: Empty FileNode node with empty tf, next node
FileList FileListCreate(char *filename, double frequency);


// Input: A FileList node
// Output: Free FileList and node linked list contain
void freeFileList(FileList a);

// Input: A FileList node
// Output: Length of linked list
int FileListCount(FileList fl);


#endif