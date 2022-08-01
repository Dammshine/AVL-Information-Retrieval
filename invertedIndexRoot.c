#include "invertedIndexRoot.h"
#include "invertedIndex.h"
#include <stdio.h>
#include <stdlib.h>

// Create InvertedIndexRoot
InvertedIndexRoot InvertedIndexRootCreate() {
    InvertedIndexRoot new = malloc(sizeof(*new));
    new->root = NULL;
    return new;
}


// Clean InvertedIndexRoot, return InvertedIndexBST stored
// Caution, it won't free InvertedIndexBST stored in root
InvertedIndexBST InvertedIndexRootUnpack(InvertedIndexRoot wrapper) {
    InvertedIndexBST temp = wrapper->root;
    free(wrapper);
    return temp;
}