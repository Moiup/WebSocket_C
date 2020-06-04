[Main menu](../../Readme.md)->[Doc](../websocket-doc.md)

# websocket_handshake_header_response(\_)
Create the server handshake header response.

## **Declaration**
```C
char *websocket_handshake_header_response(
		char *request
	)
```

## **Parameter**
**request**\
The handshake header request read with [`websocket_get_client_handshake(client_id)`](websocket_get_client_handshake.md).


## **Result**
Returns the created server handshake header, `NULL` if failed.
