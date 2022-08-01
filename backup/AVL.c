#include <stdlib.h>
#include <stdio.h>
#include "record.h"
#include "AVL.h"




typedef struct node *Node;

struct node {
    Record element;
    Node left, right;
}; 

struct tree {
    Node root;
    int (*compare)(Record, Record);
};


// ===============================================================================================================
// Auxiliary functions
Node leftRotation(Node node);
Node rightRotation(Node node);

// Node root is garunteed to be usable state (CleanNode)
// Join left, right tree and store too root in root, 
// link root with lhs, rhs
Node treeJoin(Node lhs, Node rhs, Node root);

// Return true if after free, the node is in state of use
// Return false if the node can no longer be used
bool cleanNode(Node node);
void TreeListInOrder_helper(Node node);
// ===============================================================================================================
// User functions




int Max(int a, int b) {
    return (a >= b) ? a : b;
}



int height(Node node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + Max(height(node->left), height(node->right));
}

Node NodeNew(Record record) {
    Node new = malloc(sizeof(*new));
    new->element = record;
    new->left = NULL;
    new->right = NULL;
    return new;
}




/**
 * Creates a new tree that will use the given comparison function
 */
Tree TreeNew(int (*compare)(Record, Record)) {
    Tree t = malloc(sizeof(*t));
    t->root = NULL;
    t->compare = compare;
    return t;
}

/**
 * Frees the given tree. If freeRecords is true, also frees the records.
 */
void TreeFreeHelper(Node t, bool freeRecords) {
    // base case
    if (t == NULL) {
        return;
    }
    TreeFreeHelper(t->left, freeRecords);
    TreeFreeHelper(t->right, freeRecords);
    if (freeRecords == true) {
        t->element = RecordFree(t->element);
    }
    free(t);
}

Tree TreeFree(Tree t, bool freeRecords) {
    if (t == NULL) return NULL;
    TreeFreeHelper(t->root, freeRecords);
    free(t);
    return NULL;
}

/**
 * Inserts the given record. Returns true if the record was inserted
 * successfully, or false if there was already a record that compares
 * equal to the given record in the tree (according to the comparison
 * function).
 */
// AVL Implementation
Node TreeInsert_helper(Tree t, Node node, Record rec) {
    // Base case 1: did not find rec
    if (node == NULL) {
        
        Node new = NodeNew(rec);
        return new;
    } // Base case 2: find the element
    else if (t->compare(node->element , rec) == 0) {
        RecordIncrement(node->element);
        RecordFree(rec);
        return node;
    }
    else {
        int comparison = t->compare(node->element , rec);
        if (comparison < 0) {
            node->right = TreeInsert_helper(t, node->right, rec);
        }
        else {
            node->left = TreeInsert_helper(t, node->left, rec);
        }  
        // AVL-Rotation
        int lheight = height(node->left);
        int rheight = height(node->right);
        if ((lheight - rheight) > 1) {
            // If element inserted at the right branch of left child of node
            if (t->compare(node->left->element, rec) > 0) {
                node->left = leftRotation(node->left);
            }
            node = rightRotation(node);
        }
        else if ((rheight - lheight) > 1) {
            // If element inserted at the left branch of right child of node
            if (t->compare(node->right->element, rec) < 0) {
                node->right = rightRotation(node->right);
            }
            node = leftRotation(node);
        }
        return node;
    }
}


bool TreeInsert(Tree t, Record rec) {
    if (t->root == NULL) {
        // create new node for tree
        Node new = NodeNew(rec);
        t->root = new;
    }
    else {
        TreeInsert_helper(t, t->root, rec);
    }
    // In dictionary all insert is valid hence return true
    return true;
}

/**
 * Deletes the record that compares equal to the given record (according
 * to the comparison function). Returns true if the record was deleted,
 * or false if there was no record that compared equal to the given
 * record.
 */
Node TreeDelete_helper(Tree t, Node node, Record rec, bool *result) {
    // Base case 1: No element found
    if (node == NULL) {
        *result = false;
        return node;
    }
    // Base case 2: Found the element
    // Delete element and join two tree
    if (t->compare(node->element, rec) == 0) {
        // Decrement cureent node
        RecordDecrement(node->element);
       
        // Delete element
        // Join two tree, return the new root
        if (RecordGetValue(node->element) <= 0) {
            cleanNode(node);
            node = treeJoin(node->left, node->right, node);
            if (node == NULL) return node;
            int lheight = height(node->left);
            int rheight = height(node->right);
            if ((rheight - lheight) > 1) {
                node = leftRotation(node);
            }
            return node;
        }
        // Return node
        return node;
    }

    // Recursive case
    else {
        int cmp = t->compare(node->element, rec);
        if (cmp > 0) {
            node->left = TreeDelete_helper(t, node->left, rec, result);
        }
        else {
            node->right = TreeDelete_helper(t, node->right, rec, result);
        }
        

        // AVL-Rotation
        int lheight = height(node->left);
        int rheight = height(node->right);
        if ((lheight - rheight) > 1) {
            node = rightRotation(node);
        }
        else if ((rheight - lheight) > 1) {
            node = leftRotation(node);
        }
        return node;
    }
}


bool TreeDelete(Tree t, Record rec) {
    if (t->root == NULL) {
        return false;
    }
    bool ret = true;
    t->root = TreeDelete_helper(t, t->root, rec, &ret);
    return ret;
}

/**
 * Searches for a record that compares equal to the given record
 * (according to the comparison function). Returns the record if it
 * exists, or NULL otherwise.
 */
