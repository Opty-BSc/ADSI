/* 1 Projeto IAED 2017-2018 */
/* Ricardo Grade, 90774, LEIC-T */

#include <stdio.h>
#include <string.h>

/* Constants and Macros */
#define MAX_S 10000
#define LINCOL 0
#define COLLIN 1
#define less(e1, e2, ind) (ind ? e1.lin < e2.lin : e1.col < e2.col)
#define init(vec, val, l, r) {unsigned long i; for (i = l; i <= r; i++) vec[i] = val;}

/* Struct Value used to represent an element of a matrix */
typedef struct {
	unsigned long lin, col;
	double val;
} Value;

/* Global Variables */
Value V_Esp[MAX_S];
unsigned long N_Elem, minl, maxl, minc, maxc;
double zeroEle;

/* Declaration of functions and its description */

/* Given a file name, reads it and extracts the elements */
void read(char fName[]);
/* Given an element adds it to the matrix */
/* If the value of the element != element zero */
void add(Value e);
/* Lists all elements in the matrix */
void list();
/* Shows the characteristics of the matrix */
void density();
/* Given a line index shows the elements of the respective line */
void line(unsigned long lin);
/* Given a column index shows the elements of the respective column */
void column(unsigned long col);
/* Given an indicator sorts V_Esp */
/* By Line-Column if ind == LINCOL */
/* Or by Column-Line if ind == COLLIN */
void order(int ind);
/* Removes all elements with the value zero from the matrix */
/* And updates the characteristics of the matrix */
void zero();
/* Given a file name writes all its of elements in the matrix */
void write(char fName[]);
/* Compresses matrix (double-offset indexing) */
/* Shows values, index, and offset */
void sparse();

int main(int argc, char const *argv[]) {

	/* main's Variables */
	Value e;
	char ch, str[6 + 1], fName[80 + 1] = "";

	/* Initialization of Global Variables */
	N_Elem = zeroEle = 0;

	/* Check if a file has been inserted */
	if (argc == 2) {
		/* Names the input / output file */
		strcpy(fName, argv[1]);
		/* Reads the file */
		read(fName);
	}

	/* Main cicle */
	while (1) {

		/* Reads the option inserted by the user */
		if (scanf("%c", &ch) != 1) continue;

		switch(ch) {

			/* Adds an element of type Value to the matrix */
			case 'a':
				if (scanf("%lu %lu %lf", &e.lin, &e.col, &e.val) == 3) add(e);
				break;

			/* Lists all represented elements of the matrix */
			case 'p': list(); break;

			/* Lists the boundaries of the matrix and its density */
			case 'i': density(); break;

			/* Lists a complete line of the matrix */
			case 'l':
				if (scanf("%lu", &e.lin) == 1) line(e.lin);
				break;

			/* Lists a complete column of the matrix */
			case 'c':
				if (scanf("%lu", &e.col) == 1) column(e.col);
				break;

			/* Sorts the elements of the matrix */
			case 'o':
				/* Sorts by Line - Column */
				if (getchar() == '\n') order(LINCOL);
				/* Sorts by Column - Line */
				else if (scanf("%6s", str) == 1 && !strcmp(str, "column")) order(COLLIN);
				/* By default sorts by Line - Column */
				else order(LINCOL);
				break;

			/* Sets the new element zero of the matrix */
			case 'z': 
				if (scanf("%lf", &zeroEle) == 1) zero();
				break;

			/* Compresses matrix (double-offset indexing) */
			case 's': sparse(); break;

			/* Saves the matrix elements in a file */
			case 'w':	
				/* Previous / Preset file */
				if (getchar() == '\n' && fName[0]) write(fName);
				/* New file */
				else if (scanf("%80s", fName) == 1) write(fName);
				break;

			/* Finishes the program */
			case 'q': return 0;
			
			/* Invalid input */
			default: break;
		}
	}

	return 0;
}

void read(char fName[]) {

	/* read's Variables */
	Value e;
	FILE *fp;

	/* Tries to open a file */
	if ((fp = fopen(fName, "r")) == NULL) return;

	/* Reads the file by extracting all elements of type Value */
	while (fscanf(fp, "%lu %lu %lf\n", &e.lin, &e.col, &e.val) == 3)
		add(e);
	
	/* Closes the file */
	fclose(fp);
}

