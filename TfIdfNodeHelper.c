#include "invertedIndex.h"
#include "TfIdfNodeHelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



// ====================================================================
//               sorting function for TfIdfListMerge
// ====================================================================
#define INITIAL_SIZE 64

struct TfIdfArray {
    int curr;
    int maxSize;
    TfIdfList *vector;
};
typedef struct TfIdfArray *TfIdfListArray;

struct TfIdfListwrapper {
    TfIdfList head;
    TfIdfList tail;
};
typedef struct TfIdfListwrapper* TfIdfListWrapper;

void TfIdfListAppend(TfIdfListWrapper list, TfIdfList append);

// Sorting function
int TfIdfListCompare(TfIdfList lhs, TfIdfList rhs, int mode);
TfIdfListArray TfIdfArraySort(TfIdfListArray array, int mode);
int partition(TfIdfListArray array, int i, int j, int mode);

// TfIdfListArray 
TfIdfListArray TfIdfListArrayCreate(TfIdfList list);
TfIdfListArray TfIdfListArrayInsert(TfIdfListArray vector, TfIdfList list);
TfIdfListArray TfIdfListArrayClean(TfIdfListArray vector);
TfIdfList TfIdfListArrayRemoveAll(TfIdfListArray array);


// Debuging
void TfIdfListArrayPrint(TfIdfListArray array);
void TfIdfListPrint(TfIdfList list);

// ====================================================================
//                        User Function
// ====================================================================
// Input: 
// Output: TfIdfList container can used for insert
TfIdfList TfIdfListCreate() {
    return NULL;
}

// Input: string of word of fileName
// Output: Empty FileNode node with empty tf, next node
TfIdfList TfIdfListInsert(TfIdfList root, char *filename, double tfIdf) {
    TfIdfList new = malloc(sizeof(*new));
    
    new->filename = malloc(strlen(filename) + 1);
    strcpy(new->filename, filename);

    new->tfIdfSum = tfIdf;
    new->next = NULL;

    // Return new node as TfIdfList
    if (root == NULL) return new;
    // Insert into TfIdfList
    new->next = root;
    return new;
}

// Assumed both are sorted by fileName
// Return merged list also sorted by filename
TfIdfList TfIdfListMerge(TfIdfList root, TfIdfList list) {
    // Case 1: root is NULL, let list assumed root
    if (root == NULL) {
        return list;
    }
    // Case 2: list is NULL
    if (list == NULL) {
        return root;
    }
    // Merge the list
    TfIdfListWrapper mergeList = malloc(sizeof(*mergeList));
    mergeList->head = NULL;
    mergeList->tail = NULL;
    while (root != NULL && list != NULL) {
        TfIdfList nextInsert;
        int cmp = TfIdfListCompare(root, list, ALPHABETICAL);
        if (cmp > 0) {
            // Pick root
            nextInsert = root;
            root = root->next;
            TfIdfListAppend(mergeList, nextInsert);
        }
        else if (cmp < 0) {
            // Pick list
            nextInsert = list;
            list = list->next;
            TfIdfListAppend(mergeList, nextInsert);
        }
        else {
            // Same record
            TfIdfList comb = root;
            root = root->next;
            comb->tfIdfSum = comb->tfIdfSum + list->tfIdfSum;
            TfIdfList temp = list;
            list = list->next;
            TfIdfListFree(temp);
            TfIdfListAppend(mergeList, comb);
        }
    }
    if (root == NULL) {
        while (list != NULL) {
            TfIdfList nextInsert = list;
            list = list->next;
            TfIdfListAppend(mergeList, nextInsert);
        }
    }
    else {
        while (root != NULL) {
            TfIdfList nextInsert = root;
            root = root->next;
            TfIdfListAppend(mergeList, nextInsert);
        }
    }
    TfIdfList ret = mergeList->head;
    free(mergeList);
    return ret;
}

// Input: TfIdfList head node
// Output: return TfIdfList Sorted by alphabetical order/Tfldf descending order
TfIdfList TfIdfListSort(TfIdfList list, int mode) {
    // Sort list
    // Store List into TfIdfListArray and destroy list
    TfIdfListArray sort = TfIdfListArrayCreate(list);
    list = NULL;

    // Sort list and retrive list
    sort = TfIdfArraySort(sort, mode);
    TfIdfList sortedList = TfIdfListArrayRemoveAll(sort);
    TfIdfListArrayClean(sort);
    return sortedList;
}

// Input: One tfIdef node*
// Output: Free all memory associate with this node
TfIdfList TfIdfListFree(TfIdfList node) {
    free(node->filename);
    free(node);
    return NULL;
}

// Input: Number of file contains word d1, and total file d2
// Output: log10(d1/d2)
double IdfCalculation(int d1, int d2) {
    return log10((double) d1 / d2);
}

// ====================================================================
//                  sorting function for TfIdfListMerge
// ====================================================================
int TfIdfListCompare(TfIdfList lhs, TfIdfList rhs, int mode) {    
    if (mode == ALPHABETICAL) return strcmp(lhs->filename, rhs->filename);
    if (mode == TFIDF) {
        if (lhs->tfIdfSum > rhs->tfIdfSum) {
            return -1;
        }
        if (lhs->tfIdfSum < rhs->tfIdfSum) {
            return 1;
        }
        return strcmp(lhs->filename, rhs->filename);
    }
    fprintf(stderr , "TfIdfListCompare supplied invalid mode\n");
    exit(1);
}

