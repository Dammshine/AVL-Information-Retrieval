#ifndef RECORD_H
#define RECORD_H

#include <stdbool.h>

typedef struct record *Record;
typedef struct RecordCollection *Collection;
struct RecordCollection {
    Record record;
    Collection next;
};


/**
 * Create a record stored a word
 * char *key will not be used in Record
 * free to use key variable in other place
 */
Record RecordNew(char *key);


/*
 * Create a copy of record from record
 */
Record RecordCopy(Record record);

/**
 * Frees all memory allocated to the given record
 */
Record RecordFree(Record r);

/**
 * Return the word used as the key
 */
char *RecordGetKey(Record r);

/**
 * Return the the value stored in record
 * In dictionary adt, it's an int
 * Denote the count of appearance of given (key) word
 */
int RecordGetValue(Record r);

/**
 * Increment, decrement the value associate with count
 */
void RecordIncrement(Record r);
void RecordDecrement(Record r);

/**
 * Use case, when insert duplicate record
 * Combine two record
 * Free all memory associated with rhs one
 */
void RecordCombine(Record lhs, Record rhs);


/**
 * Display as format 
 * ("%s %d\n", word, count)
 */
void RecordShow(Record r);


/**
 * Compare function
 * Return positive if lhs is greater
 * Return 0 if equal
 * Return negative if lhs is smaller
 */
int RecordCompare(Record lhs, Record rhs);


/**
 * Create a new Collection
 */
Collection CollectionNew(Record rec);

// Free collection
// If flag == true, also free record
Collection freeCollection(Collection head, bool flag);

#endif