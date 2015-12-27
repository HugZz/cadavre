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
    ListPlayers index = players;
    char buffer[MAX_BUFFER];
    int n;
    int length = 0;

    /* Browse all players */
    do
    {
        sprintf(buffer, "RD %d %d", round, NB_ROUNDS);
        /* Length of the string + '\0' */
        length = strlen(buffer) + 1;
        n = write(index->player_sock, buffer, length);
        if (n < 0) error("ERROR writing to socket");
        index = index->next_player;
    }
    while (index != players);
}

void play_round(char **lines, int round)
{
    ListPlayers index = players;
    ListPlayers current_player = players;
    char buffer[MAX_BUFFER];
    char info[3];
    int n;
    int length = 0;

    /* Browse all players */
    do
    {
        /* Tell them if it is their turn or not */
        do
        {
            if (index == current_player)
            {
                sprintf(buffer, "GO");
            }
            else
            {
                sprintf(buffer, "WT %d", current_player->player_number);
            }
            /* Length of the string + '\0' */
            length = strlen(buffer) + 1;
            n = write(index->player_sock, buffer, length);
            if (n < 0) error("ERROR writing to socket");
            index = index->next_player;
        }
        while (index != players);

        /* Communicate with current player to play */

        /* Send him previous line which is:
         * line of the last player of the last round for the first player
         * line of the last player for the others
         */

        /* First player, first round */
        if (round == 1 && current_player->player_number == 1)
        {
            sprintf(buffer, "FL");
        }
        else
        {
            if (current_player->player_number == 1)
            {
                sprintf(buffer, "LN %s", lines[round - 2 + nb_players - 1]);
            }
            else
            {
                sprintf(buffer, "LN %s", lines[round - 1 + current_player->player_number - 2]);
            }
        }
        /* Length of the string + '\0' */
        length = strlen(buffer) + 1;
        n = write(index->player_sock, buffer, length);
        if (n < 0) error("ERROR writing to socket");

        /* Get his line and store it */
        bzero(buffer, MAX_BUFFER);
        bzero(info, 3);
        while (strcmp(info, "LN") != 0)
        {
            n = read(index->player_sock, buffer, MAX_BUFFER - 1);
            if (n < 0)
            {
                error("ERROR reading from socket");
            }
            sscanf(buffer, "%s", info);
        }
        sscanf(buffer, "%s %s", info, lines[round - 1 + current_player->player_number - 1]);

        /* Indicate the well reception of data and the end of player's turn */
        index = players;
        do
        {
            sprintf(buffer, "END");
            /* Length of the string + '\0' */
            length = strlen(buffer) + 1;
            n = write(index->player_sock, buffer, length);
            if (n < 0) error("ERROR writing to socket");
            index = index->next_player;
        }
        while (index != players);
    }
    while (current_player != players);
}

void send_lines(char **lines)
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
