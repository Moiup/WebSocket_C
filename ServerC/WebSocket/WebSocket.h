/**
 * Name: WebSocket.c
 *
 * Description: The declataiton of the WebSocket function. WebSocket implementation to use them in C
 *
 * Creation date: 02/04/2020
 *
 * Author: Mohammed-Bashir Mahdi
 */
#ifndef WEBSOCKET_H
#define WEBSOCKET_H
#include <stdlib.h>
#include <stdio.h>
#include "../bitByte/bitByte.h"
#include "../easySocket/easySocket.h"

#define WS_NO_VAL -1 /* When something is not working properly */
#define WS_TRUE 1
#define WS_FALSE 0

/********************/
/* Size definition  */
/********************/
#define WS_FIN 1 /* 1 bit */
#define WS_RSV1 1 /* 1 bit */
#define WS_RSV2 1 /* 1 bit */
#define WS_RSV3 1 /* 1 bit */
#define WS_OPCODE 4 /* 4 bits */

#define WS_DF_BASE_SIZE_BITS 16 /* 16 bits */
#define WS_DF_BASE_SIZE_BYTES 2 /* 2 bytes */

#define WS_MASK 1
#define WS_PAYLOAD_LEN 7 /* 7 bits */
#define WS_PLAYLOAD_16_LEN 16 /* 16 bits */
#define WS_PLAYLOAD_64_LEN 64 /* 64 bits */

#define WS_MASKIN_KEY 4 /* 4 bytes */

/********************/
/* Index definition  */
/********************/
#define WS_FIN_I 0 /* 1 bit */
#define WS_RSV1_I 1 /* 1 bit */
#define WS_RSV2_I 2 /* 1 bit */
#define WS_RSV3_I 3 /* 1 bit */
#define WS_OPCODE_I 4 /* 4 bits */

#define WS_MASK_I 0
#define WS_PAYLOAD_LEN_I 1 /* 7 bits */

/* Playload management */
#define  WS_PLAYLOAD_16 126
#define  WS_PLAYLOAD_64 127

#define WS_PLAYLOAD_16_SIZE 2
#define WS_PLAYLOAD_64_SIZE 8

#define WS_2_POW_8 256 /* 1 */
#define WS_2_POW_16 65536 /* 2 */
#define WS_2_POW_24 16777216 /* 3 */
#define WS_2_POW_32 4294967296 /* 4 */
#define WS_2_POW_40 1099511627776 /* 5 */
#define WS_2_POW_48 281474976710656 /* 6 */
#define WS_2_POW_56 72057594037927935 /* 7 */

/***************/
/* Masking Key */
/***************/
#define MASKING_KEY_SIZE 4

/**
 * Return the length of the playload data
 */
unsigned long int websocket_playload_len_read(int client_id, byte playload);

/**
 * Read the bytes corresponding to the masking key and store it in masking_key
 * Return 0 if failure, WS_TRUE otherwise
 */
int websocket_read_masking_key(int client_id, byte *masking_key);

/**
 * Read a websocket message
 */
byte *webSocket_read_msg(int client_id, unsigned long int *data_len);

/**
 * Generate the 8 first bit with the given opcode
 */
byte websocket_create_opcode(int fin, int rsv1, int rsv2, int rsv3, int opcode);

/**
 * Return a valid dataframe
 */
byte *websocket_create_dataframe(unsigned long int data_len, byte *data, byte opcode, unsigned long int *dataframe_size);

/**
 * Send a dataframe
 * Return 0 if nothing sent
 */
int websocket_send_dataframe(int sock_id, byte *dataframe, unsigned long int dataframe_size);

#endif