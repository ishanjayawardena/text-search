/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * read.c
 ******************************************************************************/
#include "engine.h"
#define GETTEST_2
#undef GETTEST_2

/*
 * Reads the command line arguments and determine their validity
 * and if valid, store them inside INPUT_DATA parameter to
 * the function.
 */
void read_input(int argc, STRING *argv, struct input_data *id)
{
	if (id == NULL)
		printnexit("could not allocate enough memory! for INPUT_DATA item...");
    S_BITVEC opt_bits = 0;	/*	bit vector that stores enabled options for this search		*/
    int opt;
    char *rootdir = ".";		/* root directory for this search							*/ 
    /*  c - match case
     *  d - path of the root directory where search must be started							*/
    extern char *optarg;
    extern int optind, optopt;
    while ((opt = getopt(argc, argv, ":cd:")) != -1)
        switch (opt) {
        case 'd':
            if (optarg) {
                rootdir = optarg;
                opt_bits = opt_bits | D;
            }
            break;
        case 'c':
            opt_bits = opt_bits | C;
            printf("option: %c - don\'t match case when searching\n", opt);
            break;
        case ':':
            printnexit("-d option needs a value!");
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
            printnexit("");
            break;
          
        }/*free after assignment															*/
    if (optind != argc) 
        printnexit("too many arguments to the program");
    strcpy(id->rootdir, rootdir);
    id->opt_bits = opt_bits;
}

static void print_input_data(struct input_data *p)
{
    printf("\nroot directory: %s\n\
            \roptbits: %d\n", p->rootdir, p->opt_bits);
}

#ifdef GETTEST_2
int main(int argc, char *argv[])
{
    struct input_data *id = NULL;
    id = TYPED_ALLOC(struct input_data);
   	read_input(argc, argv, id);
    print_input_data(id);
}
#endif
