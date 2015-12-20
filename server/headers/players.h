/* 
 * players.h
 * Author : Hugues de Valon
 * Constants and prototypes for players
 */

extern int nb_players;

void add_player(struct sockaddr_in player_addr, int player_sock);
