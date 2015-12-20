/*
 * game.h
 * Author : Hugues de Valon
 * Function and constants to manage the gaming side.
 */

#define NB_ROUNDS 3
#define MAX_BUFFER 256

void check_round(int round);
void play_round(char **lines, int round);
void print_lines(char **lines);
