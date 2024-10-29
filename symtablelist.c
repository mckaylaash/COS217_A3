/*--------------------------------------------------------------------*/
/* symtablelist.c                                                              */
/* Author: Mckayla Ashley  

SymTable Abstract Data Type Linked List Implementation                                               */
/*--------------------------------------------------------------------*/

#include "symtable.h"
#include <assert.h> 
#include <stdlib.h> 
#include <string.h>

/* Each binding, or key-value pair, is stored in a Node.  Nodes are 
linked to form a list.  */
struct Node {
  /* The Key. */ 
  char *key; 
  /* The Item. */
  const void *value; 
  /* The address of the next StackNode. */
  struct Node *next;
};

/* A SymTable is a "dummy" node that points to the first Node in the 
linked list. It keeps track of the number of Nodes in the table. */
struct SymTable {
  /* The address of the first StackNode. */
  struct Node *first; 
    /* Number of key-value pairs in table. */
  size_t numNodes;
};

SymTable_T SymTable_new(void) {
  SymTable_T oSymTable; 
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
  return oSymTable->numNodes; 
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue) {
	struct Node *newNode;

  assert (oSymTable != NULL);
  assert (pcKey != NULL);

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
        (*pfApply)((char*)psCurrentNode->key, 
        (void*)psCurrentNode->value, (void*)pvExtra);
}



