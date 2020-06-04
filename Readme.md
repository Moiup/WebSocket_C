# WebSocket for C

Websocket implementation for a C server.

- [documentation](Doc/websocket-doc.md)

## Dependecies

- bitByte
- easySocket
- stringDyn
- openssl

When compiling to create the executable file, you must add these flags depending on you OS:

**Debian/Ubuntu**
```
-lcrypto
```

**MacOS**
```
/usr/local/opt/openssl/include -L /usr/local/opt/openssl/lib -lcrypto
```

## Makefile

To generate the `websocket.o` file, this is how you have to proceed:

```bash
CC=gcc
FLAGC=${FLAG} -c

WebSocket.o: bitByte.o stringDyn.o WebSocket/WebSocket.h WebSocket/WebSocket.c
	$(CC) $(FLAGC) WebSocket/WebSocket.c -o WebSocket.o

stringDyn.o: stringDyn/stringDyn.h stringDyn/stringDyn.c
	$(CC) ${FLAGC} stringDyn/stringDyn.c -o stringDyn.o

easySocket.o: easySocket/easySocket.h easySocket/easySocket.c
	$(CC) ${FLAGC} easySocket/easySocket.c -o easySocket.o

bitByte.o: bitByte/bitByte.h bitByte/bitByte.c
	$(CC) ${FLAGC} bitByte/bitByte.c -o bitByte.o
```
