
/* Macro that gives the id and HashTable size returns the index to be placed */
#define hIndex(id, hSize) id % hSize

/* Functions declaration */
void hashInit(control conduct, int nMem);
void redifHash(control conduct, int mult);
void appendItem(control conduct, task item);
node insertReverse(node nodeHead, task item);
node headList(node tail);
void delNodeRev(node element, node *tail);
node hashSearch(control conduct, unsigned long id);
void freeTask(task item);
void freeNode(node element);
void freeList(node head, int ind);
void freeHash(control conduct);
