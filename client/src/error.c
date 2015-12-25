/*
 * error.c
 * Author : Hugues de Valon
 * Error functions.
 */

#include <stdlib.h>
#include <stdio.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}
