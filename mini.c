#include "mini.h"

void ft_putstr(char *str)
{
    int i = 0;

    while(str[i] != 0)
    {
        write(1, &str[i], 1);
        i++;
    }
}

int err(char *str, t_mini *mini)
{
    ft_putstr(str);
    if (mini->buf)
        free(mini->buf);
    if (mini->fp)
        fclose(mini->fp);
    return(1);
}

int open_file(char *file, t_mini *mini)
{
    mini->fp = fopen(file, "r");
    if (mini->fp == NULL)
        return(1);
    return(0);
}

int pars_back(t_mini *mini)
{
    mini->res = fscanf(mini->fp, HEAD, &mini->width, &mini->height, &mini->ch);
    if (mini->res != 3 || mini->width <= 0 || mini->width > 300 || \
    mini->height <= 0 || mini->height > 300)
        return(1);
    return(0);
}

int alloc_buf(t_mini *mini)
{
    mini->buf = malloc(sizeof(char) * mini->width * mini->height);
    if (!mini->buf)
        return (1);
    return(0);
}

void fill_back(t_mini *mini)
{
    int i;
    int j;

    i = 0;
    while(i < mini->height)
    {
        j = 0;
        while (j < mini->width)
        {
            mini->buf[mini->width * i + j] = mini->ch;
            j++;
        }
        i++;
    }
}

int scan_oper(t_mini *mini)
{
    mini->res = fscanf(mini->fp, OPER, &mini->c, &mini->x, &mini->y, &mini->r, &mini->ch_c);
    if (mini->c != 'c' && mini->c != 'C')
        return (1);
    if (mini->r <= 0.0) // вот тут раньше было <= 0
        return(1);
    return(mini->res);
}

int in_circus(t_mini *mini, float y, float x)
{
    float dist;

    dist = sqrtf((y - mini->y) * (y - mini->y) + (x - mini->x) * (x - mini->x));
    if (dist > mini->r)
        return (0);
    else if ((dist - mini->r) * (dist - mini->r) < 1.00000000)
        return(2);
    else
        return (1);
}

void draw_circus(t_mini *mini)
{
    int i;
    int j;
    int res;

    i = 0;
    while(i < mini->height)
    {
        j = 0;
        while (j < mini->width)
        {
            res = in_circus(mini, i, j);
            if ((mini->c == 'c' && res == 2) || (mini->c == 'C' && res > 0))
                mini->buf[mini->width * i + j] = mini->ch_c;
            j++;
        }
        i++;
    }
}

int fill_circus(t_mini *mini)
{
    int res;

    while(1)
    {
        res = scan_oper(mini);
    printf("%d\n", res);
        if (res == 5)
            draw_circus(mini);
        else if (res == EOF)
            return (0);
        else
            return (1);
    }
    return (0);
}

void draw_all(t_mini *mini)
{
    int i;
    int j;

    i = 0;
    while(i < mini->height)
    {
        j = 0;
        while (j < mini->width)
        {
            write(1, &mini->buf[mini->width * i + j], 1);
            j++;
        }
        write(1, "\n", 1);
        i++;
    }
}

int main(int argc, char **argv)
{
    t_mini mini;

    mini.fp = NULL;
    mini.buf = NULL; // этой строки не было
    if (argc != 2)
        return (err(ARG, &mini));
    if (open_file(argv[1], &mini))
        return (err(CORR, &mini));
    if (pars_back(&mini))
        return (err(CORR, &mini));
    if (alloc_buf(&mini))
        return (err(CORR, &mini));
    fill_back(&mini);
    if (fill_circus(&mini))
        return (err(CORR, &mini));
    draw_all(&mini);
    fclose(mini.fp);
    free(mini.buf);
    while(1);
}