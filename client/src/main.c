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
#include <netdb.h>

#include "../headers/error.h"
#include "../headers/game.h"

int main(int argc, char *argv[])
{
    /* Loop index */
    int i = 0;
    int sockfd; // Socket for remote connection
    int portno; // Port to connect with the server.
    int n; // Value of bytes written/read
    struct sockaddr_in serv_addr; // Address of the server.
    struct hostent *server; // Domain name of the server.
    char buffer[MAX_BUFFER];
    /* 2 characters information string */
    char info[3];
    /* Number of players */
    int nb_players = 0;
    /* Number of this player */
    int number = 0;
    int round = 0;
    int max_rounds = 0;
    /* Number of the current player */
    int player = 0;

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

    /* Fill the fields of server address */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Connect to the server */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        error("ERROR connecting");
    }
    /* Server answer */
    bzero(buffer, MAX_BUFFER);
    printf("Connecting to the server ...\n");
    n = read(sockfd, buffer, MAX_BUFFER - 1);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    if (strcmp(buffer, "YES") == 0)
    {
        printf("You have been accepted, waiting for other players.\n");
    }
    if (strcmp(buffer, "NO") == 0)
    {
        printf("You have been kicked by the server.\n");
        return EXIT_FAILURE;
    }

    /* Check game starting */
    bzero(buffer, MAX_BUFFER);
    bzero(info, 3);
    n = read(sockfd, buffer, MAX_BUFFER - 1);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    sscanf(buffer, "%s %d %d", info, &number, &nb_players);

    printf("Game starting: you are player %d/%d.\n", number, nb_players);

    /* Game loop */
    while (1)
    {
        /* Check beginning of round */
        bzero(buffer, MAX_BUFFER);
        bzero(info, 3);
        n = write(sockfd, "RD", 3);
        if (n < 0) error("ERROR writing to socket");
        n = read(sockfd, buffer, MAX_BUFFER - 1);
        if (n < 0)
        {
            error("ERROR reading from socket");
        }
        sscanf(buffer, "%s %d %d", info, &round, &max_rounds);
        printf("ROUND %d/%d:\n", round, max_rounds);

        /* For each player */
        for (i = 0; i < nb_players; i++)
        {
            /* Check either it is this client turn or any other */
            n = write(sockfd, "PL", 3);
            if (n < 0) error("ERROR writing to socket");
            bzero(buffer, MAX_BUFFER);
            bzero(info, 3);
            n = read(sockfd, buffer, MAX_BUFFER - 1);
            sscanf(buffer, "%s %d", info, &player);
            if (n < 0)
            {
                error("ERROR reading from socket");
            }
            if (strcmp(info, "WT") == 0)
            {
                printf("Player %d is playing ...\n", player);
            }
            else if (strcmp(info, "GO") == 0)
            {
                /* Play this round: send line to server */
                printf("It's your turn to play.\n");
                play_round(sockfd);
            }

            /* End of player's turn */
            n = write(sockfd, "END", 4);
            if (n < 0) 
            {
                error("ERROR writing to socket");
            }
            bzero(buffer, MAX_BUFFER);
            n = read(sockfd, buffer, MAX_BUFFER - 1);
            if (n < 0)
            {
                error("ERROR reading from socket");
            }
        }

        /* Break the game loop if it is the last round */
        if (round == max_rounds)
        {
            break;
        }
    }

    /* Print the final lines */
    print_lines(sockfd);

    printf("End of game. Goodbye !\nMade by Hugues de Valon\n");

    return EXIT_SUCCESS;
}
