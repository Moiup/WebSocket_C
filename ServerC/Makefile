CC=gcc
FLAG=-W -Wall
FLAGC=${FLAG} -c

server: useful.o bitByte.o easySocket.o WebSocket.o stringDyn.o server.c
	$(CC) $(FLAG) useful.o bitByte.o easySocket.o WebSocket.o stringDyn.o server.c -lcrypto -o server 

stringDyn.o: stringDyn/stringDyn.h stringDyn/stringDyn.c
	$(CC) ${FLAGC} stringDyn/stringDyn.c -o stringDyn.o

WebSocket.o: bitByte.o WebSocket/WebSocket.h WebSocket/WebSocket.c
	$(CC) $(FLAGC) WebSocket/WebSocket.c -o WebSocket.o

easySocket.o: easySocket/easySocket.h easySocket/easySocket.c
	$(CC) ${FLAGC} easySocket/easySocket.c -o easySocket.o

bitByte.o: bitByte/bitByte.h bitByte/bitByte.c
	$(CC) ${FLAGC} bitByte/bitByte.c -o bitByte.o

useful.o: useful.h useful.c
	${CC} ${FLAGC} -c useful.c -o useful.o

clean:
	rm -f *.o server
