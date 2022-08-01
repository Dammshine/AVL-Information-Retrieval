#ifndef TEXTQUERY_H
#define TEXTQUERY_H


#include "record.h"
#include <stdlib.h>
#include <stdbool.h>



typedef struct Text *textQuery;







// ======================================================
//                    TextQuery
// ======================================================
// Create TextQuery from a file
// The TextQuery need contain
// 1. Total words count
// 2. Individual words and word count of each words
// 3. All words appear in textqeury need to be normalised if normalise is true
textQuery createTextQuery(char *filename, bool normalise);


// Search in textQuery
// Return NULL if not found
// Otherwise return record of words
Record findWord(textQuery query, char *word);


// Remove a random record stored in textQuery
// Return the removed textQuery
// Return NULL if the textQuery is empty
Record removeAny(textQuery query);


// Return the total numbers in words
int sizeOfQuery(textQuery query);

// Free Query
// If flag == true, also free record
textQuery freeQuery(textQuery query, bool flag);

// ======================================================
//                    Collection
// ======================================================

// Return a linked list contains all records in query
Collection queryReturnAll(textQuery query);




#endif