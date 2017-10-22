/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void readdata();
char isnum(char *s);

//Suggestion: define a struct that mirrors a record (row) of the data set


//Suggestion: prototype a mergesort function
#include "mergesort.c"
void mergesort(char ***s, int low, int high, char type, int collen, int tosort);