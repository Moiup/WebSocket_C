/**
 * Name: server.c
 *
 * Description: The server code example
 *
 * Author: Mohammed-Bashir Mahdi
 *
 * Date: 22/08/2019
 *
 */
#include "useful.h"
#include "easySocket/easySocket.h"
#include <stdio.h>
#include <stdlib.h>
#include "WebSocket/WebSocket.h"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8000
#define MAX_CONNECTION 15


/*****************************/
/* Global variables          */
/*****************************/
int sock_id;
int client_id;



/**
*
*/
void close_server(int _n){
    fprintf(stdout, "Shuting down the server.\n");
    close(sock_id);
    close(client_id);
    exit(EXIT_SUCCESS);
}



int main(){
    char *response_header;
    unsigned long int data_len;
    byte *data;
    byte *to_send_df;
    unsigned long int to_send_df_size;
    char *msg_read;
    byte opcode;
    int is_sent;
    int i;

    add_event_listener(SIGINT, close_server);

    /* Creating the socket */
    sock_id = easySocket_server_creation(
                                        SERVER_ADDRESS,
                                        SERVER_PORT,
                                        MAX_CONNECTION
                                    );
                                    
    if(sock_id == easySocket_NO_VAL){
        fprintf(stderr, "Error creating the server socket.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Server launched on %s - port : %d\n",
                                                        SERVER_ADDRESS,
                                                        SERVER_PORT
                                                    );

    /* Accepting a client */
    client_id = easySocket_server_accept(sock_id);
    if(client_id == easySocket_NO_VAL){
        fprintf(stderr, "Error accepting client.\n");
        exit(EXIT_FAILURE);
    }

    /* Reading the handshake header from the client */
    msg_read = websocket_get_client_handshake(client_id);
    
    fprintf(stdout, "%s", msg_read);

    /* Preparing the handshake response */
    response_header = websocket_handshake_header_response(msg_read);

    fprintf(stdout, "\n TO SEND 1:\n");
    for(int i= 0; i < 19; i++){
        fprintf(stdout, "%c", response_header[i]);
    }

    fprintf(stdout, "\n TO SEND 2:\n%s", strdup(response_header));
    /* Sending the handshake response */
    websocket_send(client_id, response_header, strlen(response_header));


    fprintf(stdout, "\nHandshake finished\n");
    data = webSocket_read_msg(client_id, &data_len);

    fprintf(stdout, "\n%s\n", data);
    fprintf(stdout, "\n%ld\n", data_len);

    opcode = websocket_create_opcode(1, 0, 0, 0, 1);

    to_send_df = websocket_create_dataframe(data_len, data, opcode, &to_send_df_size);

    is_sent = websocket_send(client_id, (byte *)to_send_df, to_send_df_size);

    for(i = 0; i < 5; i++){
        bitByte_display(&to_send_df[i]);
        fprintf(stdout, "\n");
    }

    fprintf(stdout, "is_sent: %d\n", is_sent);

    free(to_send_df);
    free(response_header);
    free(msg_read);
    close(sock_id);
    close(client_id);

    return EXIT_SUCCESS;
}