Record TreeSearch_helper(Tree t, Node node, Record rec);

Record TreeSearch(Tree t, Record rec) {
    if (t == NULL || t->root == NULL) return NULL;
    return TreeSearch_helper(t, t->root, rec);
}

Record TreeSearch_helper(Tree t, Node node, Record rec) {
    if (node == NULL) {
        return NULL;
    }
    
    int cmp = t->compare(node->element, rec);
    if (cmp == 0) {
        return node->element;
    } // Recursive case
    else if (cmp > 0) {
        return TreeSearch_helper(t, node->left, rec);
    }
    else {
        return TreeSearch_helper(t, node->right, rec);
    }
}

// Remove a "random" (funny, I won't do that) record stored in AVL tree
// Return the removed AVL tree
// Return NULL if the Tree is empty
Record TreeRemoveAny(Tree t) {
    if (t != NULL && t->root != NULL) return t->root->element;
    return NULL;
}



/**
 * Displays all records in the given tree in order, one per line
 */
void TreeListInOrder(Tree t) {
    if (t == NULL || t->root == NULL) return;
    TreeListInOrder_helper(t->root); 
}

void TreeListInOrder_helper(Node node) {
    if (node == NULL) return;
    TreeListInOrder_helper(node->left);
    printf("%s %d\n", RecordGetKey(node->element), RecordGetValue(node->element));
    TreeListInOrder_helper(node->right);
}

/**
 * Output all records in the linked list
 */
typedef struct CollectionHead *CollectionWrapper;
struct CollectionHead {
    Collection h;
};

void TreeRemoveAllHelper(CollectionWrapper head, Node node) {
    if (node == NULL || node->element == NULL) return;
    TreeRemoveAllHelper(head, node->left);
    TreeRemoveAllHelper(head, node->right);
    if (head->h == NULL) {
        head->h = CollectionNew(node->element);
    }
    else {
        Collection new = CollectionNew(node->element);
        new->next = head->h;
        head->h = new;
    }
}

Collection TreeRemoveAll(Tree t) {
    if (t == NULL) return NULL;
    CollectionWrapper cw = malloc(sizeof(*cw));
    cw->h = NULL;
    TreeRemoveAllHelper(cw, t->root);
    Collection ret = cw->h;
    free(cw);
    return ret;
}


// Join two tree, return new root node
// Node root is garuanteed to be cleaned
Node treeJoin(Node lhs, Node rhs, Node root) {
    // Case 1: Two child both NULL
    if (lhs == NULL && rhs == NULL) {
        // Free root, as it's the only node and being deleted
        free(root);
        return NULL;
    } // Case 2: One of two children is NULL. Free root node, return not NULL children
    else if (lhs == NULL) {
        free(root);
        return rhs;
    }
    else if (rhs == NULL) {
        free(root);
        return lhs;
    } // Case 3: Both not NULL, replace smallest in rhs tree with the root
    else {    // Left rotation on rhs root to maintain balance
        Node curr = rhs;
        Node parent = NULL;
        // Find the minimum node in rhs tree
        // Find the parent of minimum node in rhs
        while (curr->left != NULL) {
            parent = curr;
            curr = curr->left;
        }

        // case 3.1: rhs is the lowest of rhs tree
        if (parent == NULL) {
            // replace root node with rhs node
            root->element = rhs->element;
            root->right = rhs->right;
            // Delete rhs
            rhs->element = NULL;
            free(rhs);
            // Right rotation on root(remove a node on right of root)
            int lheight = height(root->left->left);
            int rheight = height(root->left->right);
            if (rheight - lheight > 0) {
                root->left = leftRotation(root->left);
            }
            root = rightRotation(root);
            return root;
        } // case 3.2: Replace minimum in rhs with the new root
        else {
            //  Unlink the minimum node in rhs, right rotation on parent (remove a node on left of parent)
            parent->left = curr->right;
            parent->right = rightRotation(parent);
            int lheight = height(parent->left);
            int rheight = height(parent->right);
            if (rheight - lheight > 1) {
                parent = leftRotation(root->left);
            }
            
            // Replace minimum
            root->element = curr->element;
            free(curr);
            return root;
        }
    }
}

Node rightRotation(Node node) {
    // If left is empty
    if (node->left == NULL) {
        return node;
    }

    // Replace curr->left with curr->left->right
    // Replace the order of two node
    Record curr_rec = node->element;
    Record left_rec = node->left->element;
    Node ltree = node->left;
    
    Node t1 = ltree->left;
    Node t2 = ltree->right;
    Node t3 = node->right;

    // Swap record
    node->element = left_rec;
    ltree->element = curr_rec;


    node->right = ltree;
    node->left = t1;
    ltree->left = t2;
    ltree->right = t3;
    return node;
}

Node leftRotation(Node node) {
    // If right is empty
    if (node->right == NULL) {
        return node;
    }

    // Replace curr->left with curr->left->right
    // Replace the order of two node
    Record curr_rec = node->element;
    Record right_rec = node->right->element;
    Node rtree = node->right;
    
    Node t1 = node->left;
    Node t2 = rtree->left;
    Node t3 = rtree->right;

    // Swap record
    node->element = right_rec;
    rtree->element = curr_rec;

    node->left = rtree;
    node->right= t3;
    rtree->left = t1;
    rtree->right = t2;
    return node;
}


// Free memory associated with node but not freeing node
bool cleanNode(Node node) {
    if (node == NULL) {
        return false;
    }
    if (node->element == NULL) {
        return true;
    }
    free(node->element);
    return true;
}