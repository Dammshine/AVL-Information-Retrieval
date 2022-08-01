#include "record.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct record {
    char *key;          // a string used for 
    int value;          // a int to store the count of word (key)
};



// ======================================================
//                    TextQuery
// ======================================================
/**
 * Create a record stored a word
 */
Record RecordNew(char *key) {
    Record new = malloc(sizeof(*new));
    new->key = malloc(strlen(key) + 1);
    strcpy(new->key, key);
    new->value = 1;
    return new;
}


/*
 * Create a copy of record from record
 */
Record RecordCopy(Record record) {
    Record duplicateRecord = RecordNew(record->key);
    duplicateRecord->value = record->value;
    return duplicateRecord;
}

/**
 * Frees all memory allocated to the given record
 */
Record RecordFree(Record r) {
    if (r == NULL) return NULL;
    free(r->key);
    free(r);
    return NULL;
}


/**
 * Return the word used as the key
 */
char *RecordGetKey(Record r) {
    if (r == NULL) return NULL;
    return r->key;
}

/**
 * Return the the value stored in record
 * In dictionary adt, it's an int
 * Denote the count of appearance of given (key) word
 */
int RecordGetValue(Record r) {
    return r->value;
}

/**
 * Increment, decrement the value associate with count
 */
void RecordIncrement(Record r) {
    if (r == NULL) return;
    r->value++;
}

void RecordDecrement(Record r) {
    if (r == NULL) return;
    r->value--;
}

/**
 * Use case, when insert duplicate record
 * Combine two record
 * Free all memory associated with rhs one
 */
void RecordCombine(Record lhs, Record rhs) {
    if (lhs == NULL || rhs == NULL) return;
    lhs->value += rhs->value;
    RecordFree(rhs);
}


/**
 * Display as format 
 * ("%s %d\n", word, count)
 */
void RecordShow(Record r) {
    if (r == NULL) return;
    printf("%s %d\n", r->key, r->value);
}



/**
 * Compare function
 * Return positive if lhs is greater
 * Return 0 if equal
 * Return negative if lhs is smaller
 */
int RecordCompare(Record lhs, Record rhs) {
    if (lhs == NULL || rhs == NULL) {
        fprintf(stderr, "Compareing invalid record(Record points to NULL)");
        exit(0);
    }
    return strcmp(rhs->key, lhs->key);
}

// ======================================================
//                    Collection
// ======================================================

/**
 * Create a new Collection
 */
Collection CollectionNew(Record rec) {
    Collection new = malloc(sizeof(*new));
    new->record = rec;
    new->next = NULL;
    return new;
}

// Free collection
// If flag == true, also free record
Collection freeCollection(Collection head, bool flag) {
    Collection temp = head;
    while (head != NULL) {
        temp = head;
        head = head->next;
        if (flag == true) temp->record = RecordFree(temp->record);
        //printf("%p \n", temp);
        free(temp);
    }
    return NULL;
}
