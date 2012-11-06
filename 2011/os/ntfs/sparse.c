#include <windows.h>
#include <winioctl.h>
#include <stdio.h>

#include "safe.h"

// 1 TiB
#define FILENAME "sparse.dat"
#define SPARSE_FILE_SIZE 1099511627776L

int main()
{
    int iRetCode = 0;
    HANDLE hFile;
    DWORD dwDummy;
    LARGE_INTEGER llFileSize;

    llFileSize.QuadPart = SPARSE_FILE_SIZE;
    printf("Creating sparse file %s (size = %lli GB)\n", FILENAME,
        llFileSize.QuadPart / 1024 / 1024 / 1024);

    WHANDLEOK(hFile = CreateFile(FILENAME, GENERIC_WRITE, FILE_SHARE_WRITE,
        NULL, CREATE_ALWAYS, 0, NULL));

    WNONZERO(DeviceIoControl(hFile, FSCTL_SET_SPARSE, NULL, 0, NULL, 0,
        &dwDummy, NULL));

    WNONZERO(SetFilePointerEx(hFile, llFileSize, NULL, FILE_BEGIN));

    WNONZERO(SetEndOfFile(hFile));

cleanup:
    CloseHandle(hFile);
    return iRetCode;
}
