/*
 * Purpose of this BST is to adapt genetic programming style for this data structure. 
 * In this file, BST can be view as
 *                   [root] -> record
 *                   |    |
 *    record <- [left]    [right] -> record
 * It don't need to know what is inside of record
 */

#ifndef TREE_H
#define TREE_H

#include "record.h"
#include <stdbool.h>

typedef struct tree *Tree;


/**
 * Creates a new tree that will use the given comparison function
 */
Tree TreeNew(int (*compare)(Record, Record));

/**
 * Frees the given tree. If freeRecords is true, also frees the records.
 */
Tree TreeFree(Tree t, bool freeRecords);

/**
 * Inserts the given record. Returns true if the record was inserted
 * successfully, or false if there was already a record that compares
 * equal to the given record in the tree (according to the comparison
 * function).
 */
bool TreeInsert(Tree t, Record rec);

/**
 * Deletes the record that compares equal to the given record (according
 * to the comparison function). Returns true if the record was deleted,
 * or false if there was no record that compared equal to the given
 * record.
 */
bool TreeDelete(Tree t, Record rec);

/**
 * Searches for a record that compares equal to the given record
 * (according to the comparison function). Returns the record if it
 * exists, or NULL otherwise.
 */
Record TreeSearch(Tree t, Record rec);


// Remove a random record stored in AVL tree
// Return the removed AVL tree
// Return NULL if the Tree is empty
Record TreeRemoveAny(Tree t);

/**
 * Displays all records in the given tree in order, one per line
 */
void TreeListInOrder(Tree t);

/**
 * Output all records in the linked list
 */
Collection TreeRemoveAll(Tree t);



#endif