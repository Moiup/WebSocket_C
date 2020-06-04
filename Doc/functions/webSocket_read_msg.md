[Main menu](../../Readme.md)->[Doc](../websocket-doc.md)

# webSocket_read_msg(\_,\_)
Create the server handshake header response.

## **Declaration**
```C
byte *webSocket_read_msg(
		int client_id,
		size_t *data_len
	)
```

## **Parameter**
**client_id**\
The client socket id.

**data_len**\
An int address that will store the length of the read data.

## **Result**
Returns the read dataframe, `NULL` if failed.
