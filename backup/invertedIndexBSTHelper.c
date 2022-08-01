#include "invertedIndexBSTHelper.h"
#include "invertedIndexRoot.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro for max, min
#define max(a, b) (((b) < (a)) ? (a) : (b))
#define mix(a, b) (((a) < (b)) ? (a) : (b))

// ==========================================================================================
//                          Auxiliary Function
// ==========================================================================================
/*
 *  Return height of given node
 */
int invertedIndexHeight(InvertedIndexBST node);

/*
 *  Right rotation at given node. Change tree structure, return new root
 */
InvertedIndexBST invertedIndexRRotation(InvertedIndexBST node);

/*
 *  Left rotation at given node. Change tree structure, return new root.,
 */
InvertedIndexBST invertedIndexLRotation(InvertedIndexBST node);



// ==========================================================================================
//                          User Function
// ==========================================================================================

// Input: string of word
// Output: Empty InvertedIndexBST node with empty fileList, children node
InvertedIndexBST createNewBSTNode(char *new_word) {
    // Initilize tree's own member
	InvertedIndexBST new = malloc(sizeof(*new));

	new->word = malloc(strlen(new_word) + 1);
	strcpy(new->word, new_word);

    new->fileList = NULL;

    // Initialize tree's child
	new->left = NULL;
	new->right = NULL;

    return new;
}


// Input: A InvertedIndexBST node, a FileList node
// Output: Added FileList node at the begin of InvertedIndexBST that have same key member fileList
InvertedIndexBST invertedIndexBSTInsertHelper(InvertedIndexBST root, char *key, FileList addedFile);
void invertedIndexBSTInsert(InvertedIndexRoot root, char *key, FileList addedFile) {
    // Case 1: If invertedIndexBST is empty
    if (root->root == NULL) {
        root->root = createNewBSTNode(key);
        root->root->fileList = addedFile;
        return;
    }
    // Case 2: Use recursion to insert FileList
    root->root = invertedIndexBSTInsertHelper(root->root, key, addedFile);
}

// Uses AVL implementation
InvertedIndexBST invertedIndexBSTInsertHelper(InvertedIndexBST root, char *key, FileList addedFile) {
    // Base case 1: Node do not exist in InvertedIndexBST
    // Create a new node in this spot
    if (root == NULL) {
        InvertedIndexBST new = createNewBSTNode(key);
        new->fileList = addedFile;
        new->fileList->next = NULL;
        return new;
    }

    int cmp = strcmp(root->word, key);
    // Base case 2: Node exist in InvertedIndexBST
    // Insert fileNode
    if (cmp == 0) {
        addedFile->next = root->fileList;
        root->fileList = addedFile;
        return root;
    }

    // Recursive case
    // Traverse left, right for search correct position in invertedIndexBST
    if (cmp > 0) {
        root->left = invertedIndexBSTInsertHelper(root->left, key, addedFile);
    }
    else {
        root->right = invertedIndexBSTInsertHelper(root->right, key, addedFile);
    }

    // AVL-rotation
    int lheight, rheight;
    lheight = invertedIndexHeight(root->left);
    rheight = invertedIndexHeight(root->right);
    if ((rheight - lheight) > 1) {
        int rstrCmp = strcmp(root->right->word, key);
        if (rstrCmp > 0) {
            // Node inserted at the left of right sub-tree
            root->right = invertedIndexRRotation(root->right);
        }
        root = invertedIndexLRotation(root);
    }
    else if ((lheight - rheight) > 1) {
        int lstrCmp = strcmp(root->left->word, key);
        if (lstrCmp < 0) {
            // Node inserted at the right of left sub-tree
            root->left = invertedIndexLRotation(root->left);
        }
        root = invertedIndexRRotation(root);
    }
    return root;
}



// Input: A key and a INvertedIndexBST tree
// Output: The corresponding node for the key in the tree
InvertedIndexBST searchKey(InvertedIndexBST root, char *key) {
	// base case 1 : element do not exist
    if (root == NULL) {
        return NULL;
    }
	// base case 2 : hit element
    if (strcmp(root->word, key) == 0) {
        return root;
    }
    // Recursive case
    if (strcmp(root->word, key) < 0) {
        return searchKey(root->right, key);
    }
    else {
        return searchKey(root->left, key);
    }
}



// ==========================================================================================
//                          Auxiliary Function
// ==========================================================================================
int invertedIndexHeight(InvertedIndexBST node) {
    if (node == NULL) return -1;
    return 1 + max(invertedIndexHeight(node->left), invertedIndexHeight(node->right));
}


InvertedIndexBST invertedIndexRRotation(InvertedIndexBST node) {
    if (node == NULL || node->left == NULL) return node;

    InvertedIndexBST t2 = node->left->right;
    InvertedIndexBST lchild = node->left;
    lchild->right = node;
    node->left = t2;
    return lchild;
}


InvertedIndexBST invertedIndexLRotation(InvertedIndexBST node) {
    if (node == NULL || node->right == NULL) return node;

    InvertedIndexBST t2 = node->right->left;
    InvertedIndexBST rchild = node->right;
    rchild->left = node;
    node->right = t2;
    return rchild;
}