
/* Ricardo Grade Nr 90774 LEIC-T */
/* 2 Projeto IAED 2017 / 2018 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structs.h"
#include "LinkedL_HashT.h"
#include "CriticalP.h"

int main() {

	/* main Variables */
	int elStart = 0;
	char option[8 + 1];
	control conduct;

	if ((conduct = (control)malloc(sizeof(struct control))) == NULL)
		perror("malloc");

	/* Initializes Linked list and Hashtable */
	conduct->hList = NULL;
	conduct->hTable = NULL;
	hashInit(conduct, HLINIT);

	/* main cycle */
	while (1) {

		/* If doesn't read a string */
		if (scanf("%s", option) != 1) {printf("illegal arguments\n"); continue;}

		/* If read string add */
		/* Adds a new task to the project */
		else if (!strcmp(option, "add")) {add(conduct, &elStart); continue;}

		/* If read string duration */
		/* Lists tasks with duration >= than indicated */
		else if (!strcmp(option, "duration")) {duration(conduct, elStart); continue;}
		
		/* If read string depend */
		/* Lists tasks that are dependent on the indicated task */
		else if (!strcmp(option, "depend")) {depend(conduct); continue;}
			
		/* If read string remove */
		/* Remove a task without dependent tasks */
		else if (!strcmp(option, "remove")) {eliminate(conduct, &elStart); continue;}
		
		/* If read string path */
		/* Lists all the tasks in the critical path of the project */
		/* In order of introduction, with the shape */
		/* <id> <description> <duration> [<early> <late>] <ids> */
		/* Set elStart to true, to print [<early> <late>] in other functions */
		else if (!strcmp(option, "path")) {path(conduct, &elStart); continue;}

		/* If read string exit */
		else if (!strcmp(option, "exit")) break;

		/* If read unknown string */
		else printf("illegal arguments\n");
	}
	
	/* Deallocate the memory */
	/* Used by tasks, Linked list and Hashtable */
	freeHash(conduct);
	freeList(conduct->hList, 1);
	free(conduct);

	return 0;
}

void add(control conduct, int *elStart) {

	/* add Variables */
	task new;
	unsigned long idSrc;
	char description[MAXDES + 1];
	node idNode, ids = NULL;

	if ((new = (task)malloc(sizeof(struct task))) == NULL)
		perror("malloc");

	/* If it doesn't read an unsigned long, a string, and another unsigned long */
	if (scanf("%lu \" %8000[^\"] \" %lu", &new->id, description, &new->duration) != 3 ||
		!new->id || !new->duration) {
		scanf("%*[^\n]");
		printf("illegal arguments\n");
		free(new);
		return;
	}

	/* If the new task id is already part of the project */
	if (hashSearch(conduct, new->id) != NULL) {
		scanf("%*[^\n]");
		printf("id already exists\n");
		free(new);
		return;
	}

	/* Allocates the memory needed to contain the read string */
	if ((new->description = (char *)malloc(sizeof(char) * (strlen(description) + 1))) == NULL)
		perror("malloc");

	/* Copy the read string to new->description */
	strcpy(new->description, description);
	/* Initialize new->antc and new->succ */
	new->antc = new->succ = NULL;

	/* While there are more ids to read */
	while (getchar() == ' ') {

		/* If it can't read an unsigned long or is invalid */
		if (scanf("%lu", &idSrc) != 1 || !idSrc) {
			scanf("%*[^\n]");
			printf("illegal arguments\n");
			freeTask(new);
			freeList(ids, 0);
			return;
		}

		/* If the task id isn't already part of the project */
		if ((idNode = hashSearch(conduct, idSrc)) == NULL) {
			scanf("%*[^\n]");
			printf("no such task\n");
			freeTask(new);
			freeList(ids, 0);
			return;
		}

		/* Append new task to ids */
		ids = insertReverse(ids, idNode->item);
	}

	/* For each task in ids Linked list append it to new->antc */
	/* And adds the new task to item->succ */
	for (idNode = headList(ids); idNode != NULL; idNode = idNode->next) {
		new->antc = insertReverse(new->antc, idNode->item);
		idNode->item->succ = insertReverse(idNode->item->succ, new);
	}
	freeList(ids, 0);

	/* Append task to Linked list and Hashtable */
	appendItem(conduct, new);
	/* Invalidates the previously calculated critial path */
	/* Set elStart to false, not to print [<early> <late>] in other functions */
	*elStart = 0;
}

void duration(control conduct, int elStart) {

	/* duration Variables */
	unsigned long duration = 0;
	node i, t;

	/* If there is a duration to be read */
	if (getchar() != '\n')
		/* If it can't read an unsigned long or is invalid */
		if (scanf("%lu", &duration) != 1 || !duration) {
			scanf("%*[^\n]");
			printf("illegal arguments\n");
			return;
		}

	/* For each Linked list node */
	for (i = headList(conduct->hList); i != NULL; i = i->next)
		/* if the duration of the task associated to the node is >= than the introduced */
		/* Prints the task attributes */
		if (i->item->duration >= duration) {

			printf("%lu \"%s\" %lu", i->item->id, i->item->description, i->item->duration);

			if (elStart) {
				if (i->item->earlyStart == i->item->latestStart)
					printf(" [%lu CRITICAL]", i->item->earlyStart);
				else
					printf(" [%lu %lu]", i->item->earlyStart, i->item->latestStart);
			}

			for (t = headList(i->item->antc); t != NULL; t = t->next)
				printf(" %lu", t->item->id);

			printf("\n");
		}
}

