#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int fd;
    int fd2;
    char b1;
    char b2;
    fd = open(argv[1], O_RDWR);
    fd2 = open(argv[2], O_RDWR);
    int e;
    int e2;
    while (1)
    {
        e = read(fd2, &b2, 1);
        e2 = read(fd, &b1, 1);
        if (b1 != b2)
        {
            write(1, "err\n", 4);
            break;
        }
        if (e == 0 && e2 == 0)
        {
            write(1, "1\n", 2);
            break;
        }
        
       
    }
    close(fd);
    close(fd2);
}
