/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * readquery.c
 ******************************************************************************/
#include "fsm.h"
#define TEST
#undef TEST

/*
 *	Initialize the keywords array.
 * Remember that we always keep the
 * last element NULL and we do not use
 * it in assignments.
 */
void initialize(char **a)
{
	int i;
	for (i = 0; i < KW_SIZE; ++i) {
		a[i] = NULL;
		
	}
}

/*
 * Frees the keywords in the 
 * keywords array.
 */
void free_keywords(char **a)
{
    int i;    
    for (i = 0; a[i] != NULL && i < KW_SIZE; ++i) {
        free(a[i]);
        a[i] = NULL;
    }
}

/*
 * Reads the next character from the file pointed to by
 * 'fp' and decide the type of that char. It may be either
 * 		         ID_CHAR : isdigit(c) || isalpha(c) || c == '_'
 *		           BLANK : isspace(c) && c != '\n'
 *		SEARCH_TYPE_CHAR : c == '|' || c == '&'
 *		             END : c == '\n' || c == EOF
 *		           OTHER : others
 *		           ERROR : -1
 */
static int tokencount;
static int idccount;
static int erroroccured;
static char *word;
static char stsofar;
static char c;

int exit_main;
int ccount;

static Character get_next(FILE *fp)
{
	Character nc;		/* next character */
	/*
	  extern char c;
	  extern int ccount, exit_main;
	*/
	c = getc(fp);				
	if (isdigit(c) || isalpha(c) || c == '_') {
		nc = ID_CHAR;
		++idccount;/* the char count of this possible identifier */
		++ccount;
		if (idccount <= MAXWORD - 1) {
			*word++ = (isalpha(c) != 0 ? tolower(c) : c);
		} else *word = '\0';
	} else if (isspace(c) && c != '\n') {
		nc = BLANK;
		*word = '\0';
	} else if (c == '|' || c == '&') {
	    ++ccount;
		nc = SEARCH_TYPE_CHAR;	
		if (stsofar == 'n') stsofar = c;
		else if (stsofar != c) {
			fprintf(stderr, "\ndifferent search types are not allowed...");
			return ERROR;
		}
	} else if (c == '\n' || c == EOF) {
		nc = END; 
		*word = '\0';/* end of input line */
	} else if (c == '.') {/* if '.' is the first char of the query, then
	                         the user wants to exit from the main program! */
	    ++ccount;
	    if (ccount == 1)
	        exit_main = 1;
	    else fprintf(stderr, "\ncharacter '%c' is unrecognized...", c);
	} else {
		nc = OTHER;
		fprintf(stderr, "\ncharacter '%c' is unrecognized...", c);
	}
	return nc;
}

/*
 * Decides the next state of the state machine based on the
 * current state and the character read from the input.
 * This makes sure that the query be in correct format.
 * i.e. each token must be separated by a SPACE, and if any SEARCH_TYPE_TOKENS
 * are used, they must be in proper position. Words more than MAXKEYWORDS are
 * ignored and words longer than MAXCHARS characters are saved up to MAXCHARS if
 * only they do not include any char except ID_CHAR.
 */
static State transition(State current, Character cc)
{
	State ns;		/* next state to be returned			*/
	/*
	  extern int erroroccured;
	  extern char c;
	*/
	if (current == START)
		switch (cc) {
		case ID_CHAR:
			ns = BUILD_ID;
			break;
		case SEARCH_TYPE_CHAR:
			ns = BUILD_ST;
			break;
		case BLANK:
			ns = BLANKS;
			break;
		case END:
			ns = STOP;
			break;
		default:
			erroroccured = 1;
			ns = IGNORE_ERROR;
			break;
		}
	else if (current == BUILD_ID)
		switch (cc) {
		case ID_CHAR:
			ns = (idccount <= (MAXWORD - 1) ? BUILD_ID : IGNORE);
			break;
		case BLANK:
			ns = IDENTIFIER;
			break;
		case END:
			ns = END_QUERY;
			break;
		default:
			erroroccured = 1;
			ns = IGNORE_ERROR;
			break;
		}
	else if (current == BUILD_ST)
		switch (cc) {
		case BLANK:
			ns = ST;
			break;
		default:
			erroroccured = 1;
			ns = (c != '\n' ? IGNORE_ERROR : STOP);
			break;
		}
	else if (current == BLANKS) {
		switch (cc) {
		case BLANK:
			ns = BLANKS;
			break;
		case ID_CHAR:
			ns = BUILD_ID;
			break;
		case SEARCH_TYPE_CHAR:
			ns = BUILD_ST;
			break;
		case END:
			ns = STOP;
			break;
		default:
			erroroccured = 1;
			ns = IGNORE_ERROR;
			break;
		}
	} else if (current == IGNORE) {
		switch (cc) {
		case ID_CHAR:
			ns = IGNORE;
			break;
		case BLANK:
			ns = IDENTIFIER;
			break;
		case END:
			ns = END_QUERY;
			break;
		default:
			ns = IGNORE_ERROR;
			break;
		}
	} else if (current == IGNORE_ERROR) {
	    switch (cc) {
	    case END:
	        ns = STOP;
	        break;
	    default:
	        ns = IGNORE_ERROR;
	        break;
	    }
	}
	return ns;
}

