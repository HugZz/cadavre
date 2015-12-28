/*
 * File : error.c
 * Description : Error function.
 * Author : Hugues de Valon
 */

#include <stdlib.h>
#include <stdio.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}
