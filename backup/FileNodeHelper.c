#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "FileNodeHelper.h"
#include "invertedIndexBSTHelper.h"
#include "invertedIndexRoot.h"



// Input: string of word of fileName
// Output: Empty FileNode node with empty tf, next node
FileList FileListCreate(char *filename, double frequency) {
    FileList new = malloc(sizeof(*new));
    new->filename = malloc(strlen(filename) + 1);
    strcpy(new->filename, filename);
    new->tf = frequency;
    new->next = NULL;
    return new;
}



// Input: A FileList node
// Output: Free FileList and node linked list contain
void freeFileList(FileList a) {
    while (a != NULL) {
        FileList temp = a;
        a = a->next;
        free(temp->filename);
        free(temp);
    }
}

// Input: A FileList node
// Output: Length of linked list
int FileListCount(FileList fl) {
    if (fl == NULL) return 0;
    return 1 + FileListCount(fl->next);
}