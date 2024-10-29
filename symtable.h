/*--------------------------------------------------------------------*/
/* symtable.h                                                         */
/* Author: Mckayla Ashley  

SymTable Abstract Data Type Interface                                               */
/*--------------------------------------------------------------------*/
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stddef.h>
/* Declaring the SymTable object. */
/* struct SymTable; */

/* A SymTable is a last-in-first-out collection of bindings, where each
binding contains a key and a value pair. */
typedef struct SymTable *SymTable_T; 
/*--------------------------------------------------------------------*/

/* Return a new Symtable object that contains no bindings. If 
insufficient memory is available, return NULL. */
SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/* Free all memory occupied by oSymTable. */
  void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Return the number of bindings in oSymTable. */
  size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* If oSymTable does not contain a binding with key pcKey, adds a new 
binding to oSymTable consisting of key pcKey and value pvValue, and 
returns 1 (TRUE). Otherwise, leaves oSymTable unchanged and returns 0 
(FALSE). If insufficient memory is available, leaves oSymTable unchanged
 and returns 0 (FALSE). */
  int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

/* If oSymTable contains a binding with key pcKey, replaces the 
binding’s value with pvValue and return the old value. Otherwise, leaves
 oSymTable unchanged and returns NULL. */
  void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

/* Returns 1 (TRUE) if oSymTable contains a binding whose key is pcKey.
 Otherwise, return 0 (FALSE). */
  int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Returns the value of the binding within oSymTable whose key is pcKey. 
Return NULL if no binding exists. */
  void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* If oSymTable contains a binding with key pcKey, removes the binding 
from oSymTable and returns the binding’s value. Otherwise, leave 
oSymTable unchanges and returns NULL. */
  void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Applies function *pfApply to each binding in oSymTable, passing 
pvExtra as an extra parameter. */
  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), 
     const void *pvExtra);

/*--------------------------------------------------------------------*/
#endif