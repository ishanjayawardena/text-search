/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * search.c
 ******************************************************************************/
#include "engine.h"

static unsigned int comp_key(STRING s)
{
  unsigned int hashval;
  
  for (hashval = 0; *s != '\0'; ++s)
		hashval = *s + PRIME * hashval;
  return hash(hashval);
}



/*
 * For testing purposes only. Print the values 
 * of parameters of a search engine.
 */
static void print_se(ENGINE *se)
{
	printf("    hashtable[0]: %p\n", se->hashtable[0]);
	printf(" se->subpaths[0]: %p\n", se->subpaths[0]);
	printf(" se->id->rootdir: %s\n", se->id->rootdir);
	printf("se->id->opt_bits: %d\n", se->id->opt_bits);
	printf("      se->nfiles: %d\n", (int)se->nfiles);
	printf(" se->search_type: %d\n", se->search_type);
}


/*
 *
 */
static WORD_LIST lookup(WORD_LIST hashtable[], STRING word, unsigned int *key, size_t pos)
{
    WORD_LIST np;
    
    for (np = hashtable[*key]; np != NULL; np = np->next)
        if (strcmp(word, np->word) == 0) {
            set_bit(np->bitvec, pos, ON);
            return np;
        }
    return NULL;
}

/*
 * Searches the hash table for given word and if it 
 * is present, set the correct bit of the bitvector 'bv'
 * that stores the final result for the given search type
 * of this query.
 */
static WORD_LIST search_table(ENGINE *e, STRING word, BITVECTOR *bv)
{
    WORD_LIST np;
    
    for (np = e->hashtable[comp_key(word)]; np != NULL; np = np->next)
        if (strcmp(word, np->word) == 0) {
        	/* if search_type is 1 bv = np->bitvec & bv 
        					else   bv = np->bitvec | bv*/
        	if (e->search_type == 1) /* and */
        		and_bits(np->bitvec, bv);
        	else if (e->search_type == 2) /* or */
        		or_bits(np->bitvec, bv);
            return np;
        }
    return NULL;
}

/*
 * Tries to add the word in 'word' to hashtabel[]. 
 * Set the correct position of this word node's bitvector ON.
 */
static size_t numberoffiles;
static unsigned int numberofwords;

static WORD_LIST add_to_table(WORD_LIST hashtable[], STRING word, unsigned int *key, size_t pos)
{
    WORD_LIST np;
    
    if ((np = lookup(hashtable, word, key, pos)) == NULL) {
        np = TYPED_ALLOC(WORD);
        if (np == NULL || 
           (np->word = strcpy(MAKESTR(strlen(word)), word)) == NULL ||
           (np->bitvec = make_bitvec(numberoffiles)) == NULL) {
           	printnexit("cannot allocate memory for hash map...");
            return NULL;
        }
        set_bit(np->bitvec, pos, ON);
        np->next = hashtable[*key];
        hashtable[*key] = np;
        ++numberofwords;
    } 
    return np;
}

/*
 * Reads the next word from the buffer associated with 'fp'.
 *		These 'words' may not be actual English words! Rather, identifiers.		
 */
static int getword(STRING word, int limit, FILE *fp, unsigned int *keyp)
{
	/* in this program, the valid chars are (a-z) (A-Z) (0-9) and '_' 
	   so that any identifier consists of only these chars is a valid 'word' */
    int c;
    STRING w = word;
    unsigned int hashval = 0;
    *w = '\0'; /* no word has been read initially. */
    /* convert all the characters into lowercase before returning! */
    /* extract only desired set of characters */
    do {
        c = getc(fp);
    } while (isspace(c) || (!isalpha(c) && c != '_' && !isdigit(c) && c != EOF));
    if (c == EOF) return EOF;
    *w++ = (isalpha(c) != 0 ? tolower(c) : c);
    for (; --limit > 0; w++) {
        *w = getc(fp); 
        if (isalpha(*w)) *w = tolower(*w);
        if (!isalnum(*w) && *w != '_' && !isdigit(*w)) {/*removed '-' and '\''*/
            *w = ' '; /* would this change the file? No										*/
            ungetc(*w, fp);
            break;
        }  
    }
    *w = '\0';
    HASH(word); /* hash on the spot, calling a function involves a considerable overhead
    				when the files grow bigger in size. */
    return w[0];
}

/*
 * Makes a new search engine																
 */
static ENGINE * make_new_engine(int argc, STRING *argv)
{
    /* PART_I: initialize the search engine */
    ENGINE *e = TYPED_ALLOC(ENGINE);
    
	e->id = TYPED_ALLOC(INPUT_DATA);
	if (e == NULL || e->id == NULL)
		printnexit("cannot allocate memory for search engine...");
	read_input(argc, argv, e->id); /* get input options and root dir if any */
	putchar('\n');
	numberoffiles = e->nfiles = write_paths((e->id)->rootdir, &(e->subpaths)); /* write the 'paths' file and get the # of text files */
	if (e->nfiles <= 0)
		printnexit("no text files found...");
	
	unsigned int index;
	for (index = 0; index < HASHSIZE; ++index)
        e->hashtable[index] = NULL;
    
    /* PART_II: now fill the hash table and sub paths array */
    FILE *f;
    size_t searched_files = 0;
    unsigned int key;
    char word[MAXWORD];
    index = 0;
	char path[PATHSIZE/2];
	for (index = 0; index < e->nfiles; ++index) {
		/* create the absolute path name for this file and open it */
		/* add the words in this file to the hash map*/
    	sprintf(path, "%s%s", e->id->rootdir, e->subpaths[index]);
		if ((f = fopen(path, "r")) != NULL) {
			++searched_files;
			fflush(stdout);
			/* print the progress bar */
			printf("\rcompleted... %d/%d", (int)searched_files, (int)e->nfiles);
			while (getword(word, MAXWORD-1, f, &key) != EOF) {
			    if (add_to_table(e->hashtable, word, &key, index+1) == NULL)
			    	printnexit("low memory...");
			    
			}
			fclose(f);
		} else {
			/* fprintf(stderr, "could not open file %s\n", path);	 */
		}
	}
	
	if (numberofwords <= 0)
		printnexit("no words found...");
	
    return e;
}

