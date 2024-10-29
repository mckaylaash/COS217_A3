/*--------------------------------------------------------------------*/
/* symtablelist.c                                                              */
/* Author: Mckayla Ashley  

SymTable Abstract Data Type Linked List Implementation                                               */
/*--------------------------------------------------------------------*/

#include "symtable.h"
#include <stdlib.h> /* for malloc, calloc, free */
#include <string.h>
#include <assert.h> /* for assert */

/* An object that stores the key value pairs in the SymTable.*/
struct Node {
  /* defensive copy of key (string) */ 
  char *key; 
  /* void pointer to a value (generic) */
  const void *value; 
   /* pointer to next node in list */
  struct Node *next;
};

/* A single linked list storing Nodes. */
struct SymTable {
  /* pointer to first node */
  struct Node *first; 
   /* number of nodes in the SymTable */
  size_t numNodes;
};

/* no parameters, return a new SymTable object with no bindings */
SymTable_T SymTable_new(void) {
    /* declared SymTable_T as a pointer to SymTable in the header file. */
  SymTable_T oSymTable; /* oSymTable is now a pointer to SymTable. */
  oSymTable = calloc(1, sizeof(*oSymTable)); 
  if (oSymTable == NULL) return NULL;
  oSymTable->numNodes = 0;
  oSymTable->first = NULL;
  return oSymTable;
 }    

void SymTable_free(SymTable_T oSymTable) {
  struct Node *binding;
  struct Node *nextbinding;    
  assert (oSymTable != NULL);

  for (binding = oSymTable->first; binding != NULL; binding = nextbinding) {
    nextbinding = binding->next;
    free(binding->key); /* free defensive copy first */
    free(binding); /* free each node. */
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
  /* assert (pvValue != NULL); */

  if (SymTable_contains(oSymTable, pcKey) == 1) return 0;

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
  newNode->value = (void *)pvValue; /* assign given value to new node */
  newNode->next = oSymTable->first; /* bindings are placed in front */
  oSymTable->first = newNode; /* first node becomes the new node */

  return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue) {
  struct Node *binding;
  const void *oldValue;

	assert (oSymTable != NULL);
  assert (pcKey != NULL);
  /* assert (pvValue != NULL); */

  for (binding = oSymTable->first; binding != NULL; binding = binding->next) {
    if (strcmp(binding->key, pcKey) == 0) {
      oldValue = binding->value;
      binding->value = (void *)pvValue;
      return (void *)oldValue;
    }
  }
  return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
  struct Node *binding;

	assert (oSymTable != NULL);
  assert (pcKey != NULL);

  for (binding = oSymTable->first; binding != NULL; binding = binding->next) {
    if (strcmp(binding->key, pcKey) == 0) return 1;
  }
  return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
  struct Node *binding;

	assert (oSymTable != NULL);
  assert (pcKey != NULL);

  for (binding = oSymTable->first; binding != NULL; binding = binding->next) {
    if (strcmp(binding->key, pcKey) == 0) return (void *) binding->value;
  }
  return NULL;
}

/* If oSymTable contains a binding with key pcKey, removes the binding 
from oSymTable and returns the binding’s value. Otherwise, leave 
oSymTable unchanges and returns NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {  
  struct Node *binding;
  struct Node *nextbinding;
  struct Node *previous;
  const void *oldValue;

	assert (oSymTable != NULL);
  assert (pcKey != NULL);

  previous = NULL;

 for (binding = oSymTable->first; binding != NULL; binding = nextbinding) {    
    if (strcmp(binding->key, pcKey) == 0) {
      oldValue = binding->value;

      if (previous == NULL) oSymTable->first = binding->next;
      else previous->next = binding->next;

      free(binding->key); /* free defensive copy first */
      free(binding);
      oSymTable->numNodes -= 1;
      return (void *) oldValue;
    }
    previous = binding;
    nextbinding = binding->next;
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



