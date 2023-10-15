#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"

int main() {
    ht_hash_table* ht = ht_new();
    
    // Use hash table
    ht_insert(ht, "key1", "value1");
    char* result = ht_search(ht, "key1");
    if (result) {
        printf("Value: %s\n", result);
    } else {
        printf("Key not found\n");
    }

    ht_delete(ht, "key1");

    ht_del_hash_table(ht);
    return 0;
}
