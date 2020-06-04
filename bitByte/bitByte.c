/**
 * Name: bitByte.c
 *
 * Description: The definition of the bitBytes function. The goal here is to be albe to work with bytes and bits.
 *
 * Creation date: 01/04/2020
 *
 * Author: Mohammed-Bashir Mahdi
 */
#include "bitByte.h"

/************************/
/* Function declaration */
/************************/


/**
 * Set the i th bit to 1
 *
 */
void bitByte_set_bit(byte *b, int i){
    b[(BITBYTE_MAX - i)/BITBYTE_SIZE] |= BITBYTE_TRUE << ((BITBYTE_MAX - i)%BITBYTE_SIZE);
}

/**
 * Reset the i th bit to 0
 */
void bitByte_reset_bit(byte *b, int i){
    b[(BITBYTE_MAX - i)/BITBYTE_SIZE] &= ~(BITBYTE_TRUE << ((BITBYTE_MAX - i) % BITBYTE_SIZE));
}

/**
 * Return the i th bit value (0 or 1)
 */
int bitByte_get(byte *b, int i){
    return ((b[(BITBYTE_MAX - i)/BITBYTE_SIZE] & (BITBYTE_TRUE << ((BITBYTE_MAX - i) % BITBYTE_SIZE))) != BITBYTE_FALSE);
}

/**
 * Initialize a byte to 0
 */
void bitByte_init(byte *b){
    *b = 0;
}

/**
 * Return the i th byte of a type 
 */
byte bitByte_get_byte(unsigned long int b, int i){
    byte res;

    res = b >> (i * BITBYTE_SIZE);

    return res;
}


/**
 * Display all the bits contained in a byte
 */
void bitByte_display(byte *b){
    int i;

    for(i = 0; i < BITBYTE_SIZE; i++){
        fprintf(stdout, "%d", bitByte_get(b, i));
    }
}

