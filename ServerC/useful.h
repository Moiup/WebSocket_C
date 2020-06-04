#ifndef USEFUL_H
#define USEFUL_H
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define NO_VAL -1

#define FALSE 0
#define TRUE 1



#define FLOAT_STRING_LENGTH 12
#define FLOAT_STRING_PRECISION "%.5f" /* Number of digits after the . */
#define ENCODING_STEP 10

/**
 * An event (signal) listener, like in JavaScript (AddEventListener)
 */
void add_event_listener(int signal, void (*func)(int));

/**
 * Return the number of digit encoding an integer
 * n: the integer
 * divider: will be stored the begin value to use in from_text_len_to_string
*/
int nb_digit_in_int(int n, int *begin);

#endif
