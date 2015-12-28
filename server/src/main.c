/*
 * File : main.c
 * Description : Main file for the server-side program.
 * Author : Hugues de Valon
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
/* Last player in the list */
ListPlayers players_last = NULL;

int main(int argc, char *argv[])
{
    /* Sockets for the server and new connections. */
    int sockfd, newsockfd; 
    int portno;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char c;
    int round = 1;
    /* Return value for write and read */
    int n;
    /* Array of strings to store the lines */
    char **lines = NULL;
    int i = 0;
    /* index to close all sockets */
    ListPlayers index = NULL;
    ListPlayers free_index = NULL;

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s PORT_NUMBER\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("\tCADAVRE SERVER\n");

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

        printf("Accept player from %s ? (y/N) ", inet_ntoa(cli_addr.sin_addr));
        c = getchar();
        /* Remove `\n` */
        getchar();
        if (c == 'y' || c == 'Y')
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
            printf("Joueur refusé.\n");
            n = write(newsockfd, "NO", 3);
            if (n < 0) error("ERROR writing to socket");
            close(newsockfd);
        }

        /* If the game is playable */
        if (nb_players > 2)
        {
            printf("Number of players : %d\nStart the game ? (y/n) ", nb_players);
            c = getchar();
            getchar();
            if (c == 'y') break;
        }
    }

    close(sockfd);

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

    while(round <= NB_ROUNDS)
    {
        printf("ROUND %d\n", round);
        /* Check if everybody is here and announce the round */
        check_round(round);
        /* Play the round */
        play_round(lines, round);
        round++;
    }
    printf("End of game.\n");

    printf("\t\nCADAVRE:\n");

    /* Announce end of game and send the cadavre, exit */
    send_lines(lines);

    /* Print the lines on server's terminal */
    for(i = 0; i < NB_ROUNDS * nb_players; i++)
    {
        printf("%s", lines[i]);
    }

    /* Close all sockets */
    index = players;
    do
    {
        close(index->player_sock);
        index = index->next_player;
    }
    while (index != players);

    index = players;
    free_index = players;
    /* Free allocated structures */
    while (free_index != players_last)
    {
        free_index = index->next_player;
        free(index);
        index = free_index;
    }
    free(index);

    free(*lines);
    free(lines);

    return EXIT_SUCCESS;
}
