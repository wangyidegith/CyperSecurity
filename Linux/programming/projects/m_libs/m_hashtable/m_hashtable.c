#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

typedef struct Node {
    KEY key;
    VALUE value;
    struct Node* next;
} Node;

static int hashFunction(int hash_seed) {   // hash_seed 的目的是为了随机化，可以是 key 或者 key 的属性，也可以是一个固定值，但是哈希表操作应该追求速度，因而hashFuction应该尽量简易
    return hash_seed % TABLE_SIZE;
}

HashTable* initHashTable() {
    HashTable* hashtable = (HashTable*)calloc(sizeof(HashTable), sizeof(char));
    return hashtable;
}

void insert(HashTable* hashtable, KEY key, VALUE value, int hash_seed) {
    // 1 get hash
    int index = hashFunction(hash_seed);
    // 2 create new_node
    Node* new_node = (Node*)calloc(sizeof(Node), sizeof(char));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    // 3 insert head
    if (hashtable->buckets[index] != NULL) {
        new_node->next = hashtable->buckets[index];
    }
    hashtable->buckets[index] = new_node;
}

int delete(HashTable* hashtable, KEY key, size_t key_len, int hash_seed) {
    // 1 get hash
    int index = hashFunction(hash_seed);
    // 2 search in bucket
    Node* cur = hashtable->buckets[index];
    Node* prev = NULL;
    while (cur == NULL && memcmp((void*)(cur->key), (void*)key, key_len)) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) {
        return -1;
    }
    // 3 found
    // 3(1) list process
    if (prev == NULL) {
        hashtable->buckets[index] = cur->next;
    } else {
        prev->next = cur->next;
    }
    // 3(2) free node
    free(cur);
    return 0;
}

VALUE search(HashTable* hashtable, KEY key, size_t key_len, int hash_seed) {
    // 1 get hash
    int index = hashFunction(hash_seed);
    // 2 search in bucket
    Node* cur = hashtable->buckets[index];
    while (cur != NULL) {
        if (memcmp((void*)(cur->key), (void*)key, key_len)) {
            cur = cur->next;
            continue;
        }
        // found
        return cur->value;
    }
    // 3 not found
    return VALUE_ERROR;
}

#define IS_POINTER_TYPE(type) ((type*)0 == (void*)0)
int update(HashTable* hashtable, KEY key, VALUE value, size_t key_len, size_t value_len, int hash_seed) {
    // 1 serach
    VALUE target = search(hashtable, key, key_len, hash_seed);
    // 2 alter
    if (target == VALUE_ERROR) {
        // not found
        return -1;
    } else {
        // found, to alter
        if (IS_POINTER_TYPE(VALUE)) {
            // memcpy((void*)target, (void*)value, value_len);   // VOLATILE: 本来是想着写一个类型判断，但是类型本身已经固定了啊，所以你还是换吧
        } else {
            memcpy((void*)&target, (void*)&value, value_len);
        }
        return 0;
    }
}

void destroyHashTable(HashTable* hashtable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* cur = hashtable->buckets[i];
        while (cur != NULL) {
            Node* temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    free(hashtable);
}

