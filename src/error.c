/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * error.c
 ******************************************************************************/
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
/*
 * Prints the error message 's' and die.
 */
int printnexit(STRING s)
{
    printf("\r%80c", ' ');
    fprintf(stderr, "\n\n%s\nEXITING PROGRAM..............\n", s);
    exit(EXIT_FAILURE);
}
