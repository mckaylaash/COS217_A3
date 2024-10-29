
#include "symtable.h"
#include <stdlib.h> /* for malloc, calloc, free */
/* #include <stdio.h> /* for printf */
#include <string.h>
#include <assert.h> /* for assert */

struct Node {
    const char *key; /* Pointer to a key (string) */ 
    void *value; /* Void pointer to a value (generic) */
    struct Node *next; /* Pointer to next node in list */
};

struct SymTable { /* manager */
   /*  to struct Node *buckets[] where buckets is an array full of 
   pointers to nodes */
    struct Node **buckets; /* Pointer to (pointer to a node) */
    size_t numBuckets; /* Number of buckets in the hash table */
    size_t numNodes; /* Number of key-value pairs in table*/
};

/* Array of bucket counts used in hash table expansion  */
static const size_t auBucketCounts[] = { 509, 1021, 2039, 4093, 8191, 
16381, 32749, 65521};
static const size_t numBucketIncrements = sizeof(auBucketCounts) / 
                     sizeof(auBucketCounts[0]);

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
      

/* Takes a SymTable object oSymTable and the number of buckets in the 
SymTable, a size_t object uBucketCount, as input. Repositions all 
bindings in the given oSymTable according to hash codes between 0 and 
uBucketCount - 1. Does not return any values. */
void rehash(SymTable_T oSymTable, size_t uBucketCount) {
   size_t i;
   size_t bucket;
   struct Node *kv;
   struct Node *nextkv;
   struct Node **newBuckets;

   assert (oSymTable != NULL);
   assert (uBucketCount != NULL);

   /* allocate memory for buckets array, starting at uBucketCount 
    buckets at the size of a pointer of a struct.*/
   newBuckets = calloc(uBucketCount, sizeof(*oSymTable->buckets));
   /* exit function if allocation failure */
   if (newBuckets == NULL) return; 

   for (i = 0; i < oSymTable->numBuckets; i++) {
      kv = oSymTable->buckets[i];
      while (kv != NULL) {
         bucket = SymTable_hash(kv->key, uBucketCount); /* hash code */
         nextkv = kv->next;

         /* bindings are placed in front */
         kv->next = newBuckets[bucket];

         /* first node becomes the new node */
         newBuckets[bucket] = kv; 

         free(kv->key); /* free defensive copy first */
         free(kv); /* free each node. */

         kv = nextkv;
      }
   }
   oSymTable->buckets = newBuckets;
   oSymTable->numBuckets = uBucketCount;
}


SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;

    /* allocate memory for SymTable itself */
    oSymTable = calloc(1, sizeof(*oSymTable));
    if (oSymTable == NULL) return NULL;

    /* allocate memory for buckets array, starting at 509 buckets at the 
    size of a pointer of a struct.*/
    oSymTable->buckets = calloc(auBucketCounts[0], 
      sizeof(*oSymTable->buckets));

    if (oSymTable->buckets == NULL) return NULL;

    oSymTable->numBuckets = auBucketCounts[0];
    oSymTable->numNodes = 0;
    oSymTable->numBuckets = 0;

    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
   size_t bucket;
   struct Node *kv;
   struct Node *nextkv;

   assert (oSymTable != NULL);

   for (bucket = 0; bucket < oSymTable->numBuckets; bucket++) {
      kv = oSymTable->buckets[bucket];
      while (kv != NULL) {
         nextkv = kv->next;
         free(kv->key); /* free defensive copy first */
         free(kv); /* free each node. */
         kv = nextkv;
      }
    }
    free(oSymTable->buckets); /* free symtable pointer to a pointer */
    free(oSymTable); /* free symtable itself. */
  }

size_t SymTable_getLength(SymTable_T oSymTable) {
   assert (oSymTable != NULL);
   return oSymTable->numNodes;
  }

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
  const void *pvValue) {
   size_t bucket;
   struct Node *newNode;
   size_t currentBucket;

   assert (oSymTable != NULL);
   assert (pcKey != NULL);
   assert (pvValue != NULL);

   /* Hash Table Expansion */
   currentBucket = oSymTable->numBuckets;
   /* checks if the number of nodes equals or exceeds the current number 
   of buckets. if so, proceeds with execution. */
   if (oSymTable->numNodes >= auBucketCounts[currentBucket]) {
      /* make sure we are not at maximum number of buckets (65521) */
      if (currentBucket < (numBucketIncrements - 1)) { 
        /* move to the next element in the array */
        oSymTable->numBuckets++; 
        rehash(oSymTable, auBucketCounts[oSymTable->numBuckets]);
      }
   }

   /* allocate space for node */
   newNode = (struct Node*) calloc(1, sizeof(struct Node));
   if (newNode == NULL) return 0;
   oSymTable->numNodes += 1; /* increase number of nodes */

   /* allocate space for key defensive copy */
   newNode->key = calloc(strlen(pcKey) + 1, sizeof(char));
   if (newNode->key == NULL) {
     free(newNode);
    return 0;
   }

   strcpy(newNode->key, pcKey);
   /* assign given value to new node */
   newNode->value = (void *)pvValue; 

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);

   /* bindings are placed in front */
   newNode->next = oSymTable->buckets[bucket];

   /* first node becomes the new node */
   oSymTable->buckets[bucket] = newNode; 

   return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
 const void *pvValue) {
   size_t bucket;
   struct Node *kv;
   void *oldValue;

	assert (oSymTable != NULL);
   assert (pcKey != NULL);
   assert (pvValue != NULL);

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);
   kv = oSymTable->buckets[bucket];
      while (kv != NULL) {
         if (strcmp(kv->key, pcKey) == 0) {
            oldValue = kv->value;
            kv->value = (void *)pvValue;
            return oldValue;
         }
      kv = kv->next;
      }
  return NULL;
 }
  
int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
   struct Node *kv;
   size_t bucket;

	assert (oSymTable != NULL);
   assert (pcKey != NULL);

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);
   kv = oSymTable->buckets[bucket];
   
   while (kv != NULL) {
         if (strcmp(kv->key, pcKey) == 0) return 1;
         kv = kv->next;
      }
  return 0;
  }

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
   struct Node *kv;
   size_t bucket;

	assert (oSymTable != NULL);
   assert (pcKey != NULL);

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);
   kv = oSymTable->buckets[bucket];
   
   while (kv != NULL) {
      if (strcmp(kv->key, pcKey) == 0) return kv->value;
      kv = kv->next;
  }
  return NULL;
  }

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
   struct Node *kv;
   size_t bucket;
   void *oldValue;

	assert (oSymTable != NULL);
   assert (pcKey != NULL);

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);
   kv = oSymTable->buckets[bucket];
   while (kv != NULL) {
      if (strcmp(kv->key, pcKey) == 0) {
         oldValue = kv->value;
         free(kv);
         oSymTable->numNodes -= 1;
         return oldValue;
      }
      kv = kv->next;
      }
  return NULL;
  }

  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra) {

      size_t bucket;
      struct Node *psCurrentNode;

      assert (oSymTable != NULL);
      assert (pfApply != NULL);

      for (psCurrentNode = )


      
     }

    