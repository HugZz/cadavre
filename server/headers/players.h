/* 
 * File : players.h
 * Description : Constants and prototypes for players
 * Author : Hugues de Valon
 */

#ifndef _PLAYERS_
#define _PLAYERS_

/* Structure of a player in our list */
struct Player
{
    /* His address */
    struct sockaddr_in player_addr;
    /* The socket for that connection */
    int player_sock;
    /* Number of the player */
    int player_number;
    /* Link to the next player */
    struct Player *next_player;
};
typedef struct Player Player;
typedef Player* ListPlayers;

/* Link to the first player */
extern ListPlayers players;
/* Link to the last player */
extern ListPlayers players_last;
/* Number of player in the game */
extern int nb_players;

/* Add a player to the linked list */
void add_player(struct sockaddr_in player_addr, int player_sock);

#endif
