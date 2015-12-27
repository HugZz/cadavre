/*
 * game.c
 * Author : Hugues de Valon
 * Function and constants to manage the gaming side.
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
    while (strcmp(info, "FL") != 0 && strcmp(info, "LN") != 0)
    {
        n = read(sockfd, buffer, MAX_BUFFER - 1);
        if (n < 0)
        {
            error("ERROR reading from socket");
        }
        sscanf(buffer, "%s", info);
    }
    if (strcmp(info, "FL") != 0)
    {
        printf("First line of the game.\n");
    }
    else
    {
        sscanf(buffer, "%s %s", info, previous);
        printf("previous line: %s", previous);
    }

    /* Send the new line */
    printf("write: ");
    scanf("%s", new);
    sprintf(buffer, "LN %s", new);
    /* Length of the string + '\0' */
    length = strlen(buffer) + 1;
    n = write(sockfd, buffer, length);
    if (n < 0) error("ERROR writing to socket");
}

void print_lines(int sockfd)
{
    printf("Lines printing !\n");
}
