#include <stdlib.h>
#include <string.h>
#include "../include/hashmap.h"

// DJB2 hash function, a commonly used and efficient hash function for strings.
// See: https://stackoverflow.com/questions/7666509/hash-function-for-string
size_t hash_function(const char *key) {
    size_t hash = 5381;
    int c;

    // Process each character in the string and update the hash.
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash;
}

// Creates a new bucket (key-value pair) with the provided key and value.
// The key is duplicated to avoid accidental modifications.
bucket_t *create_bucket(const char *key, void *value) {
    bucket_t *bucket = malloc(sizeof(bucket_t));
    if (!bucket) return NULL;

    bucket->key = strdup(key);  // Duplicate the key to avoid external changes.
    bucket->value = value;
    bucket->next = NULL;

    return bucket;
}

// Frees the memory allocated for a bucket, including the key.
void free_bucket(bucket_t *bucket) {
    if (bucket) {
        free(bucket->key);  // Free the duplicated key.
        free(bucket);       // Free the bucket structure.
    }
}

// Creates a hashmap with the specified initial capacity.
// Buckets are initialized to NULL using calloc.
hashmap_t *create_hashmap(size_t initial_capacity) {
    hashmap_t *map = malloc(sizeof(hashmap_t));
    if (!map) return NULL;

    map->capacity = initial_capacity;
    map->size = 0;
    map->buckets = calloc(initial_capacity, sizeof(bucket_t *));  // Initialize all bucket pointers to NULL.
    if (!map->buckets) {
        free(map);
        return NULL;
    }

    return map;
}

// Inserts a key-value pair into the hashmap. If the key already exists, its value is updated.
// Collisions are handled by chaining (linked lists).
void insert_hashmap(hashmap_t *map, const char *key, void *value) {
    size_t index = hash_function(key) % map->capacity;  // Get the bucket index from the hash.

    // Traverse the chain (linked list) to check if the key already exists.
    bucket_t *bucket = map->buckets[index];
    while (bucket) {
        if (strcmp(bucket->key, key) == 0) {
            bucket->value = value;  // Update the value if the key is found.
            return;
        }
        bucket = bucket->next;
    }

    // If the key does not exist, create a new bucket and add it to the chain.
    bucket = create_bucket(key, value);
    if (!bucket) return;  // Handle memory allocation failure.

    // Insert the new bucket at the beginning of the chain.
    bucket->next = map->buckets[index];
    map->buckets[index] = bucket;
    map->size++;  // Increment the size of the map (number of key-value pairs).
}

// Retrieves the value associated with the given key from the hashmap.
// Returns NULL if the key is not found.
void *get_hashmap(hashmap_t *map, const char *key) {
    size_t index = hash_function(key) % map->capacity;  // Get the bucket index from the hash.

    // Traverse the chain to find the key.
    bucket_t *bucket = map->buckets[index];
    while (bucket) {
        if (strcmp(bucket->key, key) == 0) {
            return bucket->value;  // Return the value if the key is found.
        }
        bucket = bucket->next;
    }

    return NULL;  // Key not found.
}

// Frees all memory associated with the hashmap, including all buckets and the map itself.
void free_hashmap(hashmap_t *map) {
    if (map) {
        // Free each bucket chain.
        for (size_t i = 0; i < map->capacity; i++) {
            bucket_t *bucket = map->buckets[i];
            while (bucket) {
                bucket_t *next = bucket->next;
                free_bucket(bucket);  // Free each bucket in the chain.
                bucket = next;
            }
        }

        free(map->buckets);  // Free the array of bucket pointers.
        free(map);           // Free the hashmap structure itself.
    }
}