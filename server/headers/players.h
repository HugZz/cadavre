/* 
 * players.h
 * Author : Hugues de Valon
 * Constants and prototypes for players
 */

/* Structure of a player in our list */
struct Player
{
    // His address
    struct sockaddr_in player_addr;
    // The socket for that connection
    int player_sock;
    struct Player *next_player;
};
typedef struct Player Player;
typedef Player* ListPlayers;

extern ListPlayers players;
extern int nb_players;

void add_player(struct sockaddr_in player_addr, int player_sock);
