#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COUNT 50

int main()
{
    FILE *fout = fopen("file.ext", "w");
    int retval;
    for (int i = 0; i < COUNT; ++i)
    {
        sleep(1);
        if ((retval = fprintf(fout, "%d\n", i)) <= 0)
        {
            printf("fprintf error %d\n", retval);
            break;
        }
        fflush(fout);
    }
    if (fclose(fout))
    {
        printf("error on closing file\n");
    }
    return 0;
}
