/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * engine.h
 ******************************************************************************/
#ifndef ENGINE_H_
#define ENGINE_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include "bitvec.h"
#include "fsm.h"
#include "common.h"
#define MAXCHARS 80	/*	maximum # of first chars to be tested in a file 
						to determine if it is a text file	*/
#define TRUE    1
#define FALSE   0
#define MAXWORDS    10000
#define MAXSEARCHES 8 /* we are using a linked list */
#define k 10
#define HASHSIZE    1024 /* hash size must be 2^k */
/* we decided this size to suit the hashing algorithm we use. */

#define PATHSIZE	1024 /*	maximum size of a 
							possible absolute path of a text file	*/
#define KILOBYTE	1024
/*	flags used to identify the type of a file	*/
#define S_IFMT	__S_IFMT
#define S_IFDIR	__S_IFDIR
/* a simple macro for memory allocation needs	*/
#define TYPED_ALLOC(type) (type *) malloc(sizeof (type))
#define TYPED_CALLOC(n, type) (type *) calloc(n, sizeof (type))
#define MAKESTR(size) (char *) malloc(size + 1) /* a macro for making 
												 a new string of size 'size'	*/
#define SEP '/'	/*	this is the file separator used in Linux	*/

#define MAKENODE(p, w, tmp) {\
                p = TYPED_ALLOC(WORD);\
                p->word = strcpy(MAKESTR(strlen(w)), w);\
                p->next = NULL;\
                tmp = p;}

/*
 * Hashing function:
 * Compute the key to index the hash table for the given word.
 * Algorithm used here is Fibbonacci Hasing.
 */    
#define PRIME 311U            
#define bitsize() (CHAR_BIT * sizeof (size_t))
#define FIB() (sqrt(5) - 1) / 2.0
#define a (unsigned int) ((double) FIB() * (double) pow(2, bitsize()))
#define hash(x) ((unsigned long) ((x) * a) >> (unsigned long) (bitsize() - k))
#define HASH(s) {\
	for (hashval = 0; *s != '\0'; ++s) \
			hashval = *s + PRIME * hashval;\
	*keyp = hash(hashval);} /* inline this macro */

typedef enum {A = 1, O = 2, L = 4, E = 8, C = 16, D = 32}	OPT_BITS;
typedef char S_BITVEC;
typedef unsigned int L_BITVEC; /*	long bit vector type- length: 32 bits. 
									So we can search for a maximum of 32 key words	*/
typedef struct input_data {
    char rootdir[PATHSIZE/4];
    S_BITVEC opt_bits;	/* the correct combination of options 
    					   that the user invoked the prog. with	*/
} INPUT_DATA;

/* This is the structure for an object of key_word type.
  The binary search tree consists of the input keywords of the main program and this tree
  is built from the nodes of this type objects(i.e 'key_word' type)	*/
typedef struct word {
    BITVECTOR           *bitvec;
    char				*word; /* the actual string of the identifier or simply the key word */
    struct word		    *next;	/* pointer to the left subtree */
} WORD;
typedef struct word *WORD_LIST;

/* This is the structure of a search engine object!*/
typedef struct search_engine {
    WORD_LIST			hashtable[HASHSIZE]; /*	the binary tree of this engine	*/
    INPUT_DATA			*id;
    STRING              *subpaths;
    size_t              nfiles;
    unsigned short		search_type;
} ENGINE;

/* functions defined in listfiles.c	*/
extern int istextfile(STRING);
extern void dirwalk(STRING, void (*fcn)(STRING));
extern unsigned int	write_paths(STRING, STRING **);
extern void read_input(int, STRING *, INPUT_DATA *);
/* these parameters and this function is defined in unistd.h
	 and I had to include them deliberately here. I think this is due to
	 the flags I use in compilation(see makefile)	*/
extern int getopt(int, STRING const *, const STRING);

extern int exit_main;
extern int ccount;

#endif /* ENGINE_H_ */