void add(Value e) {

	/* add's Variable */
	unsigned long i;

	/* Checks if there is an element */
	/* On the same position of the matrix */
	for (i = 0; i < N_Elem; i++)
		/* If it exists */
		if (e.lin == V_Esp[i].lin && e.col == V_Esp[i].col) {
			/* Replaces the previous value by the new */
			V_Esp[i].val = e.val;
			/* If the new value is the element zero */
			/* Calls function zero to take care of it */
			if (e.val == zeroEle) zero();
			return;
		}

	/* If there isn't any elements on the same position of the new one */

	/* If the new value is the element zero */
	if (e.val == zeroEle) return;

	/* Updates the boundaries of the matrix */
	if (N_Elem) {
		if (e.lin < minl) minl = e.lin;
		if (e.lin > maxl) maxl = e.lin;
		if (e.col < minc) minc = e.col;
		if (e.col > maxc) maxc = e.col;

	} else {
		minl = maxl = e.lin;
		minc = maxc = e.col;
	}

	/* Updates the existing number of elements */
	/* Adds the element to the vector which represents the matrix */
	V_Esp[N_Elem++] = e;
}

void list() {

	/* list's variables */
	unsigned long i;

	/* If there are no elements in the matrix */
	if (!N_Elem) {
		printf("empty matrix\n");
		return;
	}
		
	/* If there are, lists all of them with the format [lin;col]=val */
	for (i = 0; i < N_Elem; i++)
		printf("[%lu;%lu]=%.3f\n", \
			V_Esp[i].lin, V_Esp[i].col, V_Esp[i].val);
}

void density() {

	/* density's Variables and its Intialization */
	unsigned long size = (maxl - minl + 1) * (maxc - minc + 1);
	double perc = 100.0 * N_Elem / size;

	/* If there are no elements in the matrix */
	if (!N_Elem) {
		printf("empty matrix\n");
		return;
	}
	
	/* If there are, shows the bondaries of the matrix */
	/* The number of elements, the size, and the density in percentage */
	printf("[%lu %lu] [%lu %lu] %lu / %lu = %.3f%%\n", \
		minl, minc, maxl, maxc, N_Elem, size, perc);
}

void line(unsigned long lin) {

	/* line's Variables */
	int ind = 0;
	unsigned long i;
	double V_Aux[MAX_S];

	/* Starts V_Aux [minc,maxc] with element zero */
	init(V_Aux, zeroEle, minc, maxc);

	/* Insert in V_Aux [minc, maxc] the value of the elements in the specified line */
	for (i = 0; i < N_Elem; i++)
		if (V_Esp[i].lin == lin && (ind = 1))
			V_Aux[V_Esp[i].col] = V_Esp[i].val;

	/* If there are no elements in the line */
	if (!ind) {
		printf("empty line\n");
		return;
	}

	/* If there are, shows all of them with zeroEle in the empty spaces */
	/* Format in line: */
	/* val0 val1 val2 etc. */
	for (i = minc; i <= maxc; i++)
		printf(" %.3f", V_Aux[i]);
	printf("\n");
}

void column(unsigned long col) {

	/* column's Variables */
	int ind = 0;
	unsigned long i;
	double V_Aux[MAX_S];

	/* Starts V_Aux [minl,maxl] with element zero */
	init(V_Aux, zeroEle, minl, maxl);

	/* Insert in V_Aux [minl, maxl] the value of the elements in the specified column */
	for (i = 0; i < N_Elem; i++)
		if (V_Esp[i].col == col && (ind = 1))
			V_Aux[V_Esp[i].lin] = V_Esp[i].val;

	/* If there are no Elements in the column */
	if (!ind) {
		printf("empty column\n");
		return;
	}

	/* If there are, shows all of them with zeroElement in the empty spaces */
	/* Format in column: */
	/* [col;0]=val0 */
	/* [col;1]=val1 */
	/* [col;2]=val2 */
	/* etc. */
	for (i = minl; i <= maxl; i++)
		printf("[%lu;%lu]=%.3f\n", i, col, V_Aux[i]);
}

void order(int ind) {

	/* order's variables */
	unsigned long i = 0;
	int iter, t;
	Value e;

	/* It only does 2 cycles and changes the ind variable */
	/* From true to false or  from false to true */
	for (iter = 0; iter < 2; i = 0, ind = ind ? 0 : 1, iter++)

		/* Insertion sort, adapted */
		while (++i < N_Elem) {

			t = i;
			e = V_Esp[i];

			/* less(e, V_Esp[t], ind) */
			/* Does e1.lin < V_Esp[t].lin if ind == true */
			/* Does e1.col < V_Esp[t].col if ind == false */
			while (--t >= 0 && less(e, V_Esp[t], ind))
				V_Esp[t + 1] = V_Esp[t];

			V_Esp[t + 1] = e;
		}
}

