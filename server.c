#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include "server.h"

char message[60];
int server_socket;
int peer_socket;

void setup_server(){

    socklen_t       sock_len;
    struct sockaddr_in      server_addr;
    struct sockaddr_in      peer_addr;

    memset(message, '0', sizeof(message)); //initialize message memory to 0
    /* Create server socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
            fprintf(stderr, "Error creating socket --> %s", strerror(errno));

            exit(EXIT_FAILURE);
    }

    /* Zeroing server_addr struct */
    memset(&server_addr, 0, sizeof(server_addr));
    /* Construct server_addr struct */
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDRESS, &(server_addr.sin_addr));
    server_addr.sin_port = htons(PORT_NUMBER);

    /* Bind */
    if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
    {
            fprintf(stderr, "Error on bind --> %s", strerror(errno));

            exit(EXIT_FAILURE);
    }

    /* Listening to incoming connections */
    if ((listen(server_socket, 5)) == -1)
    {
            fprintf(stderr, "Error on listen --> %s", strerror(errno));

            exit(EXIT_FAILURE);
    }

    sock_len = sizeof(struct sockaddr_in);
    /* Accepting incoming peers */
    peer_socket = accept(server_socket, (struct sockaddr *)&peer_addr, &sock_len);
    if (peer_socket == -1)
    {
            fprintf(stderr, "Error on accept --> %s", strerror(errno));

            exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(peer_addr.sin_addr));
}

char* send_receive_TCP(int sequence){
    char file_size[256];
    struct stat file_stat;
    off_t offset;
    int remain_data;
    char buffer[BUFSIZ];
    int fd;
    int n;
    char filename[128];
    int sent_bytes = 0;
    char seqstr[4];
    ssize_t len;

    snprintf(seqstr, sizeof(seqstr), "%d", sequence); //cast sequence number to string
    snprintf(filename,sizeof(filename), "/home/lsmpc1-linux/deliverables/De10_standard_files/NNESD/chb01_quantized_seq/RT_NormQuantdata_seq%s", seqstr);
    printf("filename: %s\n", filename); //check file name
    fd = open(filename,O_RDONLY); // open file
    if (fd == -1)
    {
            fprintf(stderr, "Error opening file --> %s", strerror(errno));

            exit(EXIT_FAILURE);
    }

    /* Get file stats */
    if (fstat(fd, &file_stat) < 0)
    {
            fprintf(stderr, "Error fstat --> %s", strerror(errno));

            exit(EXIT_FAILURE);
    }

    fprintf(stdout, "File Size: \n%d bytes\n", file_stat.st_size);
    sprintf(file_size, "%d", file_stat.st_size); // copy file size

    /* Sending file size */
    len = send(peer_socket, file_size, sizeof(file_size), 0); //send file size
    if (len < 0)
    {
          fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));

          exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Server sent %d bytes for the size\n", len);

    offset = 0;
    remain_data = file_stat.st_size;
    /* Sending file data */

        while(((sent_bytes = sendfile(peer_socket, fd,&offset, BUFSIZ))>0)&&(remain_data > 0)) //while remaining data
        {
          remain_data -= sent_bytes; //substract sent data from remaining elements
          fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
    }
    recv(peer_socket, message, sizeof(message),0); //receive message from client
    printf("received message : %s\n", message); //print message

    close(fd);
    return message;
}

void close_sockets()
{
    close(peer_socket); //close sockets
    close(server_socket);
}
