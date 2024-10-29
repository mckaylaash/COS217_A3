
#include "symtable.h"
#include <stdlib.h> /* for malloc, calloc, free */
#include <string.h>
#include <assert.h> /* for assert */

/* An object that stores the key value pairs in the SymTable.*/
struct Node {
   /* Pointer to a key (string) */ 
    char *key; 
   /* Void pointer to a value (generic) */
    const void *value; 
   /* Pointer to next node in list */
    struct Node *next;
};

/* A Hash Table implementation of a Symbol Table containing a 
struct Node **buckets, size_t numBuckets, and size_t numNodes. */
struct SymTable { /* manager */
   /*  Buckets is an array full of pointers to nodes */
    struct Node **buckets; 
    /* Number of buckets in the hash table */
    size_t numBuckets; 
    /* Number of key-value pairs in table*/
    size_t numNodes; 
};

/* Array of bucket counts used in hash table expansion  */
static const size_t auBucketCounts[] = { 509, 1021, 2039, 4093, 8191, 
16381, 32749, 65521};
/* Stores the size of the auBucketCounts array. */
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
   struct Node *binding;
   struct Node *nextbinding;
   struct Node **newBuckets;

   assert (oSymTable != NULL);
   /* assert (uBucketCount != NULL); */

   /* allocate memory for buckets array, starting at uBucketCount 
    buckets at the size of a pointer of a struct.*/
   newBuckets = calloc(uBucketCount, sizeof(*oSymTable->buckets));
   /* exit function if allocation failure */
   if (newBuckets == NULL) return; 

   for (i = 0; i < oSymTable->numBuckets; i++) {
      binding = oSymTable->buckets[i];
      while (binding != NULL) {
         bucket = SymTable_hash(binding->key, uBucketCount); /* hash code */
         nextbinding = binding->next;

         /* bindings are placed in front */
         binding->next = newBuckets[bucket];

         /* first node becomes the new node */
         newBuckets[bucket] = binding; 

         free(binding->key); /* free defensive copy first */
         free(binding); /* free each node. */

         binding = nextbinding;
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
   struct Node *binding;
   struct Node *nextbinding;

   assert (oSymTable != NULL);

   for (bucket = 0; bucket < oSymTable->numBuckets; bucket++) {
      binding = oSymTable->buckets[bucket];
      while (binding != NULL) {
         nextbinding = binding->next;
         free(binding->key); /* free defensive copy first */
         free(binding); /* free each node. */
         binding = nextbinding;
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

   if (SymTable_contains(oSymTable, pcKey) == 1) return 0;

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

   /* allocate space for key defensive copy */
   newNode->key = calloc(strlen(pcKey) + 1, sizeof(char));
   if (newNode->key == NULL) {
    free(newNode);
    return 0;
   }

   oSymTable->numNodes += 1; /* increase number of nodes */
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
   struct Node *binding;
   const void *oldValue;

	assert (oSymTable != NULL);
   assert (pcKey != NULL);
   /* assert (pvValue != NULL); */

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);
   binding = oSymTable->buckets[bucket];
      while (binding != NULL) {
         if (strcmp(binding->key, pcKey) == 0) {
            oldValue = binding->value;
            binding->value = (void *)pvValue;
            return (void *)oldValue;
         }
      binding = binding->next;
      }
  return NULL;
 }
  
int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
   struct Node *binding;
   size_t bucket;

	assert (oSymTable != NULL);
   assert (pcKey != NULL);

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);
   binding = oSymTable->buckets[bucket];
   
   while (binding != NULL) {
         if (strcmp(binding->key, pcKey) == 0) return 1;
         binding = binding->next;
      }
  return 0;
  }

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
   struct Node *binding;
   size_t bucket;

	assert (oSymTable != NULL);
   assert (pcKey != NULL);

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);
   binding = oSymTable->buckets[bucket];
   
   while (binding != NULL) {
      if (strcmp(binding->key, pcKey) == 0) return (void *)binding->value;
      binding = binding->next;
  }
  return NULL;
  }

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
   struct Node *binding;
   size_t bucket;
   struct Node *previous;
   const void *oldValue;

	assert (oSymTable != NULL);
   assert (pcKey != NULL);

   bucket = SymTable_hash(pcKey, auBucketCounts[oSymTable->numBuckets]);
   binding = oSymTable->buckets[bucket];
   previous = NULL;

   while (binding != NULL) {
    if (strcmp(binding->key, pcKey) == 0) {
      oldValue = binding->value;

      if (previous == NULL) oSymTable->buckets[bucket] = binding->next;
      else previous->next = binding->next;

      free(binding->key); /* free defensive copy first */
      free(binding);
      oSymTable->numNodes -= 1;
      return (void *) oldValue;
    }
    previous = binding;
    binding = binding->next;    
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

      for (bucket = 0; bucket < auBucketCounts[oSymTable->numBuckets]; bucket++) {
         for (psCurrentNode = oSymTable->buckets[bucket];
         psCurrentNode != NULL;
         psCurrentNode = psCurrentNode->next)
        (*pfApply)((char*)psCurrentNode->key, (void*)psCurrentNode->value, (void*)pvExtra);
      }
   }

    