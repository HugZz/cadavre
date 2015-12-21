/*
 * error.c
 * Author : Hugues de Valon
 * Error functions.
 */

void error(char *msg)
{
    perror(msg);
    exit(0);
}
