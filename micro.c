#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ARG "Error: argument\n"
#define CORR "Error: Operation file corrupted\n"
#define HEAD "%d %d %c\n"
#define OPER "%c %f %f %f %f %c\n"

typedef struct s_mic
{
    FILE *fp;
    int res;
    char *buf;
    int width;
    int height;
    char ch;
    char r;
    float x;
    float y;
    float w;
    float h;
    char ch_r;
} t_mic;

void ft_putstr(char *s)
{
    int i = -1;

    while (s[++i] != 0)
        write(1, &s[i], 1);
}

int clear_all(char *str, t_mic *m)
{
    if (str)
        ft_putstr(str);
    if (m->buf)
        free(m->buf);
    if (m->fp)
        fclose(m->fp);
    return (1);
}

int open_file(char *file, t_mic *m)
{
    m->fp = fopen(file, "r");
    if (!m->fp)
        return (1);
    return (0);
}

int pars_back(t_mic *m)
{
    m->res = fscanf(m->fp, HEAD, &m->width, &m->height, &m->ch);
    if (m->res != 3 || m->width <= 0 || m->height > 300 || m->height <= 0 || m->width > 300)
        return (1);
    m->buf = malloc(m->width * m->height);
    if (!m->buf)
        return (1);
    return (0);
}

void fill_back(t_mic *m)
{
    int i = -1;
    int j;

    while (++i < m->height)
    {
        j = -1;
        while (++j < m->width)
            m->buf[m->width * i + j] = m->ch;
    }
}

void draw_all(t_mic *m)
{
    int i = -1;
    int j;

    while (++i < m->height)
    {
        j = -1;
        while (++j < m->width)
            write(1, &m->buf[m->width * i + j], 1);
        write(1, "\n", 1);
    }
}

int pars_rec(t_mic *m)
{
    m->res = fscanf(m->fp, OPER, &m->r, &m->x, &m->y, &m->w, &m->h, &m->ch_r);
    if (m->res != 6)
        return (1);
    if (m->r != 'r' && m->r != 'R')
        return (1);
    if (m->w <= 0.00000000 || m->h <= 0.00000000)
        return (1);
    return (m->res);
}

int in_rec(t_mic *m, float y, float x)
{
    if ((y - m->y) < 0 || (m->y + m->h) - y < 0 || (x - m->x) < 0 || (m->x + m->w) - x < 0)
        return (0);
    else if ((y - m->y) < 1.00000000 || (x - m->x) < 1.00000000 || (m->y + m->h) - y < 1.00000000 || \
    (m->x + m->w) - x < 1.00000000)
        return (2);
    else
        return (1);
}

void draw_rec(t_mic *m)
{
    int i = -1;
    int j;
    int res;

    while (++i < m->height)
    {
        j = -1;
        while (++j < m->width)
        {
            res = in_rec(m, i, j);
            if ((res > 0 && m->r == 'R') || (res == 2 && m->r == 'r'))
                m->buf[m->width * i + j] = m->ch_r;
        }
    }
}

int fill_rec(t_mic *m)
{
    int res;

    res = pars_rec(m);
    if (m->res == -1)
        return (0);
    if (res != 6)
        return (1);
    while (res == 6)
    {
        draw_rec(m);
        res = pars_rec(m);
    }
    if (m->res == -1)
        return (0);
    return (1);
}

int main(int argc, char **argv)
{
    t_mic m;

    m.buf = NULL;
    m.fp = NULL;
    if (argc != 2)
        return (clear_all(ARG, &m));
    if (open_file(argv[1], &m))
        return (clear_all(CORR, &m));
    if (pars_back(&m))
        return (clear_all(CORR, &m));
    fill_back(&m);
    if (fill_rec(&m))
        return (clear_all(CORR, &m));
    draw_all(&m);
    clear_all(NULL, &m);
    return (0);
}
