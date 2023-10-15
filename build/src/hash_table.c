#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"
#include "prime.h"

#define HT_PRIME_1 151
#define HT_PRIME_2 163
#define HT_INITIAL_BASE_SIZE 53

// Define HT_PRIME_1, HT_PRIME_2, and HT_INITIAL_BASE_SIZE if they are not already defined.

static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

ht_hash_table* ht_new() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = 53; // Initialize size as needed
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

static long ht_hash(const char* s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);

    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }
    return hash;
}

static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }

    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;

    while (cur_item != NULL) {
        if (cur_item->key && cur_item->value) {
            if (strcmp(cur_item->key, key) == 0) {
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }

        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }

    ht->items[index] = item;
    ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;

    while (item->key && item->value) {
        if (strcmp(item->key, key) == 0) {
            return item->value;
        }

        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;

    while (item->key && item->value) {
        if (strcmp(item->key, key) == 0) {
            ht_del_item(item);
            ht->items[index] = NULL;
            ht->count--;
            return;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
}

static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = base_size; // Initialize size based on the base_size
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->size = new_ht->size;
    ht->count = new_ht->count;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    free(new_ht->items);
    free(new_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->size * 2;
    ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->size / 2;
    ht_resize(ht, new_size);
}
