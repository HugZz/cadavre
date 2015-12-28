/*
 * File : error.c
 * Description : Basic functions to handle errors.
 * Author : Hugues de Valon
 */

#include <stdlib.h>
#include <stdio.h>

/* Print a perror message */
void error(char *msg)
{
    perror(msg);
    exit(1);
}