TfIdfListArray TfIdfListSortHelper(TfIdfListArray array, int i, int j, int mode) {
    if (i < j) {
        int q = partition(array, i, j, mode);
        TfIdfListSortHelper(array, i, q - 1, mode);
        TfIdfListSortHelper(array, q + 1, j, mode);
    }
    return array;
}


// Invariant analysis {
//      At the begining of each iteration of the for loop, we have
//      1. If position k, i <= k <= leftPartition is less than pivot
//      2. leftPartition < k <= rightPartition, then is greater than or equal pivot
//      3. If k = j, then A[k] is equal to pivot
//      If rightPartition < k  < j, then have not being uncovered, thus do not have any relationship.
// }
// Initialization: Invariant hold prior to the loop, as all element beside pivot remain uncovered.
// Maintenance: We consider two case, suppose condition 1,2 satisfy prior for scanning (rightPartition + 1)
//              1. Suppose rightPartition + 1 greater than or equal pivot. Then we advance rightPartition. In this case 
//                  a. leftPartition < k <= rightPartition + 1, satisfy, as element within leftPartition <= k <= rightPartition range greater than pivot and rightPartition + 1 greater than pivot
//                  b. If position k, i <= k <= leftPartition is less than pivot, unchanged.
//              2. Suppose rightPartition + 1 smaller than pivot. leftPartition++, and swap leftPartition with rightPartition++.
//                  a. leftPartition + 1 < k <= rightPartition + 1 satisfy because rightPartition + 1 swap with leftPartition + 1, and leftPartition + 1 greater or qual to pivot.
//                  b. i <= k <= leftPartition + 1, as leftPartition + 1 swap with rightPartition + 1 , which is less than pivot.
// Termination: At the end, rightPartition = j - 1. Then the array have partitioned by a set smaller than pivot, a set greater or equal to pivot, a single entry is pivot at the end.
int partition(TfIdfListArray array, int i, int j, int mode) {
    // Select pivot
    TfIdfList pivot = array->vector[j];
    
    // Set left, right partition flag both at the start of array
    int leftPartition = i - 1;
    int rightPartition = i;
    // Use right flag advance array
    for (;rightPartition < j; rightPartition++) {
        int compareRightP = TfIdfListCompare(array->vector[rightPartition], pivot, mode);
        // If array element at right flag position
        // Advance left flag position, swap left flag element with right flag element
        if (compareRightP < 0) {
            leftPartition++;
            TfIdfList temp = array->vector[leftPartition];
            array->vector[leftPartition] = array->vector[rightPartition];
            array->vector[rightPartition] = temp;
        }
        // Advance right flag position
    }
    // Exchange pivot with leftPartition + 1
    // Then, i..leftPartition is less than pivot and leftPartition + 1..j is greater than or euqal to pivot
    leftPartition++;
    TfIdfList temp = array->vector[leftPartition];
    array->vector[leftPartition] = array->vector[j];
    array->vector[j] = temp;
    return leftPartition;
}


TfIdfListArray TfIdfArraySort(TfIdfListArray array, int mode) {
    return TfIdfListSortHelper(array, 0, array->curr - 1, mode);
}

// ====================================================================
//                     TfIdfListArray
// ====================================================================
TfIdfListArray TfIdfListArrayCreate(TfIdfList list) {
    TfIdfList temp = list;
    // Initialize array
    TfIdfListArray new = malloc(sizeof(*new));
    new->maxSize = INITIAL_SIZE;
    new->curr = 0;
    new->vector = malloc(sizeof(*list) * INITIAL_SIZE);

    while (temp != NULL) {
        new = TfIdfListArrayInsert(new, temp);
        temp = temp->next;
    }
    return new;
}

TfIdfListArray TfIdfListArrayInsert(TfIdfListArray array, TfIdfList list) {
    if (array->curr == array->maxSize) {
        int newMax = array->maxSize * 2;
        array->vector = realloc(array->vector, sizeof(struct TfIdfNode) * newMax);
        array->maxSize = newMax;
    }
    array->vector[array->curr] = list;
    array->curr++;
    return array;
}

TfIdfListArray TfIdfListArrayClean(TfIdfListArray vector) {
    free(vector->vector);
    free(vector);
    return NULL;
}

TfIdfList TfIdfListArrayRemoveAll(TfIdfListArray array) {
    // Initialize TfIdfList head
    TfIdfList head = array->vector[0];
    head->next = NULL;

    // Always insert after current and update current
    // So new List is in low-high order
    TfIdfList curr = head;
    int counter = 1;
    while (counter < array->curr) {
        TfIdfList insert = array->vector[counter];
        insert->next = NULL;
        curr->next = insert;
        curr = insert;
        counter++;
    }
    curr->next = NULL;
    return head;
}

// Debug function
void TfIdfListArrayPrint(TfIdfListArray array) {
    int counter = 0;
    while (counter < array->curr) {
        printf("%s\n", (array->vector[counter])->filename);
        counter++;
    }
}

void TfIdfListPrint(TfIdfList list) {
    TfIdfList temp = list;
    while (temp != NULL) {
        printf("%s\n", temp->filename);
        temp = temp->next;
    }
}


void TfIdfListAppend(TfIdfListWrapper list, TfIdfList append) {
    if (list->head == NULL) {
        list->head = append;
        list->tail = append;
        return;
    }
    // Different record, append at the end
    int cmp = TfIdfListCompare(list->tail, append, ALPHABETICAL);
    
    if (cmp > 0) {
        list->tail->next = append;
        append->next = NULL;
        list->tail = list->tail->next;
        return;
    }
    return;
}
