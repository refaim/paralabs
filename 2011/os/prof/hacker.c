#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 

char buffer[BUFSIZ]; /* BUFSIZ is set up in stdio.h */

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Usage: %s filename", argv[0]);
        exit(1);
    }
    char *filename = argv[1];
    int fd = open(filename, O_APPEND | O_WRONLY);
    if (fd == -1)
    {
        perror("Opening file");
        exit(1);
    }
    while (1)
    {
        printf("What mark you want to write? ");
        int mark;
        scanf("%d", &mark);
        if (mark == 0) break;
        int count = sprintf(buffer, "%d\n", mark);
        write(fd, buffer, count);
    }
    close(fd);

    return 0;
}
