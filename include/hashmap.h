//
// Created by sanurb on 9/20/2024.
//

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

// A bucket represents a single key-value pair in the hashmap.
// The bucket is part of a linked list to handle collisions.
typedef struct bucket {
    char *key;             // Key for the entry
    void *value;           // Associated value
    struct bucket *next;   // Pointer to the next bucket in case of collisions
} bucket_t;


// The hashmap structure contains an array of buckets, with
// a defined capacity and current size (number of key-value pairs).
typedef struct {
    size_t capacity;       // Total number of slots in the hashmap
    size_t size;           // Current number of key-value pairs stored
    bucket_t **buckets;    // Array of bucket pointers (linked lists)
} hashmap_t;

hashmap_t *create_hashmap(size_t initial_capacity);
void insert_hashmap(hashmap_t *map, const char *key, void *value);
void *get_hashmap(hashmap_t *map, const char *key);
void free_hashmap(hashmap_t *map);

size_t hash_function(const char *key);

bucket_t *create_bucket(const char *key, void *value);
void free_bucket(bucket_t *bucket);

#endif //HASHMAP_H
