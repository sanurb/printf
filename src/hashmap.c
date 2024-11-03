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

// Creates a new key-value pair (bucket), duplicating the key to prevent external modifications.
bucket_t *create_bucket(const char *key, void *value) {
    bucket_t *bucket = malloc(sizeof(bucket_t));
    if (!bucket) return NULL;

    bucket->key = strdup(key);  // Copy key for data integrity within the hashmap.
    bucket->value = value;
    bucket->next = NULL;

    return bucket;
}

// Frees the memory allocated for a bucket, including the duplicated key for safety.
void free_bucket(bucket_t *bucket) {
    if (bucket) {
        free(bucket->key);
        free(bucket);
    }
}

// Creates a hashmap with a given initial capacity.
// Initializes bucket pointers to NULL using calloc for safe access during insertions.
hashmap_t *create_hashmap(size_t initial_capacity) {
    hashmap_t *map = malloc(sizeof(hashmap_t));
    if (!map) return NULL;

    map->capacity = initial_capacity;
    map->size = 0;
    map->buckets = calloc(initial_capacity, sizeof(bucket_t *));  // Ensures NULL pointers for each bucket.
    if (!map->buckets) {
        free(map);
        return NULL;
    }

    return map;
}

// Inserts a key-value pair, updating the value if the key already exists.
// Resolves hash collisions via linked list chaining to handle multiple entries per bucket.
void insert_hashmap(hashmap_t *map, const char *key, void *value) {
    size_t index = hash_function(key) % map->capacity;

    // Traverse linked list at bucket index to locate key if already present.
    bucket_t *bucket = map->buckets[index];
    while (bucket) {
        if (strcmp(bucket->key, key) == 0) {
            bucket->value = value;  // Update value for existing key, preserving unique keys.
            return;
        }
        bucket = bucket->next;
    }

    // Key not found; create and insert a new bucket at the head of the list to optimize insertion.
    bucket = create_bucket(key, value);
    if (!bucket) return;  // Handle memory allocation failure gracefully.

    bucket->next = map->buckets[index];
    map->buckets[index] = bucket;
    map->size++;  // Reflect new entry in the hashmap size.
}

// Retrieves the value associated with a key, or NULL if not found.
// Uses chaining to search within the bucket, necessary for hash collision handling.
void *get_hashmap(hashmap_t *map, const char *key) {
    size_t index = hash_function(key) % map->capacity;

    // Traverse linked list at the index to locate the key.
    bucket_t *bucket = map->buckets[index];
    while (bucket) {
        if (strcmp(bucket->key, key) == 0) {
            return bucket->value;  // Return associated value if key matches.
        }
        bucket = bucket->next;
    }

    return NULL;  // Key not found in the hashmap.
}

// Frees all resources used by the hashmap, including each bucket chain and the map structure itself.
void free_hashmap(hashmap_t *map) {
    if (map) {
        // Free each bucket chain to release all key-value pairs.
        for (size_t i = 0; i < map->capacity; i++) {
            bucket_t *bucket = map->buckets[i];
            while (bucket) {
                bucket_t *next = bucket->next;
                free_bucket(bucket);
                bucket = next;
            }
        }

        free(map->buckets);  // Release the array of bucket pointers.
        free(map);           // Release the hashmap structure itself.
    }
}