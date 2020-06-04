[Main menu](../../Readme.md)->[Doc](../websocket-doc.md)

# websocket_send(\_,\_,\_)
Sets the 8 first bits of a dataframe.

## **Declaration**
```C
int websocket_send(
		int sock_id,
		void *data,
		size_t size
	)
```

## **Parameter**
**sock_id**\
The client socket id.

**data**\
The data to send.

**data_size**\
The size of the data.

## **Result**
The number of bytes sent, `WS_NO_VAL` if failed.
