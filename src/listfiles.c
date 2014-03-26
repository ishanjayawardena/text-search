/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * listfiles.c
 ******************************************************************************/
#include "engine.h"
#include "list.h"
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define TEST
#undef TEST

/*
 * Decide if the regular file in 'filepath' is a
 * text file or not.
 */
int istextfile(STRING filepath)
{
	FILE *fp;
	int n;
	char c;
	
	if((fp = fopen(filepath, "r")) != NULL) {
		for (n = 0;((c = getc(fp)) != EOF) && n < MAXCHARS; ++n) {
			if ((!isprint(c) && !iscntrl(c))) {
				/* NOT an ASCII text file	*/
				return FALSE;
			}
		}
		/* OK we hope this to be a text file */
		fclose(fp);
		return TRUE;
	} else {
		/* fprintf(stderr, "Cannot open file: %s\n", filepath);	*/
		return FALSE;
	}
	
}

/*
 * determines if the string 'name' corresponds
 * to a text file.
 */
static LIST pathlist = NULL;
static size_t rootsize;
static int n;
static char errorpath[PATHSIZE / 4];

static void start(STRING name)
{
	struct stat stbuf;
	fflush(stdout);
	if (stat(name, &stbuf) == -1) {
		if (n == 0) { /* 'n' is used as a flag to decide if the name is the rootdir */
			/* the root directory itself cannot be accessed !*/
			sprintf(errorpath, "\rcannot open directory: %s", name);
			printnexit(errorpath);
		}
		return; /* ignore all the other directories that cannot
				   be opened */
	}
	++n;
	printf("\rplease wait...");
	fflush(stdout);
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name, start); /* a directory */
	if (S_ISREG(stbuf.st_mode))
		if (istextfile(name)) {/* a text file */
			add_to_list(&pathlist, make_newnode(&name[rootsize]));
		}
}

/* 
 * dirwalk: apply fcn to all files in dir	
 */
void dirwalk(STRING dir, void (*fcn)(STRING))
{
	char name[PATHSIZE];
	struct dirent *dp;
	DIR *dfd;
	fflush(stdout);
	if ((dfd = opendir(dir)) == NULL) {
		/* fprintf(stderr, "dirwalk: can't open %s\n", dir);	*/
		/* if we cannot open a directory, just skip it silently	*/
		return;
	}
	printf("\rplease wait...");
	fflush(stdout);
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0 ||
			strcmp(dp->d_name, "..") == 0)
				continue;
		if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
		else {
			if (dp->d_name[strlen(dp->d_name) - 1] != '~') { /* skip temporary files	*/
				sprintf(name, "%s/%s", dir, dp->d_name);
				(*fcn)(name);
			}
		}
	}
	closedir(dfd);
}


/*
 * This does a lot of work to come up with a correct
 * absolute root directory path name by removing
 * additional back slashes
 */
unsigned int write_paths(STRING pathp, STRING **subpaths)
{
    
    char *pwd = getenv("PWD");	/* sometimes we need the pwd as the root directory */
    char *newargv1 = NULL;		/* we use this to replace argv[1] */
    char *root = NULL;			/* the search for text files begins here */
    int size = 0;	
    n = 0;		
     /*	here we check for root names that end with a '/' and remove that
    					from its name and build a nicer name */
    if (pathp[strlen(pathp) - 1] == SEP) {
    	newargv1 = MAKESTR(strlen(pathp) - 1);
    	strncat(newargv1, pathp, strlen(pathp) - 1);
    } else { /*	this is a good name already	*/
    	newargv1 = pathp;
    }
    if (strcmp(newargv1, ".") == 0) { /* so the root is the pwd	*/
    	size = strrchr(pwd, '\0') - pwd;
    	root = MAKESTR(size);
    	strcat(root, pwd);
    } else if (strcmp(newargv1, "..") == 0) { /* so the root is parent of pwd	*/
    	size = strrchr(pwd, SEP) - pwd;/* we remove the name of pwd from pwd to get 
    									the name of parent of pwd	*/
    	root = MAKESTR(size);
    	strncat(root, pwd, size);
    } else { /* we have to anticipate root names like "../one/two" OR "./one/two"	*/
    	if (strncmp(newargv1, "../", 3) == 0) {
	    	size = strrchr(pwd, SEP) - pwd + strlen(newargv1) - 2;
	    	root = MAKESTR(size);
	    	strncat(root, pwd, strrchr(pwd, SEP) - pwd);
	    	strcat(root, &newargv1[2]);
    	} else if (strncmp(newargv1, "./", 2) == 0) {
    		size = strrchr(pwd, '\0') - pwd + strlen(newargv1) - 1;
    		root = MAKESTR(size);
    		strcat(root, pwd);
    		strcat(root, &newargv1[1]);
    	} else { /* a normal absolute path 
    			eg: "/home/ishan/devel/C/miniproject/beta3_3/three/one/dire.c"	*/
    		root = strcpy(MAKESTR(strlen(newargv1)), newargv1);
    		/* we still have to do this, otherwise 'pathp' will store
    		   a garbage value in it once exited from this function. 
    		   This gave me a lot of trouble. see below */
    	}
    }
   
    /* now remove additional SEP chars from path */
    int i, j, this, pre;
    this = pre = j = 0;
    char temp[PATHSIZE];
    for (i = 0; root[i] != '\0' && i < PATHSIZE; ++i) {
        if (root[i] == SEP) this = 1;
        else this = 0;
        if (!(pre == 1 && root[i] == SEP)) temp[j++] = root[i];
        pre = this;
    }
    if (temp[j-1] == SEP) temp[j-1] = '\0';
    else temp[j] = '\0';
    rootsize = strlen(temp);
    unsigned int listsize = get_listsize(pathlist);
    make_list(&pathlist);
    start(temp); /*	start listing directories	*/
    listsize = get_listsize(pathlist);
    strcpy(pathp, temp);
    free(root);
    /* make an array of sub paths instead of the linked list and 
       delete the linked list.*/
    *subpaths = make_array(pathlist);
    clear_list(&pathlist);
    return listsize;
}

#ifdef TEST
/* print file sizes	*/
int 
main(int argc, char **argv)
{
	if (argc == 1) write_paths(".");
	else
		while (--argc > 0)
			write_paths(*++argv);
	return 0;
}
#endif
