
#include <stdio.h>
#include <stdlib.h>
#include "Structs.h"
#include "LinkedL_HashT.h"

void hashInit(control conduct, int nMem) {

	/* hashInit Variables */
	unsigned long i;

	/* Update's HashTable size */
	conduct->hSize = nMem;
	if ((conduct->hTable = (node *)malloc(sizeof(node) * conduct->hSize)) == NULL)
		perror("malloc");

	/* Initializes the heads of HashTable */
	for (i = 0; i < conduct->hSize; i++) conduct->hTable[i] = NULL;
}

void redifHash(control conduct, int mult) {

	/* redifHash Variables */
	node i;
	int hInd;

	/* Delete and creates a new HashTable */
	freeHash(conduct);
	hashInit(conduct, conduct->hSize * mult);

	/* Insert all tasks in new HashTable */
	for (i = conduct->hList; i != NULL; i = i->prev) {

		hInd = hIndex(i->item->id, conduct->hSize);
		conduct->hTable[hInd] = insertReverse(conduct->hTable[hInd], i->item);
	}
}

void appendItem(control conduct, task item) {

	/* appendItem Variables */
	int hInd = hIndex(item->id, conduct->hSize);

	/* Insert in Linked list and HashTable a new task */
	conduct->hList = insertReverse(conduct->hList, item);
	conduct->hTable[hInd] = insertReverse(conduct->hTable[hInd], item);
}

node insertReverse(node nodeHead, task item) {

	/* insertReverse Variables */
	node new;
	if ((new = (node)malloc(sizeof(struct node))) == NULL)
		perror("malloc");

	/* Initialize new task */
	new->item = item;
	/* Inserts the node in the linked list */
	new->prev = nodeHead;
	new->next = NULL;
	if (nodeHead != NULL) nodeHead->next = new;

	return new;
}

node headList(node tail) {

	/* headList Variables */
	node head = NULL, i;

	/* Given the tail of the linked list it finds the head */
	for (i = tail; i != NULL; head = i, i = i->prev);

	return head;
}

void delNodeRev(node element, node *tail) {

	/* If the element is the only one in the list */
	if (element->prev == NULL && element->next == NULL)
		*tail = NULL;

	/* If the element is the head of the list */
	else if (element->prev == NULL && element->next != NULL)
		element->next->prev = NULL;

	/* If the element is the tail of the list */
	else if (element->prev != NULL && element->next == NULL) {
		*tail = element->prev;
		element->prev->next = NULL;
	}

	/* If the element is in the middle of the list */
	else {
		element->prev->next = element->next;
		element->next->prev = element->prev;
	}
}

node hashSearch(control conduct, unsigned long id) {

	/* hashSearch Variables */
	float i;
	node nodeHead = conduct->hTable[hIndex(id, conduct->hSize)];

	/* Walks through node linked list where the searched task can be */
	for (i = 0; nodeHead != NULL; i++, nodeHead = nodeHead->prev) {

		if (nodeHead->item->id == id) return nodeHead;

		/* If the search is taking too many iterations */
		if (i / conduct->hSize > 0.5) {
			redifHash(conduct, 2);
			return hashSearch(conduct, id);
		}
	}

	return NULL;
}

void freeTask(task item) {
	
	/* Deallocates memory of the string */
	free(item->description);
	/* Deallocates memory of the antecedent Linked list */
	freeList(item->antc, 0);
	/* Deallocates memory of the successors Linked list */
	freeList(item->succ, 0);
	/* Deallocates memory of the node */
	free(item);
}

void freeNode(node element) {

	/* Deallocates memory of the task */
	freeTask(element->item);
	/* Deallocates memory of the node */
	free(element);
}

void freeList(node tail, int ind) {

	/* freeList Variables */
	node i;
	
	/* If ind is True free node and task */
	/* If ind is False free just node */
	while (tail != NULL) {
		i = tail->prev;
		if (ind) freeNode(tail);
		else free(tail);
		tail = i;
	}
}

void freeHash(control conduct) {

	/* freeHash Variables */
	unsigned long i;

	/* For each tail of the HashTable it frees the related linked list */
	for (i = 0; i < conduct->hSize; i++) freeList(conduct->hTable[i], 0);

	/* frees HashTable */
	free(conduct->hTable);
}
