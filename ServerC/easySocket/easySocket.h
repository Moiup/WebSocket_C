/**
 * Name: easySocket.c
 *
 * Description: The definition of the my socket function. The goal is to mask the POSIX socket function to make the easier to use
 *
 * Creation date: 19/08/2019
 *
 * Author: Mohammed-Bashir Mahdi
 */
#ifndef easySocket_H
#define easySocket_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>


#define easySocket_NO_VAL -1

#define easySocket_SUCCESS 1
#define easySocket_READ_ALL_MSG_BUFF_SIZE 50

int easySocket_accept_size_addr;
struct sockaddr_in easySocket_accept_client_address;

/**
 * Return the server socket id if succeed, easySocket_NO_VAL otherwise
 */
int easySocket_server_creation(char *address, int port, int max_connection);

/**
 * Return the Id of the accepted client, or -1 if failed
 * Check errno value
 * Mask the POSIX accept function
 *
 * To access the size given by accept, there is the global variable : easySocket_accept_size_addr
 * To access the client sockaddr_in, ther is the global variable : easySocket_accept_client_address
 */
int easySocket_server_accept(int sock_id);

/**
 * Return the client socket id, easySocket_NO_VAL if failed
 */
int easySocket_client_creation();

/**
 * Connect the client to the server identified by its address and port
 * sock_id is the id gotten with easySocket_client_creation()
 */
int easySocket_client_connect(int sock_id, char *address, int port);

/**
 * Send a message to a client (client_id)
 * Return easySocket_NO_VAL if failed
 */
int easySocket_send_message(int dest_id, void *message, size_t msg_len);

/**
 * Set a socket to non-blocking
 */
void easySocket_set_non_block(int sock_id);

/**
 * Read a message in a buffer, for a given size (it is just a mask of the original read function)
 */
size_t easySocket_read(int sock_id, void *message_buffer, size_t count);


#endif