/*
 * Reads a valid query.
 */
int get_query(char **keywords, unsigned short *st, int *wcount)
{
	State cs;		/* current state */
	char realword[MAXWORD];
	/* 
	   extern int erroroccured, exit_main;
	   extern char *word, stsofar;*/
	/* initialize extern variables for this read */
	word = realword;
	idccount = tokencount = erroroccured  = 0;
	stsofar = 'n'; 
	int i =0;
	int sepcount = 0; /* this is the search type token count i.e '|' or '&' count */
	/* 'tokencount' is the total count of tokens together with
	   search type tokens */
	cs = START;
	do {
		if (cs == IDENTIFIER) {
		    /*printf("%s - Identifier\n", realword);*/  /* we cannot use word. it;s just a pointer and 
			  at this moment it has been incremented */
		    cs = START;
		    ++tokencount;
		    word = realword; 
		    idccount = 0; 
		    keywords[i++] = STRCPY(realword);
		    /* STRCPY is similar to strcpy((char *)malloc(strlen(realword) + 1), realword);*/
			if (ISEVEN(tokencount) && stsofar != 'n') {
				fprintf(stderr, "\ntoken '%s' at wrong position", realword);
				SET();
			}
		} else if (cs == ST) {
		    cs = START;
			++tokencount; 
			++sepcount;
			if (!ISEVEN(tokencount)) {
				fprintf(stderr, "\nSEARCH TYPE token at wrong position...");
				SET();
			}
		} else if (cs == END_QUERY) {
			/*printf("%s - Identifier\n", realword);word = realword;idccount = 0; */
			++tokencount;
			keywords[i++] = STRCPY(realword);
			if (ISEVEN(tokencount) && stsofar != 'n' ) {
				fprintf(stderr, "\ntoken '%s' at wrong position", realword);
				SET();
			}
			break;
		}
		cs = transition(cs, get_next(stdin));
	} while (cs != STOP && i < MAX_KEYWORDS && !exit_main);
	
	if (c != '\n')	while ((c = getc(stdin)) != '\n');
	
	if (exit_main)	return EXIT;
	/* now check for duplicate keywords */
	*wcount = tokencount - sepcount;
	int a, b;
    for (a = 0; a < *wcount - 1; ++a) 
    	for (b = a + 1; b < *wcount; ++b)
			if(strcmp(keywords[a], keywords[b]) == 0) {
				fprintf(stderr, "\nDuplicate keywords...\n"); 
				return ERROR;
			}   	
	if (((tokencount - (sepcount << 1U) != 1) && stsofar != 'n') || 
		erroroccured || tokencount == 0) {
		fprintf(stderr, "\nInvalid query format...\n");
		return ERROR;
	} 
	
	switch (stsofar) {/* get the search type: 0-notype, 1-and, 2-or */
	case 'n':
		*st = 0;
		break;
	case '&':
		*st = 1;
		break;
	case '|':
		*st = 2;
		break;
	}
	return OK;	
}

#ifdef TEST
int 
main(void)
{
    char *keywords[KW_SIZE];
    unsigned short search_type;
    int wcount;
    extern int exit_main, ccount;
    int i, type;
    /* read the root */
    /* build the hash map */
    /* search!*/
    for (;;) {
        exit_main = ccount = wcount = 0;
        initialize(keywords);
        printf("query >> ");
        type = get_query(keywords, &search_type, &wcount);
        if (type == OK) {
            
            for (i = 0; keywords[i] != NULL; ++i) {
				fprintf(stdout, "%s : \n", keywords[i]);
				/* now search */
        	}
        	printf("search type: %d\nnwords: %d\n", search_type, wcount);
        } else if (type == ERROR) {
            /* printf("ERROR ERROR ERROR\n"); */
            free_keywords(keywords);
        } else {
            fprintf(stdout, "\nexitting program...\n");
            exit(EXIT_SUCCESS);
        }
        free_keywords(keywords); /* free all allocated memory */
    }
}
#endif
