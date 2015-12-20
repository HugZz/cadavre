/* 
 * players.c
 * Author : Hugues de Valon
 * Implements players linked list and usefull functions
 * to manage it.
 */

#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "../headers/players.h"

/* Add a player to the player list */
void add_player(struct sockaddr_in player_addr, int player_sock)
{
    ListPlayers index = players;
    // Number of the player.
    int number = 0;

    // If there are no players yet.
    if (players == NULL)
    {
        players = (ListPlayers) malloc(sizeof(*players));
        players->player_addr = player_addr;
        players->player_sock = player_sock;
        players->player_number = number;
        players->next_player = NULL;
    }
    else
    {
        while (index->next_player != NULL)
        {
            index = index->next_player;
            number++;
        }
        number++;
        index->next_player = (ListPlayers) malloc(sizeof(*players));
        index = index->next_player;
        index->player_addr = player_addr;
        index->player_sock = player_sock;
        index->player_number = number;
        index->next_player = NULL;
    }
}
