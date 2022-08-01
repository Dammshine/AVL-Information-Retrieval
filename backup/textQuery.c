#include "textQuery.h"
#include "record.h"    
#include "AVL.h"    
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



struct Text {
    Tree root;          // The info extracted from file (I don't want user to know it is a tree QwQ, this thing store nothing)
    int totalWords;     // The total words from file
};


bool normalisation(char *word);

// ======================================================
//                    TextQuery
// ======================================================
// Create TextQuery from a file
// The TextQuery need contain
// 1. Total words count
// 2. Individual words and word count of each words
// 3. All words appear in textqeury need to be normalised
textQuery createTextQuery(char *filename, bool normalise) {
    // read char from filename
    FILE *input_stream = fopen(filename, "r");

    // Skip file if unable to open
    if (input_stream == NULL) {
        // perror(input_stream);
        printf("unable to open this file");
        return NULL;
    }

    // Create binary tree stored the word and count
    // Create root for file-binary tree
    Tree root = TreeNew(RecordCompare);

    // Read words by words
    char word[100];
    int counter = 0;
    if (normalise == true) {
        while (!feof(input_stream)) {
            fscanf(input_stream, "%s ", word);
            if (normalisation(word) == true) {
                Record rec = RecordNew(word);
                TreeInsert(root, rec);
                counter++;
            }
        }
    }
    else {
        while (!feof(input_stream)) {
            fscanf(input_stream, "%s ", word);
            Record rec = RecordNew(word);
            TreeInsert(root, rec);
            counter++;
        }
    }
    
    

    // Construct textQuery, in textQuery all words insert garunteed to success
    textQuery ret = malloc(sizeof(*ret));
    ret->root = root;
    ret->totalWords = counter;

    fclose(input_stream);
    return ret;
}


// Search in textQuery
// Return NULL if not found
// Otherwise return record of words
Record findWord(textQuery query, char *word) {
    if (query->root == NULL) return NULL;
    Record temp = RecordNew(word);
    Record ret = TreeSearch(query->root, temp);
    RecordFree(temp);
    return ret;
}


// Remove a “random" (LOL，if u r user pls leave this file) 
// record stored in textQuery
// Return the removed textQuery
// Return NULL if the textQuery is empty
Record removeAny(textQuery query) {
    if (query == NULL || query->root == NULL) return NULL;
    return TreeRemoveAny(query->root);
}


// Return the total numbers in words
int sizeOfQuery(textQuery query) {
    if (query == NULL) return 0;
    return query->totalWords;
}

// Free Query
// If flag == true, also free record
textQuery freeQuery(textQuery query, bool flag) {
    if (query == NULL) return NULL;
    query->root = TreeFree(query->root, flag);
    free(query);
    return NULL;
}

// ======================================================
//                    Collection
// ======================================================

// Return a linked list contains all records in query
Collection queryReturnAll(textQuery query) {
    if (query == NULL) return NULL;
    return TreeRemoveAll(query->root);
}



// ======================================================
//                    Auxilary
// ======================================================
// Complexity of this function is O(1)
bool normalised(char *word, int length);
bool normalisation(char *word) {
    // remove ending symbol
    int length = strlen(word);
    while (length > 0 && normalised(word, length) == false) {
        length = strlen(word);
    }
    
    
    
    // After normalisation, if word is invalid, return false
    if (length == 0) {
        return false;
    }
    
    // change to lower case
    int counter = 0;
    while (counter < length) {
        if (isalpha(word[counter])) {
            word[counter] = tolower(word[counter]);
        }
        counter++;
    }
    return true;
}

bool normalised(char *word, int length) {
    switch(word[length - 1]) {
        case '.' :
        case ',' :
        case ':' :
        case ';' :
        case '?' :
        case '*' :
            word[length - 1] = '\0';
            return false;
        default :
            return true;
    }
}