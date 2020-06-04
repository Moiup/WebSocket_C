[Main menu](../../Readme.md)->[Doc](../websocket-doc.md)

# websocket_create_opcode(\_,\_,\_,\_,\_)
Sets the 8 first bits of a dataframe.

## **Declaration**
```C
byte websocket_create_opcode(
		int fin,
		int rsv1,
		int rsv2,
		int rsv3,
		int opcode
	)
```

## **Parameter**
**fin**\
fin bit.

**rsv1**\
rsv1 bit.

**rsv2**\
rsv2 bit.

**rsv3**\
rsv3 bit.

**opcode**\
4 bits of opcode.

## **Result**
Returns a byte set with the corresponding values.
