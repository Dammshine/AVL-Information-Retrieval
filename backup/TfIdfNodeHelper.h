#ifndef TLFDFNODE_HELPER_H
#define TLFDFNODE_HELPER_H

#include "invertedIndex.h"

// TfldNodeHelper.c file --------------------------------------------
// Input: 
// Output: TfIdfList container can used for insert
TfIdfList TfIdfListCreate();

// Input: fileName, tfIdf
// Output: Insert new TfIdfList to TfIdfList provided
TfIdfList TfIdfListInsert(TfIdfList root, char *filename, double tfIdf);

// Input: Two tfIdef node* 
// Output: Merge two TfIdfList. 
//         For node in rhs contain same filename as arbitary node in lhs. Combine the tfIdfSum; 
//         For node in rhs contain filename not in lhs, add to the end of list
TfIdfList TfIdfListMerge(TfIdfList root, TfIdfList list);

// Input: TfIdfList head node
// Output: return TfIdfList Sorted by alphabetical order
TfIdfList TfIdfListSort(TfIdfList list);


// Input: One tfIdef node*
// Output: Free all memory associate with this node
TfIdfList TfIdfListFree(TfIdfList node);

// Input: Number of file contains word d1, and total file d2
// Output: log10(d1/d2)
double IdfCalculation(int d1, int d2);


#endif