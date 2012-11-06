#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "marks"

int main()
{
    int handle = open(FILENAME, O_APPEND | O_WRONLY);
    if (handle == -1)
    {
        printf("opening error\n");
        return 1;
    }
    while (1)
    {
        char buffer[1024];
        int count = sprintf(buffer, "%d\n", 100500);
        write(handle, buffer, count);
    }
    close(handle);
    return 0;
}
