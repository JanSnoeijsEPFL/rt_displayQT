#ifndef SERVER_H
#define SERVER_H

#endif // SERVER_H

#define PORT_NUMBER     5000
#define SERVER_ADDRESS  "169.254.37.95"

void setup_server();
char* send_receive_TCP(int sequence);
void close_sockets();
