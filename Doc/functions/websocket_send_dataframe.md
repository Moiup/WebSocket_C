[Main menu](../../Readme.md)->[Doc](../websocket-doc.md)

# websocket_send_dataframe(\_,\_,\_)
Sets the 8 first bits of a dataframe.

## **Declaration**
```C
int websocket_send_dataframe(
		int sock_id,
		void *data,
		size_t data_size
	)
```

## **Parameter**
**sock_id**\
The client socket id.

**data**\
The dataframe to send created with [`websocket_create_dataframe(...)`](websocket_create_dataframe.md

**data_size**\
The size of the dataframe to send.


## **Result**
Return the number of bytes send, 0 if nothing sent.
