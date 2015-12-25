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
#include "../headers/players.h"
#include "../headers/game.h"

void check_round(int round)
{
    NULL;
}

void play_round(char **lines, int round)
{
    NULL;
}

void print_lines(char **lines)
{
    NULL;
}

void check_game(void)
{
    ListPlayers index = players;
    char buffer[MAX_BUFFER];
    int n;
    int length = 0;

    /* Browse all players */
    do
    {
        sprintf(buffer, "OK %d %d", index->player_number, nb_players);
        /* Length of the string + '\0' */
        length = strlen(buffer) + 1;
        n = write(index->player_sock, buffer, length);
        if (n < 0) error("ERROR writing to socket");
        index = index->next_player;
    }
    while (index != players);
}
