/**
 * Name: easySocket.c
 *
 * Description: The definition of the my socket function. The goal is to mask the POSIX socket function to make the easier to use
 *
 * Creation date: 19/08/2019
 *
 * Author: Mohammed-Bashir Mahdi
 */
#include "easySocket.h"

/**
 * The socket address size
 */
const int easySocket_size_addr = sizeof(struct sockaddr_in);


/**
 * Return the server socket id if succeed, easySocket_NO_VAL otherwise
 */
int easySocket_server_creation(char *address, int port, int max_connection){
    int sock_id;
    struct sockaddr_in socket_address;
    
    /*errno = 0;*/
    /* Creating the socket */
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_id == easySocket_NO_VAL){
        return easySocket_NO_VAL;
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr(address);
    socket_address.sin_port = htons(port);
    /*errno = 0;*/
    /* We bind the sock_id with the socket_address */
    if(bind(sock_id, (struct sockaddr*)&socket_address, easySocket_size_addr) == easySocket_NO_VAL){
        return easySocket_NO_VAL;
    }

    /* We put the system in listening mode, so that we can listen to client connections */
    if(listen(sock_id, max_connection) == easySocket_NO_VAL){
        return easySocket_NO_VAL;
    }
    return sock_id;
}

/**
 * Return the Id of the accepted client, or -1 if failed
 * Check errno value
 * Mask the POSIX accept function
 *
 * To access the size given by accept, there is the global variable : easySocket_accept_size_addr
 * To access the client sockaddr_in, ther is the global variable : easySocket_accept_client_address
 */
int easySocket_server_accept(int sock_id){
    return accept(
                sock_id,
                (struct sockaddr*)&easySocket_accept_client_address,
                (socklen_t *)&easySocket_accept_size_addr
            );
}

/**
 * Return the client socket id
 */
int easySocket_client_creation(){
    int sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_id == easySocket_NO_VAL){
        return easySocket_NO_VAL;
    }
    return sock_id;
}


/**
 * Connect the client to the server identified by its address and port
 * sock_id is the id gotten with easySocket_client_creation()
 */
int easySocket_client_connect(int sock_id, char *address, int port){
    struct sockaddr_in server_socket;
    struct sockaddr_in *server_addresse;
    struct addrinfo indices;
    struct addrinfo *res;
    int size_addr;

    size_addr = sizeof(indices);
    memset(&indices, 0, sizeof(indices));
    indices.ai_family = AF_INET;

    if(getaddrinfo(address, NULL, &indices, &res) == easySocket_NO_VAL){
        return easySocket_NO_VAL;
        exit(EXIT_FAILURE);
    }

    server_addresse = (struct sockaddr_in *)res->ai_addr;
    server_socket.sin_addr = server_addresse->sin_addr;
    server_socket.sin_port = htons(port);
    server_socket.sin_family = AF_INET;
    size_addr = sizeof(server_socket);

    if(connect(sock_id, (struct sockaddr*)&server_socket, (socklen_t)size_addr) == easySocket_NO_VAL){
        return easySocket_NO_VAL;
    }
    return easySocket_SUCCESS;
}

/**
 * Send a message to a client (client_id)
 */
int easySocket_send_message(int dest_id, void *message){
    return send(dest_id, message, strlen(message) + 1, 0);
}

/**
 * Set a socket to non-blocking
 */
void easySocket_set_non_block(int sock_id){
    int flags = fcntl(sock_id, F_GETFL, 0);
    fcntl(sock_id, F_SETFL, flags | O_NONBLOCK);
}

/**
 * Read a message in a buffer, for a given size (it is just a mask of the original read function)
 */
size_t easySocket_read(int sock_id, void *message_buffer, size_t count){
    return read(sock_id, message_buffer, count);
}
