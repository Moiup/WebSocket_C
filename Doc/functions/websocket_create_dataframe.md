[Main menu](../../Readme.md)->[Doc](../websocket-doc.md)

# websocket_create_dataframe(\_,\_,\_)
Sets the 8 first bits of a dataframe.

## **Declaration**
```C
byte *websocket_create_dataframe(
		size_t data_len,
		byte *data,
		byte opcode,
		size_t *dataframe_size
	)
```

## **Parameter**
**data_len**\
The client socket id.

**data**\
The data to send.

**opcode**\
A valid opcode byte generated with [`websocket_create_dataframe(...)`](websocket_create_dataframe.md).

**dataframe_size**\
Pointer which will get the value of the size of the generated dataframe.

## **Result**
Return a valid dataframe, `NULL` if failed.
