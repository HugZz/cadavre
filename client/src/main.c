/* 
 * main.c
 * Author : Hugues de Valon
 * Main file for the player-side program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#include "../headers/error.h"

int main(int argc, char *argv[])
{
    int sockfd; // Socket for remote connection
    int portno; // Port to connect with the server.
    int n; // Value of bytes written/read
    struct sockaddr_in serv_addr; // Address of the server.
    struct hostent *server; // Domain name of the server.

    /* Check arguments */
    if (argc < 3) 
    {
       fprintf(stderr,"usage: %s HOSTNAME PORT\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    /* Create socket for the connection */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    /* Get address of host with his domain name */
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        error("ERROR, no such host");
    }









    return EXIT_SUCCESS;
}
