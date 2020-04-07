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
        // fprintf(stdout, "\nData len 16: %d\n", data_len_16);
        if(is_read){
            return (unsigned long int)(data_len_16[0] * WS_2_POW_8 + data_len_16[1]);
        }
        return WS_FALSE;
    }
    if(playload == WS_PLAYLOAD_64){
        is_read = easySocket_read(client_id, data_len_64, WS_PLAYLOAD_64_SIZE);
        // fprintf(stdout, "\nData len 16: %d\n", data_len_16);
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


byte *websocket_read_data(int client_id, unsigned long int data_len, byte *masking_key){
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
        return NULL;
    }

    for(i = 0; i < data_len; i++){
        j = i % MASKING_KEY_SIZE;
        // fprintf(stdout, "\n");
        // bitByte_display(&data[i]);
        // fprintf(stdout, "\n");
        // bitByte_display(&masking_key[i]);
        // fprintf(stdout, "\n");

        data[i] = data[i] ^ masking_key[j];


        // bitByte_display(&data[i]);

        // fprintf(stdout, "\n");
    }


    return data;
}

/**
 * Read a websocket message
 * Send NULL if error ()
 */
byte *webSocket_read_msg(int client_id, unsigned long int *data_len){
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

    fprintf(stdout, "%ld\n", *data_len);
    

    return websocket_read_data(client_id, *data_len, masking_key);
}

/**
 * Generate the 8 first bit with the given opcode
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
 */
byte *websocket_create_dataframe(unsigned long int data_len, byte *data, byte opcode, unsigned long int *dataframe_size){
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
        fprintf(stdout, "\ndata_len : %ld\n", data_len);
        fprintf(stdout, "dataframe_size : %ld\n", *dataframe_size);
        df = (byte *)malloc(*dataframe_size);
        if(df == NULL){
            fprintf(stderr, "(websocket_create_dataframe) Error allocating df\n");
            exit(EXIT_FAILURE);
        }
        bitByte_display(&playload_len);
        fprintf(stdout, "\n");
    
        fprintf(stdout, "TEST 1\n");
        for(i = WS_DF_BASE_SIZE_BYTES, j = 0; i < *dataframe_size; i++,j++){
            df[i] = data[j];
            fprintf(stdout, "TEST i: %c\n", (char)df[i]);
        }
        fprintf(stdout, "TEST 2\n");
    }


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
 * Send a dataframe
 * Return 0 if nothing sent
 */
int websocket_send_dataframe(int sock_id, byte *dataframe, unsigned long int dataframe_size){
    return send(sock_id, (byte *)dataframe, dataframe_size, 0);
}
