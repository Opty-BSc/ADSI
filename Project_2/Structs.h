
typedef struct task *task;
typedef struct node *node;
typedef struct control *control;

struct task {
	unsigned long id, duration;
	char *description;
	/* Dependents */
	node antc;
	/* Dependencias */
	node succ;
	unsigned long earlyStart, latestStart;
};

struct node {
	task item;
	/* Point to the next node */
	node next;
	/* Point to the previous node */
	node prev;
};

struct control {
	/* Linked list tail */
	node hList;
	/* HashTable */
	node *hTable;
	/* HashTable's size */
	unsigned long hSize;
};
