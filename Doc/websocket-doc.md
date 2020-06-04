[Main menu](../Readme.md)

# WebSocket

```C
char *websocket_get_client_handshake(int)
```
Reads the client handshake demand. [More here.](./functions/websocket_get_client_handshake.md)

```C
char *websocket_handshake_header_response(char *);
```
Create a response header for handshake. [More here.](./functions/websocket_handshake_header_response.md)

```C
byte *webSocket_read_msg(int, size_t *)
```
Read a received message. [More here.](./functions/webSocket_read_msg.md)

```C
byte websocket_create_opcode(int, int, int, int, int)
```
Create the first 8 bytes of a dataframe. [More here.](./functions/websocket_create_opcode.md)

```C
byte *websocket_create_dataframe(size_t, byte *, byte, size_t *)
```
Create a dataframe [More here.](./functions/websocket_create_dataframe.md)

```C
int websocket_send_dataframe(int, void *, size_t)
```
Send a dataframe. [More here.](./functions/websocket_send_dataframe.md)

```C
int websocket_send(int, void *, size_t)
```
Send data using websocket (all in one: create the opcode, the dataframe and send it). [More here.](./functions/websocket_send.md)
