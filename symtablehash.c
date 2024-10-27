#include "symtable.h"
#include <stdlib.h> /* for malloc, calloc, free */
#include <stdio.h> /* for printf */

struct Node {
    const char *key; /* Pointer to a key (string) */ 
    void *value; /* Void pointer to a value (generic) */
    struct Node *next; /* Pointer to next node in list */
};

struct SymTable { /* manager */
   /*  to struct Node *buckets[] where buckets is an array full of 
   pointers to nodes */
    struct Node **buckets; /* Pointer to (pointer to a node) */
    size_t numBuckets; /* Number of avaliable entries in the hash table*/
    size_t numNodes; 
};

/* Array of bucket counts used in hash table expansion  */
static const size_t auBucketCounts[] = { 509, 1021, 2039, 4093, 8191, 
16381, 32749, 65521};
static const size_t numBucketIncrements = sizeof(auBucketCounts)/sizeof(auBucketCounts[0]);

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}
      
SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;

    /* allocate memory for SymTable itself */
    oSymTable = calloc(1, sizeof(*oSymTable));
    if (oSymTable == NULL) cry(); return NULL;

    /* allocate memory for buckets array, starting at 509 buckets.*/
    oSymTable->buckets = calloc(auBucketCounts[0],sizeof(*oSymTable->buckets));
    if (oSymTable->buckets == NULL) cry(); return NULL;
    oSymTable->numBuckets = auBucketCounts[0];
    oSymTable->numNodes = 0;
    return oSymTable;
}

  void SymTable_free(SymTable_T oSymTable) {
    size_t bucket;
    struct Node **nextbucket;
    struct Node **nextkv;

    assert (oSymTable != NULL);

    for (oSymTable->buckets; nextbucket != NULL; nextbucket = nextkv) {
      nextkb
    
    
    nextkv = kv->next;
    free(kv->key); /* free defensive copy first */
    free(kv); /* free each node. */
  }
  free(oSymTable); /* free symtable itself. */


  }

  size_t SymTable_getLength(SymTable_T oSymTable);

  int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

  void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

  int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

  void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

  void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);