/*
 * this is the implementation of our simple buffer. Its size is 10 KB.
 */
static int char_count = 0;
static int const max =   KILOBYTE * 10;
static char buffer[KILOBYTE * 10]; /* this is the buffer */
static char thispath[KILOBYTE/2];	/* we first read a small portion of the out put to this
								little buffer and then write it to the big one.
								it sort of accumulates a proper single output line. */


static void flush_buffer(void)
{
	fprintf(stdout, "%s", buffer);
	char_count = 0;	
}

static void print(STRING str)
{	
	int size = strlen(str);
	if (size <= max - char_count) /* NO ROOM */
		char_count += sprintf(buffer + char_count, "%s", str);
	else {
		/* printf("flushing...noroom for %s\n", str); */
		flush_buffer();
		/* printf("printing to buffer again...%s\n", str); */
		print(str);
	}
}

/* each and every bit of this bitvector maps to a string in subpaths[] array 
	so we need to extract each such ON bit's subpaths[] string */
static size_t matches = 0;

static void buffer_output(ENGINE *e, BITVECTOR *bv)
{
	size_t i;
	
	for (i = 0; i < e->nfiles; ++i) {
		if (test_bit(bv, i + 1)) {
			sprintf(thispath, "%s%s\n", e->id->rootdir, e->subpaths[i]);
			print(thispath); 
			++matches;
		}
	}
}
/* end of code related to buffering */

 
/*
 * this is search(). It does all the searching and buffering the output. 
 */
static size_t search(ENGINE *e, STRING keywords[])
{
	int i;
	WORD_LIST word;
	BITVECTOR *bv = make_bitvec(e->nfiles);
	if (e->search_type == 0) {
		set_bits(bv, ON);
		for (i = 0; keywords[i] != NULL; ++i) {
			word = search_table(e, keywords[i], bv);
			if (word != NULL) {	
				sprintf(thispath, "\nfound    : %s\n", keywords[i]); print(thispath);
				buffer_output(e, word->bitvec);
			} else {
				sprintf(thispath, "\nnot found: %s", keywords[i]); print(thispath);
			}
		}
	} else {
		/* this is similar to
		   (e->search_type == 1) ? set_bits(bv, ON): set_bits(bv, OFF);*/
		set_bits(bv, (e->search_type == 1) ? ON: OFF);
		for (i = 0; keywords[i] != NULL; ++i) {
			if (search_table(e, keywords[i], bv) != NULL) {
				/* print the paths in bitvector use buffer */
				sprintf(thispath, "found    : %s\n", keywords[i]); print(thispath);
			} else {
				sprintf(thispath, "not found: %s\n", keywords[i]); print(thispath);
				if (e->search_type == 1) set_bits(bv, OFF);
			}
		}
		buffer_output(e, bv);
	}
	delete_bitvec(bv);
	return 1;
}

/*
 * Main driver for our program!																
 */

int main(int argc, char *argv[])
{
	
	ENGINE *se = NULL;
	
	se = make_new_engine(argc, argv);
	/* now read the query */
	STRING st[] = {"NOTYPE", "AND", "OR", "ERROR TYPE"};
	STRING keywords[KW_SIZE];
	/* This keyword array has a size of one plus the real
	   allowable number of keywords. i.e, for this program, 16 + 1 = 17
	   The reason is that we want to keep the last element of this array
	   NULL and untouched since it is used as an SENTINEL value inside
	   loops to learn the bounds of this array. By doing this we can
	   save a lot of typing. */
    /* unsigned short search_type;*/
    int wcount;
    extern int exit_main, ccount; /* these are defined in fsm.h */
    int result;
	for (;;) {/* check the number of key words input */
        exit_main = ccount = wcount = matches = 0;
        initialize(keywords);
        printf("\r%80cquery >> ", ' ');
        result = get_query(keywords, &(se->search_type), &wcount);
        /* search types 0-NOTYPE, 1-AND, 2-OR */
        if (result == OK) {
        	if (search(se, keywords)) {/* -i option then -c option? */
        		flush_buffer(); 
        		if (!matches && wcount != 1) printf("\nno matches found...\n");
        		printf("\n\nsearch type     : %s\n", st[se->search_type]);
        		printf("key words       : %d\n", wcount);
        		printf("total words     : %u\n", numberofwords);
        		printf("total text files: %d\n", (int)se->nfiles);
        		if (matches) printf("files matched   : %d\n", (int)matches);
        	}
        } else if (result == ERROR) {
            /* printf("ERROR ERROR ERROR\n"); */
            free_keywords(keywords);
        } else { /* the result is EXIT */
            fprintf(stdout, "\nexiting program...\n");
            exit(EXIT_SUCCESS);
        }
        free_keywords(keywords); /* free all allocated memory */
    }
}

