/**
 * Name: WebSocket.c
 *
 * Description: The definition of the WebSocket function. WebSocket implementation to use them in C
 *
 * Creation date: 02/04/2020
 *
 * Author: Mohammed-Bashir Mahdi
 */
#include "WebSocket.h"

/**
 * Return the 
*/
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
 * Return the key stored in the handshake header
*/
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
        if(request[i] == WS_WEBSOCKET_KEY[j]){
            j++;
            while(request[i + j] == WS_WEBSOCKET_KEY[j]){
                j++;
                if(WS_WEBSOCKET_KEY[j] == '\0'){
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
        return NULL;
    }

    for(i = key_begin, j=0; i < key_end; i++, j++){
        key_header[j] = request[i]; 
    }

    key_header[key_size - 1] = '\0';

    return key_header;
}

/**
 * Create the handshake header response
*/
char *websocket_handshake_header_response(char *request){
    unsigned char digest[SHA_DIGEST_LENGTH];
    char *key_head = get_sec_WebSocket_key(request);
    // char *key_head = strdup("dGhlIHNhbXBsZSBub25jZQ==");
    unsigned char *unsigned_key;
    char *concat_keys = stringDyn_concat_new(key_head, WS_GUID);
    int data_len = strlen(concat_keys);
    SHA_CTX shactx;
    char *b64 = NULL;
    char *header = NULL;

    unsigned_key = (unsigned char *)concat_keys;

    SHA1_Init(&shactx);
    SHA1_Update(&shactx, unsigned_key, data_len);
    SHA1_Final(digest, &shactx);

    b64 = b64_encode(digest, SHA_DIGEST_LENGTH);

    header = stringDyn_concat_new(WS_PROT_1, WS_PROT_2);
    header = stringDyn_concat_realloc(header, WS_PROT_3);
    header = stringDyn_concat_realloc(header, WS_PROT_4);
    header = stringDyn_concat_realloc(header, b64);
    header = stringDyn_concat_realloc(header, WS_NEW_LINE_SEP);
    header = stringDyn_concat_realloc(header, WS_NEW_LINE_SEP);

    free(b64);
    free(key_head);
    free(concat_keys);
    b64 = NULL;
    key_head = NULL;
    concat_keys = NULL;

    return header;
}

/**
 * Reading and returning the handshake header received from the client
 * Return NULL if error
*/
char *websocket_get_client_handshake(int client_id){
    char *msg_read = NULL;
    int is_read;

    msg_read = (char *)malloc(WS_CLIENT_HANDSHAKE_SIZE * sizeof(char));
    if(msg_read == NULL){
        return NULL;
    }

    is_read = easySocket_read(client_id, msg_read, WS_CLIENT_HANDSHAKE_SIZE);
    if(is_read){
        return msg_read;
    }

    /* Failed to read, must free the msg_read memory */
    free(msg_read);

    return NULL;
}


/**
 * Return the length of the playload data
 */
unsigned long int websocket_playload_len_read(int client_id, byte playload){
    byte data_len_16[WS_PLAYLOAD_16_SIZE];
    byte data_len_64[WS_PLAYLOAD_64_SIZE];
    int is_read;
    bitByte_reset_bit(&playload, WS_MASK_I);


    if(playload < WS_PLAYLOAD_16){
        return (unsigned long int)playload;
    }
    if(playload == WS_PLAYLOAD_16){
        is_read = easySocket_read(client_id, data_len_16, WS_PLAYLOAD_16_SIZE);
        if(is_read){
            return (unsigned long int)(data_len_16[0] * WS_2_POW_8 + data_len_16[1]);
        }
        return WS_FALSE;
    }
    if(playload == WS_PLAYLOAD_64){
        is_read = easySocket_read(client_id, data_len_64, WS_PLAYLOAD_64_SIZE);
        if(is_read){
            return (unsigned long int)(data_len_64[0] * WS_2_POW_56 + data_len_64[1] * WS_2_POW_48 + data_len_64[2] * WS_2_POW_40 + data_len_64[3] * WS_2_POW_32 + data_len_64[4] * WS_2_POW_24 + data_len_64[5] * WS_2_POW_16 + data_len_64[6] * WS_2_POW_8 + data_len_64[7]);
        }
        return WS_FALSE;
    }
    /* Must be managed the other data */
    return WS_FALSE;
}

/**
 * Read the bytes corresponding to the masking key and store it in masking_key
 * Return 0 if failure, WS_TRUE otherwise
 */
int websocket_read_masking_key(int client_id, byte *masking_key){
    return easySocket_read(client_id, masking_key, MASKING_KEY_SIZE);
}

/**
 * Return the data stored in the received datagram
*/
byte *websocket_read_data(int client_id, size_t data_len, byte *masking_key){
    int is_read;
    byte *data = NULL;
    unsigned long int i;
    unsigned long int j;
    
    /* Allocating data memory */
    data = (byte *)malloc((data_len) * sizeof(byte));
    if(data == NULL){
        return NULL;
    }

    /* Reading the data */
    is_read = easySocket_read(client_id, data, data_len);
    if(!is_read){
        /* Fail to read, must free the allocated data memory */
        free(data);
        return NULL;
    }

    for(i = 0; i < data_len; i++){
        j = i % MASKING_KEY_SIZE;

        data[i] = data[i] ^ masking_key[j];
    }

    return data;
}

/**
 * Read a websocket message
 * Send NULL if error ()
 */
byte *webSocket_read_msg(int client_id, size_t *data_len){
    int is_read;
    byte fin_1_2_3_opcode;
    byte mask_playload;
    // unsigned long int data_len;
    byte masking_key[MASKING_KEY_SIZE];
    
    /* Reading :
     * - FIN (1 bit)
     * - RSV1 (1 bit)
     * - RSV2 (1 bit)
     * - RSV3 (1 bit)
     * - OPCODE (4 bits)
     */
    is_read = easySocket_read(client_id, &fin_1_2_3_opcode, 1);
    if(!is_read){
        return NULL;
    }

    is_read = easySocket_read(client_id, &mask_playload, 1);
    if(!is_read){
        return NULL;
    }

    if(!bitByte_get(&mask_playload, WS_MASK_I)){
        return NULL;
    }
    /**
     * From now own, we know that the mask value is set
     */
    *data_len = websocket_playload_len_read(client_id, mask_playload);


    is_read = websocket_read_masking_key(client_id, masking_key);
    if(!is_read){
        return NULL;
    }

    return websocket_read_data(client_id, *data_len, masking_key);
}

/**
 * Generate the 8 first bit with the 4 first bits and the opcode
 */
byte websocket_create_opcode(int fin, int rsv1, int rsv2, int rsv3, int opcode){
    byte b;
    b = opcode;

    if(fin){
        bitByte_set_bit(&b, WS_FIN_I);
    }
    if(rsv1){
        bitByte_set_bit(&b, WS_RSV1_I);
    }
    if(rsv2){
        bitByte_set_bit(&b, WS_RSV2_I);
    }
    if(rsv3){
        bitByte_set_bit(&b, WS_RSV3_I);
    }

    return b;
}


/**
 * Return a valid dataframe
 * NULL otherwise
 */
byte *websocket_create_dataframe(unsigned long int data_len, byte *data, byte opcode, size_t *dataframe_size){
    unsigned long int i;
    unsigned long int j;
    *dataframe_size = BITBYTE_SIZE;
    byte playload_len;
    byte *df = NULL; /* the dataframe */
    /* Putting FIN to 1 */
    bitByte_set_bit(&opcode, WS_FIN_I);

    if(data_len < WS_PLAYLOAD_16){
        playload_len = data_len;
        *dataframe_size = WS_DF_BASE_SIZE_BYTES + data_len;
        df = (byte *)malloc(*dataframe_size);
        if(df == NULL){
            return NULL;
        }


        for(i = WS_DF_BASE_SIZE_BYTES, j = 0; i < *dataframe_size; i++,j++){
            df[i] = data[j];
        }
    }
    else if(data_len <= BITBYTE_MAX_2_BYTE){
        playload_len = WS_PLAYLOAD_16;
        *dataframe_size = WS_DF_BASE_SIZE_BYTES + WS_PLAYLOAD_16_SIZE + data_len;
        df = (byte *)malloc(*dataframe_size);
        if(df == NULL){
            return NULL;
        }

        /* Affecting the data size to each bytes */
        df[WS_DF_BASE_SIZE_BYTES] = bitByte_get_byte(data_len, 1);
        df[WS_DF_BASE_SIZE_BYTES + 1] = bitByte_get_byte(data_len, 0);


        for(i = WS_DF_BASE_SIZE_BYTES + WS_PLAYLOAD_16_SIZE, j = 0; i < *dataframe_size; i++,j++){
            df[i] = data[j];
        }
    }
    else {
        playload_len = WS_PLAYLOAD_64;
        *dataframe_size = WS_DF_BASE_SIZE_BYTES + WS_PLAYLOAD_64_SIZE + data_len;
        df = (byte *)malloc(*dataframe_size);
        if(df == NULL){
            return NULL;
        }

        /* Affecting the data size to each bytes */
        df[WS_DF_BASE_SIZE_BYTES] = bitByte_get_byte(data_len, 7);
        df[WS_DF_BASE_SIZE_BYTES + 1] = bitByte_get_byte(data_len, 6);
        df[WS_DF_BASE_SIZE_BYTES + 2] = bitByte_get_byte(data_len, 5);
        df[WS_DF_BASE_SIZE_BYTES + 3] = bitByte_get_byte(data_len, 4);
        df[WS_DF_BASE_SIZE_BYTES + 4] = bitByte_get_byte(data_len, 3);
        df[WS_DF_BASE_SIZE_BYTES + 5] = bitByte_get_byte(data_len, 2);
        df[WS_DF_BASE_SIZE_BYTES + 6] = bitByte_get_byte(data_len, 1);
        df[WS_DF_BASE_SIZE_BYTES + 7] = bitByte_get_byte(data_len, 0);

        for(i = WS_DF_BASE_SIZE_BYTES + WS_PLAYLOAD_64_SIZE, j = 0; i < *dataframe_size; i++,j++){
            df[i] = data[j];
        }
    }


    df[0] = opcode;
    df[1] = playload_len;

    for(i = 0; i <  BITBYTE_SIZE; i++){
        if(bitByte_get(&opcode, i)){
            bitByte_set_bit(&df[0], i);
        }
        else{
            bitByte_reset_bit(&df[0], i);
        }
        if(bitByte_get(&playload_len, i)){
            bitByte_set_bit(&df[1], i);
        }
        else{
            bitByte_reset_bit(&df[1], i);
        }
    }

    return df;
}

/**
 * Send a datagramme created with websocket_create_dataframe(unsigned long int data_len, byte *data, byte opcode, unsigned long int *dataframe_size)
 * Return 0 if nothing sent
 */
int websocket_send_dataframe(int sock_id, void *data, size_t data_size){
    return send(sock_id, data, data_size, 0);
}

/**
 * Send a message using Websocket, no need to create an opcode and a datagramme, everything is done with this function
 *
*/
int websocket_send(int sock_id, void *data, size_t size)
{
    byte *to_send;
    byte opcode;
    unsigned long int dataframe_size;
    unsigned long int is_sent;


    opcode = websocket_create_opcode(1, 0, 0, 0, 1);
    to_send = websocket_create_dataframe(size, (byte *)data, opcode, &dataframe_size);
    if(to_send == NULL)
    {
        return WS_NO_VAL;
    }

    is_sent = websocket_send_dataframe(sock_id, to_send, dataframe_size);
    free(to_send);
    // Doing it with a if and not returning websocket_send in order to do a free
    if(!is_sent){
        return WS_NO_VAL;
    }

    return is_sent;
}
