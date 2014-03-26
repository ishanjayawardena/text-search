/*******************************************************************************
 * 070206B I.U.Jayawardena udeshike@gmail.com
 * bitvec.c
 ******************************************************************************/
#include "bitvec.h"
#include "common.h"
#define TEST
#undef TEST

/*
 * Sets all the bits of bitvector 'b' to the
 * given type. This type may be either ON, or OFF.
 */
void set_bits(BITVECTOR *bitvec, size_t type)
{
    size_t i;
    bitvec_t t = (type == ON ? 0xffU : 0x00U);
    for (i = 0; i < N_BYTES(bitvec->size); ++i)
        bitvec->bitvec[i] = t;
}

/*
 * Flips all the bits of bitvector 'b'
 */
void flip_bits(BITVECTOR *b)
{
    size_t i;
    for (i = 0; i < N_BYTES(b->size); ++i)
        b->bitvec[i] = ~(b->bitvec[i]);
}

/*
 * Prints the bitvector in '1's and '0's
 */
void print_bits(BITVECTOR *b)
{
    size_t i;
    size_t n = BITS_PER_CHAR;	/* in limits.h
		n is the number of bits in a machine word */
    bitvec_t mask = 0x01U << (n - 1);			/* mask = 10000000
		or shift the low-order bit to the high-order end */
    size_t nb;
    size_t totalbits = 0;
    bitvec_t temp;
    for (nb = 0; nb < N_BYTES(b->size); ++nb) {
        temp = b->bitvec[nb];
        for (i = 1; i <= n && totalbits < b->size; ++i) {
            ++totalbits;
            putchar(((temp & mask) == 0) ? '0' : '1');
            /* An & mask checks if the high-order bit of a is on or not */
            temp <<= 1;
            /* brings the next bit of a into the high-order position		*/
        }
        putchar(' ');
    }
    putchar('\n');
}

/*
 * Prints the bitvector in decimal notation.
 * i.e if the bitvector 'b' consists of 4 bytes
 * the output may something like 124, 7, 89, 13
 * depending on the exact bit pattern present.
 */
void print_dec(BITVECTOR *b)
{
    size_t i;
    for (i = 0; i < N_BYTES(b->size); ++i)
        printf("%d,", b->bitvec[i]);
    printf("\n");
}

/*
 * Test the bit of 'b' at position to see
 * if it is a '1' or a '0'
 */
size_t test_bit(BITVECTOR *b, size_t position)
{
    if (position < 1U || position > b->size) {
        fprintf(stderr, "\ntest_bit():position out of bitvector bounds\n");
        exit(1);
    }
    return ((TEMP(b, position) & MASK(position)) == 0) ? OFF : ON;
}

/*
 * Returns the size of the bitvector
 */
size_t get_size(BITVECTOR *b)
{
    if (b != NULL)  return b->size;
    else            return -1;
}

/*
 * Sets the bit of 'b' at position to type
 */
void set_bit(BITVECTOR *b, size_t position, size_t type)
{
    if (position < 1U || position > b->size) {
        fprintf(stderr, "\nset_bit(): position out of bitvector bounds %ld\n", position);
        exit(1);
    }
    if (type == ON) TEMP(b, position) = TEMP(b, position) | MASK(position);
    else if (type == OFF)   TEMP(b, position) = TEMP(b, position) & ~MASK(position);
}

/*
 * These are 'and' and 'or' operations.
 * the second parameter accumulates the
 * result of relevant operation.
 * i.e. b = b | a
 *		b = b & a
 */
void and_bits(BITVECTOR *a, BITVECTOR *b)
{
    size_t i;
    /* set_bits(b, ON); */
    for (i = 0; i < N_BYTES(b->size); ++i)
        b->bitvec[i] = (a->bitvec[i]) & (b->bitvec[i]);
}

void or_bits(BITVECTOR *a, BITVECTOR *b)
{
    size_t i;
    /* set_bits(b, OFF); */
    for (i = 0; i < N_BYTES(b->size); ++i)
        b->bitvec[i] = (a->bitvec[i]) | (b->bitvec[i]);
}

/*
 * Makes a new bitvector of size 'size'
 * and set all the bits OFF.
 */
BITVECTOR * make_bitvec(size_t size)
{
    BITVECTOR *bv = (BITVECTOR *)malloc(sizeof(BITVECTOR));
    if (bv == NULL)
        printnexit("could not make a new bitvec...");
    bv->size = size;
    bv->bitvec = ALLOC(N_BYTES(bv->size));
    set_bits(bv, OFF);
    return bv;
}

/*
 * Deletes the bitvector pointed by 'b'
 */
void delete_bitvec(BITVECTOR *b)
{
    free(b->bitvec);
    free(b);
    b = NULL;
}

#ifdef TEST
int main(void)
{
    size_t n_bits = 23U;
    BITVECTOR *bitvec;

    bitvec = make_bitvec(n_bits);

    set_bits(bitvec, OFF);
    print_bits(bitvec);

    set_bit(bitvec, 18U, ON);
    set_bit(bitvec, 8U, ON);
    set_bit(bitvec, 2U, ON);
    set_bit(bitvec, 12U, ON);
    set_bit(bitvec, 5U, ON);
    set_bit(bitvec, 10U, ON);
    set_bit(bitvec, 15U, ON);
    set_bit(bitvec, 17U, ON);
    set_bit(bitvec, 1U, ON);
    set_bit(bitvec, 20U, ON);
    set_bit(bitvec, 13U, OFF);
    set_bit(bitvec, 7U, OFF);
    print_bits(bitvec);

    printf("10: %d\n", test_bit(bitvec, 10U));
    printf("11: %d\n", test_bit(bitvec, 11U));
    printf("17: %d\n", test_bit(bitvec, 17U));
    printf("18: %d\n", test_bit(bitvec, 18U));
    printf("19: %d\n", test_bit(bitvec, 19U));
    print_bits(bitvec);

    printf("\nflip twice:\n");
    flip_bits(bitvec);
    print_bits(bitvec);
    flip_bits(bitvec);
    print_bits(bitvec);

    printf("\nflip bit 8, 18, 20:\n");
    print_bits(bitvec);
    flip_bit(bitvec, 8U);
    flip_bit(bitvec, 18U);
    flip_bit(bitvec, 20U);
    print_bits(bitvec);
    print_dec(bitvec);
    delete(bitvec);
    exit(EXIT_SUCCESS);
}
#endif

