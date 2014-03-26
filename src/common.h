/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * common.h
 ******************************************************************************/
#ifndef COMMON_H_
#define COMMON_H_

typedef char *STRING;
int printnexit(STRING);
#define N_CHARS 20 /* maximum # characters per word */
#define MAXWORD N_CHARS + 1	/*	maximum # of chars in a word	*/
#define MAX_KEYWORDS 16 /* maximum # keywords for a query */
#define KW_SIZE MAX_KEYWORDS + 1
#endif
