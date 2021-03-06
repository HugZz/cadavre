/*
 * File : game.h
 * Author : Hugues de Valon
 * Descriptions : Function and constants to manage the gaming side.
 */

#ifndef _GAME_
#define _GAME_

#define MAX_BUFFER 256

void play_round(int sockfd);
void print_lines(int sockfd, int nb_players, int max_rounds);

#endif
