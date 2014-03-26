/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * bitvec.h
 ******************************************************************************/
#ifndef BITVEC_H_
#define BITVEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define BITS_PER_CHAR (CHAR_BIT * sizeof (unsigned char))
#define N_BYTES(n) (int) ceil((double) n / (double) BITS_PER_CHAR)
#define ALLOC(n) (unsigned char *) calloc(n, sizeof (unsigned char))
#define INDEX(n) ((position - 1U) / CHAR_BIT)
#define BITNUMBER(n) ((n - 1U) % CHAR_BIT)
#define flip_bit(b, n)												\
	(test_bit(b, n) == 0) ? set_bit(b, n, ON) : set_bit(b, n, OFF)
#define TEMP(b, n) (b->bitvec[INDEX(n)])
#define MASK(n) (0x01U << ((CHAR_BIT - 1U) - BITNUMBER(n)))
#define ON 1
#define OFF 0

typedef unsigned char bitvec_t;
typedef struct bitvector_t {
	bitvec_t *bitvec;
    size_t size;
} BITVECTOR;

/* function prototypes for the functions defined in bitvec.c */
extern void set_bit(struct bitvector_t *bitvector, size_t position, size_t type);
extern void set_bits(struct bitvector_t *bitvector, size_t type);
/* there is a flip_bit(struct bitvector_t *, size_t) macro */
extern void flip_bits(struct bitvector_t *bitvector);
extern size_t test_bit(struct bitvector_t *bitvector, size_t position);
extern size_t get_size(struct bitvector_t *bitvector);
extern void and_bits(struct bitvector_t *bitvector_a, BITVECTOR *bitvector_b);
extern void or_bits(struct bitvector_t *bitvector_a, struct bitvector_t *bitvector_b);
extern void print_bits(struct bitvector_t *bitvector);
extern void print_dec(struct bitvector_t *bitvector);
extern struct bitvector_t *make_bitvec(size_t size);
extern void delete_bitvec(struct bitvector_t *bitvector);

#endif /* BITVEC_H_ */
