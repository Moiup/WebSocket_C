/**
 * Name: bitByte.c
 *
 * Description: The declaration of the bitBytes function. The goal here is to be albe to work with bytes and bits.
 *
 * Creation date: 01/04/2020
 *
 * Author: Mohammed-Bashir Mahdi
 */
#ifndef BITBYTE_H
#define BITBYTE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BITBYTE_SIZE 8
#define BITBYTE_MAX 7 /* The highest index range */
#define BITBYTE_FALSE 0
#define BITBYTE_TRUE 1

/* Byte type */
typedef unsigned char byte;

/************************/
/* Function definition  */
/************************/


/**
 * Set the i th bit to 1 (BITBYTE_TRUE)
 *
 */
void bitByte_set_bit(byte *b, int i);

/**
 * Reset the i th bit to 0 (BITBYTE_FALSE)
 */
void bitByte_reset_bit(byte *b, int i);

/**
 * Return the i th bit value (0 or 1) (BITBYTE_FALSE or BITBYTE_TRUE)
 */
int bitByte_get(byte *b, int i);

/**
 * Initialize a byte to 0 (BITBYTE_FALSE)
 */
void bitByte_init(byte *b);


/**
 * Display all the bits contained in a byte
 */
void bitByte_display(byte *b);


#endif
