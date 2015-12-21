/*
 * main.c
 * Author : Hugues de Valon
 * Main file for the server-side program.
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
#include "../headers/players.h"
#include "../headers/game.h"

/* Extern global variable to count player number */
int nb_players = 0;
/* The main player list */
ListPlayers players = NULL;

int main(int argc, char *argv[])
{
    int sockfd, newsockfd; // Sockets for the server and new connections.
    int portno;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char c;
    int round = 0;
    /* Return value for write and read */
    int n;
    /* Array of strings to store the lines */
    char **lines = NULL;
    int i = 0;

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s PORT_NUMBER\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Create new file descriptor */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    /* Fill serv_addr fields */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    /* Convert port to network format */
    serv_addr.sin_port = htons(portno);

    /* Bind the socket to the server address */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    
    /* Use the socket to listen incomming connections */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* First loop to get all clients sockets */
    while (1)
    {
        /* Block the process until a new connection appears */
        printf("Looking for players ...\n");
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
        {
            error("ERROR on accept");
        }

        printf("Accept player from %s ? (y/n) ", inet_ntoa(cli_addr.sin_addr));
        c = getchar();
        if (c == 'y')
        {
            /* We can accept the new player, so we add him
             * to the player linked list, letting him know.
             */
            nb_players++;
            n = write(newsockfd, "YES", 4);
            if (n < 0) error("ERROR writing to socket");
            add_player(cli_addr, newsockfd);
        }
        else
        {
            /* Refuse player */
            n = write(newsockfd, "NO", 3);
            if (n < 0) error("ERROR writing to socket");
            close(newsockfd);
        }

        /* If the game is playable */
        if (nb_players > 2)
        {
            printf("Number of players : %d\nStart the game ? (y/n)", nb_players);
            c = getchar();
            if (c == 'y') break;
        }
    }

    /* 2D array of strings with NB_ROUNDS * nb_players lines
     * and MAX_BUFFER columns, allocated dynamicly.
     */
    lines = (char **) calloc(NB_ROUNDS * nb_players, sizeof(*lines));
    *lines = (char *) calloc(NB_ROUNDS * nb_players * MAX_BUFFER, sizeof(**lines));
    for(i = 1; i < NB_ROUNDS * nb_players; i++)
    {
        lines[i] = lines[i - 1] + MAX_BUFFER;
    }

    /* Game part, everybody is connected */

    /* Announce the beginning of the game */
    check_game();

    while(round < NB_ROUNDS)
    {
        /* Check if everybody is here and announce the round */
        check_round(round);
        /* Play the round */
        play_round(lines, round);
        round++;
    }

    /* Announce end of game and send the cadavre, exit */
    print_lines(lines);

    /*Print the lines on server's terminal */
    for(i = 0; i < NB_ROUNDS * nb_players; i++)
    {
        printf("%s", lines[i]);
    }

    return EXIT_SUCCESS;
}
