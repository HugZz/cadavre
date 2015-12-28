/*
 * File : game.c
 * Description : Function and constants to manage the gaming side.
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
#include "../headers/game.h"

void play_round(int sockfd)
{
    char buffer[MAX_BUFFER];
    char previous[MAX_BUFFER];
    char new[MAX_BUFFER];
    char info[3];
    int length = 0;
    int n = 0;

    /* Get last line */
    bzero(new, MAX_BUFFER);
    bzero(previous, MAX_BUFFER);
    bzero(buffer, MAX_BUFFER);
    bzero(info, 3);
    n = read(sockfd, buffer, MAX_BUFFER - 1);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    sscanf(buffer, "%s", info);
    if (strcmp(info, "FL") == 0)
    {
        printf("First line of the game.\n");
    }
    else
    {
        strcpy(previous, buffer);
        printf("previous line: %s", previous);
    }

    /* Send the new line */
    printf("write: ");
    fgets(new, MAX_BUFFER - 3, stdin);
    bzero(buffer, MAX_BUFFER);
    strcpy(buffer, new);
    /* Length of the string + '\0' */
    length = strlen(buffer) + 1;
    n = write(sockfd, buffer, length);
    if (n < 0) 
    {
        error("ERROR writing to socket");
    }
}

void print_lines(int sockfd, int nb_players, int max_rounds)
{
    char buffer[MAX_BUFFER];
    int n = 0;
    int i = 0;
    int length = 0;

    for(i = 0; i < max_rounds * nb_players; i++)
    {
        bzero(buffer, MAX_BUFFER);
        sprintf(buffer, "GIV %2d", i);
        length = strlen(buffer) + 1;
        n = write(sockfd, buffer, length);
        if (n < 0) error("ERROR writing to socket");
        bzero(buffer, MAX_BUFFER);
        n = read(sockfd, buffer, MAX_BUFFER);
        if (n < 0)
        {
            error("ERROR reading from socket");
        }
        printf("%s\n", buffer);
    }
}
