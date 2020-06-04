[Main menu](../../Readme.md)->[Doc](../websocket-doc.md)

# websocket_get_client_handshake(\_)
Reading the handshake header received from the client at connection.

## **Declaration**
```C
char *websocket_get_client_handshake(
		int client_id
	)
```


## **Parameter**
**client_id**\
The client socket id.

## **Result**
Returns the handshake header, `NULL` if failed.
