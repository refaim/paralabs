#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define FAIL() iRetCode = EXIT_FAILURE; goto cleanup

#define WINAPI_ERROR() winapi_error(__FILE__, __LINE__)
#define WCHECK(call, value) if ((call) == (value)) { WINAPI_ERROR(); FAIL(); }
#define WNONZERO(call) WCHECK(call, 0)
#define WHANDLEOK(call) WCHECK(call, INVALID_HANDLE_VALUE)

int winapi_error(const char *szFilename, int iLine)
{
    printf("WinAPI Error #%lu on %s:%d", GetLastError(), szFilename, iLine);
    return EXIT_FAILURE;
}


void *check_alloc(void *pMemory, char* szFilename, int iLine)
{
    if (pMemory == NULL)
    {
        fprintf(stderr, "Allocation error on %s:%d!", szFilename, iLine);
        exit(EXIT_FAILURE);
    }
    return pMemory;
}
#define ALLOC_CHECK(mem) check_alloc(mem, __FILE__, __LINE__)
