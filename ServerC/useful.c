#include "useful.h"

/**
 * An event (signal) listener, like in JavaScript (AddEventListener)
 */
void add_event_listener(int signal, void (*func)(int)){
    struct sigaction action;

    action.sa_handler = func;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(signal, &action, NULL);
}


/**
 * Return the number of digit encoding an integer
 * n: the integer
 * divider: will be stored the begin value to use in from_text_len_to_string
*/
int nb_digit_in_int(int n, int *begin){
    int length = 1;
    *begin = 1;

    while(n > ENCODING_STEP){
        n = n / ENCODING_STEP;
        *begin = *begin * ENCODING_STEP;
        length++;
    }

    return length;
}
