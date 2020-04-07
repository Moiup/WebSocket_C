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
#include "stringDyn/stringDyn.h"
#include "WebSocket/WebSocket.h"
#include <openssl/sha.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8000
#define MAX_CONNECTION 15

#define MSG_SIZE 10000


#define WEB_SOCKET_PROTOCOLE_ANSWER "HTTP/1.1 101 Switching Protocols\nUpgrade: websocket\nConnection: Upgrade\nSec-WebSocket-Accept:"

#define PROT_1 "HTTP/1.1 101 Switching Protocols\n"
#define PROT_2 "Upgrade: websocket\n"
#define PROT_3 "Connection: Upgrade\n"
#define PROT_4 "Sec-WebSocket-Accept: "

#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

#define NEW_LINE_SEP "\n"
#define SPACE_SEP " "
#define WEBSOCKET_KEY "Sec-WebSocket-Key: "



/*****************************/
/* Global variables          */
/*****************************/
int sock_id;
int client_id;


size_t b64_encoded_size(size_t inlen)
{
	size_t ret;

	ret = inlen;
	if (inlen % 3 != 0)
		ret += 3 - (inlen % 3);
	ret /= 3;
	ret *= 4;

	return ret;
}


char *b64_encode(const unsigned char *in, size_t len)
{
    const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char   *out;
	size_t  elen;
	size_t  i;
	size_t  j;
	size_t  v;

	if (in == NULL || len == 0)
		return NULL;

	elen = b64_encoded_size(len);
	out  = (char *)malloc(sizeof(char) * (elen+1));
	out[elen] = '\0';

	for (i=0, j=0; i<len; i+=3, j+=4) {
		v = in[i];
		v = i+1 < len ? v << 8 | in[i+1] : v << 8;
		v = i+2 < len ? v << 8 | in[i+2] : v << 8;

		out[j]   = b64chars[(v >> 18) & 0x3F];
		out[j+1] = b64chars[(v >> 12) & 0x3F];
		if (i+1 < len) {
			out[j+2] = b64chars[(v >> 6) & 0x3F];
		} else {
			out[j+2] = '=';
		}
		if (i+2 < len) {
			out[j+3] = b64chars[v & 0x3F];
		} else {
			out[j+3] = '=';
		}
	}

	return out;
}

/**
*
*/
void close_server(int _n){
    fprintf(stdout, "Shuting down the server.\n");
    close(sock_id);
    close(client_id);
    exit(EXIT_SUCCESS);
}


char *get_sec_WebSocket_key(char *request){
    int len = strlen(request);
    int key_size;
    int i;
    int j;
    int key_begin;
    int is_found = 0;
    int key_end;
    int cpt;
    char *key_header = NULL;
    
    i = 0;
    while((i < len) && !is_found){
        j = 0;
        if(request[i] == WEBSOCKET_KEY[j]){
            j++;
            while(request[i + j] == WEBSOCKET_KEY[j]){
                j++;
                if(WEBSOCKET_KEY[j] == '\0'){
                    is_found = 1;
                    key_begin = i;
                    key_end = i + j;
                    break;
                }
            }
            i = i + j;
        }
        else{
            i++;
        }
    }

    cpt = 0;
    while(request[key_end + cpt] != '\n'){
        cpt++;
    }

    key_begin = key_end;
    key_end = key_end + cpt;
    key_size = key_end - key_begin;

    key_header = (char *)malloc(sizeof(char) * key_size);
    if(key_header == NULL){
        fprintf(stderr, "(get_sec_WebSocket_key) Error allocating returned_key value\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "begin %d\n", key_begin);
    fprintf(stdout, "end %d\n", key_end);
    for(i = key_begin, j=0; i < key_end; i++, j++){
        fprintf(stdout, "%c", request[i]);
        key_header[j] = request[i]; 
    }
    fprintf(stdout, "\n");
    key_header[key_size - 1] = '\0';

    fprintf(stdout, "key header: %s\n", key_header);

    return key_header;
}

char *create_header(char *request){
    unsigned char digest[SHA_DIGEST_LENGTH];
    char *key_head = get_sec_WebSocket_key(request);
    // char *key_head = strdup("dGhlIHNhbXBsZSBub25jZQ==");
    unsigned char *unsigned_key;
    char *concat_keys = stringDyn_concat_new(key_head, GUID);
    int data_len = strlen(concat_keys);
    SHA_CTX shactx;
    char *b64 = NULL;
    char *header = NULL;

    unsigned_key = (unsigned char *)concat_keys;

    SHA1_Init(&shactx);
    SHA1_Update(&shactx, unsigned_key, data_len);
    SHA1_Final(digest, &shactx);


    
    b64 = b64_encode(digest, SHA_DIGEST_LENGTH);


    fprintf(stdout, "%s", PROT_4);

    header = stringDyn_concat_new(PROT_1, PROT_2);
    header = stringDyn_concat_realloc(header, PROT_3);
    header = stringDyn_concat_realloc(header, PROT_4);
    header = stringDyn_concat_realloc(header, b64);
    header = stringDyn_concat_realloc(header, NEW_LINE_SEP);
    header = stringDyn_concat_realloc(header, NEW_LINE_SEP);



    free(b64);
    free(key_head);
    free(concat_keys);
    b64 = NULL;
    key_head = NULL;
    concat_keys = NULL;

    fprintf(stdout, "\nb64: %s\n", b64);
    return header;
}


int main(){
    char *response_header;
    unsigned long int data_len;
    byte *data;
    // byte *to_send_df;
    // unsigned long int to_send_df_size;
    char msg_read[MSG_SIZE];
    // byte opcode;
    int is_sent;
    // unsigned long int i;

    byte test_msg[2];

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
    easySocket_read(client_id, msg_read, MSG_SIZE);
    fprintf(stdout, "%s", msg_read);

    /* Preparing the handshake response */
    response_header = create_header(msg_read);

    fprintf(stdout, "\n TO SEND 1:\n");
    for(int i= 0; i < 19; i++){
        fprintf(stdout, "%c", response_header[i]);
    }

    fprintf(stdout, "\n TO SEND 2:\n%s", strdup(response_header));
    /* Sending the handshake response */
    easySocket_send_message(client_id, response_header);


    fprintf(stdout, "\nHandshake finished\n");
    data = webSocket_read_msg(client_id, &data_len);

    fprintf(stdout, "\n%s\n", data);
    fprintf(stdout, "\n%ld\n", data_len);

    // opcode = websocket_create_opcode(1, 0, 0, 0, 1);

    // bitByte_display(&opcode);

    // to_send_df = websocket_create_dataframe(data_len, data, opcode, &to_send_df_size);
    // fprintf(stdout, "to_send_df_size: %ld\n", to_send_df_size);

    // for(i = 0; i < to_send_df_size; i++){
    //     bitByte_display(&to_send_df[i]);
    //     fprintf(stdout, "\n");
    // }

    test_msg[0] = 129;
    test_msg[1] = 0;

    is_sent = websocket_send_dataframe(client_id, test_msg, 3);


    fprintf(stdout, "is_sent: %d\n", is_sent);
    fprintf(stdout, "\n");

    // free(to_send_df);
    free(response_header);
    close(sock_id);
    close(client_id);

    return EXIT_SUCCESS;
}
