/*
 * error.c
 * Author : Hugues de Valon
 * Basic functions to handle errors.
 */

#include <stdlib.h>
#include <stdio.h>

/* Print a perror message */
void error(char *msg)
{
    perror(msg);
    exit(1);
}
