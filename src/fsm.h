/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * fsm.h
 ******************************************************************************/
/*
 *        Defines the enumerated types, headers, and function definitions 
 *        used in the main program and other sub routines.
 * 		  type State defines the possible states that the FSM can obtain during the
 *		  execution of the program while the type character determines the type of
 *		  character that a particular character of the input line can obtain according 
 *		  to the way it is recognized in the C identifiers rules. 
 */
#ifndef FSMM_H_
#define FSMM_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define OK      0
#define EXIT   -1
#define ERROR  -2
#define ISEVEN(n) ((n) % 2 == 0 ? 1 : 0)
#define SET()									\
	{erroroccured = 1; cs = IGNORE_ERROR;}
#define ALLOCATE(s) (char *) malloc(strlen(s) + 1) 
#define STRCPY(s) strcpy(ALLOCATE(s), s)   

/* defines the type for the current states after a particular transition 
   has been occurred in the FSM */
enum state { /* the different states of the state machine */
	STOP, 
	START, 
	BUILD_ID, 
	IDENTIFIER, 
	BUILD_ST, 
	ST, 
	END_QUERY, 
	BLANKS, 
	IGNORE, 
	IGNORE_ERROR
};
typedef enum state State;

/* defines the type of the current input character read	*/
enum character {  /* types of characters that the state machine read from stdin */
	BLANK, 
	ID_CHAR, 
	SEARCH_TYPE_CHAR, 
	END, 
	OTHER
};
typedef enum character Character;

extern void initialize(char **);
extern int get_query(char **, unsigned short *, int *);
extern void free_keywords(char **);

#endif /* FSMM_H_ */
