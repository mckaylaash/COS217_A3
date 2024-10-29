#include "symtable.h"
#include <stdlib.h> /* for malloc, calloc, free */
#include <stdio.h> /* for printf */
#include <string.h>
#include <assert.h> /* for assert */



struct Node {
  char *key; /* defensive copy of key (string) */ 
  void *value; /* void pointer to a value (generic) */
  struct Node *next; /* pointer to next node in list */
};

/* single linked list)*/
struct SymTable {
  struct Node *first; /* pointer to first node */
  size_t numNodes; /* number of nodes in the SymTable */
};

/* no parameters, return a new SymTable object with no bindings */
SymTable_T SymTable_new(void) {
    /* declared SymTable_T as a pointer to SymTable in the header file. */
  SymTable_T oSymTable; /* oSymTable is now a pointer to SymTable. */
  oSymTable = calloc(1, sizeof(*oSymTable)); 
  if (oSymTable == NULL) return NULL;
  oSymTable->numNodes = 0;
  return oSymTable;
 }    

void SymTable_free(SymTable_T oSymTable) {
  struct Node *kv;
  struct Node *nextkv;    
  assert (oSymTable != NULL);

  for (kv = oSymTable->first; kv != NULL; kv = nextkv) {
    nextkv = kv->next;
    free(kv->key); /* free defensive copy first */
    free(kv); /* free each node. */
  }
  free(oSymTable); /* free symtable itself. */
}

size_t SymTable_getLength(SymTable_T oSymTable) {
	assert (oSymTable != NULL);
  return oSymTable->numNodes; /* must run in constant time */
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue) {
	struct Node *newNode;

  assert (oSymTable != NULL);
  assert (pcKey != NULL);
  assert (pvValue != NULL);

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
  newNode->value = (void *)pvValue; /* assign given value to new node */
  newNode->next = oSymTable->first; /* bindings are placed in front */
  oSymTable->first = newNode; /* first node becomes the new node */

  return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue) {
  struct Node *kv;
  void *oldValue;

	assert (oSymTable != NULL);
  assert (pcKey != NULL);
  assert (pvValue != NULL);

  for (kv = oSymTable->first; kv != NULL; kv = kv->next) {
    if (strcmp(kv->key, pcKey) == 0) {
      oldValue = kv->value;
      kv->value = (void *)pvValue;
      return oldValue;
    }
  }
  return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
  struct Node *kv;

	assert (oSymTable != NULL);
  assert (pcKey != NULL);

  for (kv = oSymTable->first; kv != NULL; kv = kv->next) {
    if (strcmp(kv->key, pcKey) == 0) return 1;
  }
  return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
  struct Node *kv;

	assert (oSymTable != NULL);
  assert (pcKey != NULL);

  for (kv = oSymTable->first; kv != NULL; kv = kv->next) {
    if (strcmp(kv->key, pcKey) == 0) return kv->value;
  }
  return NULL;
}

/* If oSymTable contains a binding with key pcKey, removes the binding 
from oSymTable and returns the binding’s value. Otherwise, leave 
oSymTable unchanges and returns NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {  
  struct Node *kv;
  void *oldValue;

	assert (oSymTable != NULL);
  assert (pcKey != NULL);

  for (kv = oSymTable->first; kv != NULL; kv = kv->next) {
    if (strcmp(kv->key, pcKey) == 0) {
      oldValue = kv->value;
      free(kv);
      oSymTable->numNodes -= 1;
      return oldValue;
    }
  }
  return NULL;
}


  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra) {
      struct Node *psCurrentNode;
      assert (oSymTable != NULL);
      assert (pfApply != NULL);

      for (psCurrentNode = oSymTable->first;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->next)
        (*pfApply)((char*)psCurrentNode->key, (void*)psCurrentNode->value, (void*)pvExtra);
}