void depend(control conduct) {

	/* depend Variables */
	unsigned long id;
	node idNode, i;

	/* If it can't read an unsigned long or is invalid */
	if (scanf("%lu", &id) != 1 || !id) {
		scanf("%*[^\n]");
		printf("illegal arguments\n");
		return;
	}

	/* If the task id isn't already part of the project */
	if ((idNode = hashSearch(conduct, id)) == NULL) {
		printf("no such task\n");
		return;
	}

	/* If the task have no dependencies */
	if (idNode->item->succ == NULL) {
		printf("%lu: no dependencies\n", id);
		return;
	}

	/* Prints the dependencies associated to the task */
	printf("%lu:", id);
	for (i = headList(idNode->item->succ); i != NULL; i = i->next)
		printf(" %lu", i->item->id);

	printf("\n");
}

void eliminate(control conduct, int *elStart) {

	/* eliminate Variables */
	unsigned long id;
	node idNode, i, t;

	/* If it can't read an unsigned long or is invalid */
	if (scanf("%lu", &id) != 1 || !id) {
		scanf("%*[^\n]");
		printf("illegal arguments\n");
		return;
	}

	/* If the task id isn't already part of the project */
	if ((idNode = hashSearch(conduct, id)) == NULL) {
		printf("no such task\n");
		return;
	}

	/* If the task have dependencies */
	if (idNode->item->succ != NULL) {
		printf("task with dependencies\n");
		return;
	}

	/* For each antecedent */
	for (i = idNode->item->antc; i != NULL; i = i->prev) {
		/* Finds antecedent in antecendent dependents and delete it */
		for (t = i->item->succ; t->item->id != id; t = t->prev);
		delNodeRev(t, &(i->item->succ));
		free(t);
	}

	/* Delete idNode in HashTable */
	delNodeRev(idNode, &conduct->hTable[hIndex(id, conduct->hSize)]);

	/* Finds the task in Linked list and delete it */
	for (i = conduct->hList; i->item->id != id; i = i->prev);
	delNodeRev(i, &conduct->hList);
	
	/* Delete task node in Linked list and HashTable */
	free(i);
	freeNode(idNode);

	/* Invalidates the previously calculated critial path */
	/* Set elStart to false, not to print [<early> <late>] in other functions */
	*elStart = 0;
}

void path(control conduct, int *elStart) {

	/* path Variables */
	node i, t;
	unsigned long Pduration;

	/* Initialize Project duration */
	Pduration = 0;
	/* Initialize earlyStarts */
	for (i = conduct->hList; i != NULL; i = i->next)
		i->item->earlyStart = 0;

	/* Walking through the linked list head to tail */
	for (i = headList(conduct->hList); i != NULL; i = i->next) {

		/* Walking through the successors of the task associated to the node */
		for (t = i->item->succ; t != NULL; t = t->prev)
			/* If the sum of the earlyStart and the duration of the item */
			/* Is > than the successor's earlyStart */
			/* Replaces successor's earlyStart by the sum */
			if (i->item->earlyStart + i->item->duration > t->item->earlyStart)
				t->item->earlyStart = i->item->earlyStart + i->item->duration;

		/* If the task have no dependents */
		if (i->item->succ == NULL)
			/* If the sum of the earlyStart and the duration of the item */
			/* Is > than the current Project duration */
			/* Replaces Project duration by the sum */
			if (i->item->earlyStart + i->item->duration > Pduration)
				Pduration = i->item->earlyStart + i->item->duration;
	}

	/* Initialize lateStarts */
	for (i = conduct->hList; i != NULL; i = i->prev)
		i->item->latestStart = Pduration - i->item->duration;

	/* Walking through the linked list tail to head */
	for (i = conduct->hList; i != NULL; i = i->prev)

		/* Walking through the antecedents of the task associated to the node */
		for (t = i->item->antc; t != NULL; t = t->prev)
			/* If the difference of the task's lateStart and the antecedent's duration */
			/* Is < than the antecedent's lateStart */
			/* Replaces antecedent's lateStart by the difference */
			if (i->item->latestStart - t->item->duration < t->item->latestStart)
				t->item->latestStart = i->item->latestStart - t->item->duration;

	/* Lists all the tasks in the critical path of the project and Project duration */
	printEL(conduct, Pduration);
	/* Validates critial path */
	/* Set elStart to true, to print [<early> <late>] in other functions */
	*elStart = 1;
}

void printEL(control conduct, unsigned long Pduration) {

	/* printEL Variables */
	node i, t;

	/* Walking through the linked list by the order of introduction */
	for (i = headList(conduct->hList); i != NULL; i = i->next)
		/* If the earlyStart is == to lateStart of the task associated to the node */
		/* Prints the task attributes */
		if (i->item->earlyStart == i->item->latestStart) {

			printf("%lu \"%s\" %lu ", i->item->id, i->item->description, i->item->duration);
			printf("[%lu CRITICAL]", i->item->earlyStart);

			for (t = headList(i->item->antc); t != NULL; t = t->next)
				printf(" %lu", t->item->id);

			printf("\n");
		}

	/* Prints the Project duration */
	printf("project duration = %lu\n", Pduration);
}
