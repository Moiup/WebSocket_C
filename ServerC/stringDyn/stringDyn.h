/**
 * 
 * Name: stringDyn.h
 * 
 * Description: declaration of string function. All uses dynamic allocation, so they need to be freed
 * 
 * Date: 2020/01/10
 * 
 * Author: Mohammed-Bashir Mahdi
*/
#ifndef STRING_DYN_H
#define STRING_DYN_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define stringDyn_MAX_INT_LENGTH 12
#define stringDyn_TEN_BASE 10

#define stringDyn_FLOAT_BEGIN 1


/**
 * Create a string with the content of s.
 * Useful if you want to write : stringDyn_create("hello world!");
 * 
 * Return the string, NULL if failed.
*/
char *stringDyn_create(char *s);

/**
 * Concatenate 2 string.
 * 
 * Return a new string from the concatenation of s1 and s2, NULL if failed.
*/
char *stringDyn_concat_new(char *s1, char *s2);

/**
 * Concatenate 2 string s1 and s1 by reallocating s1
 * 
 * Return the reallocated s1
 * 
*/
char *stringDyn_concat_realloc(char *s1, char *s2);

/**
 * Take an int and return a string encoding this integer
 * Return NULL if failed
*/
char *stringDyn_int_to_string(int n);

/**
 * Take a double and convert it into a string
 * n: the number of value to keep after the .
*/
char *stringDyn_float_to_string(float d, int n);


#endif