void zero() {

	/* zero's Variables */
	unsigned long i, t;

	/* Removes elements whose value is element zero */
	for (i = t = 0; i < N_Elem; i++)
		if (V_Esp[i].val != zeroEle) {
			if (i != t) V_Esp[t] = V_Esp[i];
			t++;
		}

	/* If no element has the value of element zero */
	if (i == t) return;

	/* If any element has the value of element zero */

	/* Updates the number of elements in the matrix */
	if (!(N_Elem = t)) return;

	/* Initializes the bondaries of the matrix */
	minl = maxl = V_Esp[0].lin;
	minc = maxc = V_Esp[0].col;

	/* Updates the bondaries of the matrix */
	for (i = 1; i < N_Elem; i++) {
		if (V_Esp[i].lin < minl) minl = V_Esp[i].lin;
		if (V_Esp[i].lin > maxl) maxl = V_Esp[i].lin;
		if (V_Esp[i].col < minc) minc = V_Esp[i].col;
		if (V_Esp[i].col > maxc) maxc = V_Esp[i].col;
	}
}

void write(char fName[]) {

	/* write's Variables */
	FILE *fp;
	unsigned long i;

	/* Tries to open a write file */
	if ((fp = fopen(fName, "w")) == NULL) return;

	/* In format: */
	/* lin col val */
	for (i = 0; i < N_Elem; i++)
		fprintf(fp, "%lu %lu %.3f\n", \
			V_Esp[i].lin, V_Esp[i].col, V_Esp[i].val);
	
	/* Closes the file */
	fclose(fp);
}

void sparse() {

	/* sparse's Variables */
	double V_Sparse[2 * MAX_S];
	int V_Line[MAX_S], V_Cont[2 * MAX_S], offset[2 * MAX_S];
	unsigned long index[2 * MAX_S], maxI, perLin, i, t;
	int w, st = 1, iSp = 0, tSp = -1;

	/* If the density of the matrix is > 0.5 */
	if (!N_Elem || (1.0 * N_Elem / ((maxl - minl + 1) * (maxc - minc + 1))) > 0.5) {
		printf("dense matrix\n");
		return;
	}

	/* Starts V_Sparse [0, 2*MAX_S-1] with element zero */
	init(V_Sparse, zeroEle, 0, 2 * MAX_S - 1);
	/* Starts V_Cont [minl,maxl], offset [minl,maxl] and index [0, 2*MAX_S-1] with zero */
	init(index, 0, 0, 2 * MAX_S - 1);
	init(offset, 0, minl, maxl);
	init(V_Cont, 0, minl, maxl);

	/* Counts the elements in the matrix lines */
	for (i = 0; i < N_Elem; i++)
		V_Cont[V_Esp[i].lin]++;

	/* For each line it inserts in V_Sparse the line values */
	/* And inserts the respective values of offset and index in their respective vector */
	for (i = minl; i <= maxl; i++) {

		/* Finds the index of the line that contains the largest number of elements */
		for (maxI = t = minl, t++; t <= maxl; t++)
			if (V_Cont[t] > V_Cont[maxI]) maxI = t;

		/* If there are no elements */
		if (!V_Cont[maxI]) break;
		
		/* Fills V_Line with the indexes corresponding to elements that are in V_Esp */
		/* That contains the same line as maxI */
		for (t = perLin = 0; t < N_Elem; t++)
			if (V_Esp[t].lin == maxI)
				V_Line[perLin++] = t;

		/* Finds the tSp position from where will start filling the line in V_Sparse */
		while (st)
			/* Increases tSp and checks if from this position on it's still possible to fill V_Sparse */
			for (++tSp, t = st = 0; t < perLin; t++)
				/* Without writing over some of the elements */
				if (V_Sparse[tSp + (V_Esp[V_Line[t]].col - minc)] != zeroEle) {
					/* If it doesn't, checks the next position */ 
					st = 1;
					break;
				}

		/* Inserts the respective value in offset (tSp) */
		offset[maxI] = tSp;

		/* Fills the V_Sparse and index vectors with the respective values */
		for (t = 0; t < perLin; t++) {
			V_Sparse[tSp + (V_Esp[V_Line[t]].col - minc)] = V_Esp[V_Line[t]].val;
			index[tSp + (V_Esp[V_Line[t]].col - minc)] = maxI;
		}

		/* Updates lenght (iSp) of V_Sparse and index */
		if (tSp + (int)(maxc - minc) > iSp) iSp = tSp + (int)(maxc - minc);
		/* Resets Variables */
		V_Cont[maxI] = tSp = st = -1;
	}

	/* Show the Values of V_Sparse */
	printf("value =");
	for (w = 0; w <= iSp; w++)
		printf(" %.3f", V_Sparse[w]);
	printf("\n");

	/* Show the Values of index */
	printf("index =");
	for (w = 0; w <= iSp; w++)
		printf(" %lu", index[w]);
	printf("\n");

	/* Show the Values of offset */
	printf("offset =");
	for (i = minl; i <= maxl; i++)
		printf(" %d", offset[i]);
	printf("\n");
}
