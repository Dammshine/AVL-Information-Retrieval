#ifndef ROOT_OF_INVERTEDINDEX_H
#define ROOT_OF_INVERTEDINDEX_H

#include "invertedIndex.h"
#include <stdbool.h>

// A wrapper for InvertedIndex
typedef struct RootOfIIBST {
	InvertedIndexBST root;
} RootOfIIBST;

typedef struct RootOfIIBST* InvertedIndexRoot;


// Create InvertedIndexRoot
InvertedIndexRoot InvertedIndexRootCreate(); 


// Clean InvertedIndexRoot, return InvertedIndexBST stored
// Caution, it won't free InvertedIndexBST stored in root
InvertedIndexBST InvertedIndexRootUnpack(InvertedIndexRoot wrapper);

#endif
