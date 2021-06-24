#ifndef MINI_H
#define MINI_H

#define ARG "Error: argument\n"
#define CORR "Error: Operation file corrupted\n"
#define HEAD "%d %d %c\n"
#define OPER "%c %f %f %f %c\n"

#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct s_mini
{
    FILE *fp;
    char *buf;
    int res;
    int width;
    int height;
    char ch;
    char c;
    float x;
    float y;
    float r;
    char ch_c;
} t_mini;

#endif