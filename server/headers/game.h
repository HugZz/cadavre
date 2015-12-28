/*
 * game.h
 * Author : Hugues de Valon
 * Function and constants to manage the gaming side.
 */

#ifndef _GAME_
#define _GAME_

#define NB_ROUNDS 1
#define MAX_BUFFER 256

void check_round(int round);
void play_round(char **lines, int round);
void send_lines(char **lines);
void check_game(void);

#endif
