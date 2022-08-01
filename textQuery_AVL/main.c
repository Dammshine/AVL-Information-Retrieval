#include "record.h"
#include "textQuery.h"
#include <stdio.h>
#include <string.h>

int main() {
    textQuery queue = createTextQuery("sample2", false);
    queue = freeQuery(queue, true);
    if (queue == NULL) printf("NULL");
    queue = createTextQuery("sample1", false);
    queue = freeQuery(queue, true);
    return 0;
}