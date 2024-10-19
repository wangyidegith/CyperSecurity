typedef char* KEY;   // VOLATILE
typedef int VALUE;   // VOLATILE
#define VALUE_ERROR -1   // VOLATILE: if value == poiter, VALUE_ERROR == NULL，else if value == non-poiter, VALUE_ERROR == -1

typedef struct Node Node;
#define TABLE_SIZE 1024   // VOLATILE: == CLIENTS
typedef struct HashTable {   // 该结构定义的目的是为了方便扩展
    Node* buckets[TABLE_SIZE];
} HashTable;

