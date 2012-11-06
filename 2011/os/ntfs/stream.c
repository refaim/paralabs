#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <wchar.h>

#include "safe.h"

#define DEFAULT_STREAM_SIZE_MB 150

#define MODE_ADD 1
#define MODE_LIST 2

int AddDataStream(char *szFilename, char *szStreamName, DWORD dwStreamSize)
{
    int iRetCode = EXIT_SUCCESS;

    char *szFillData = NULL;
    char *szFullName = NULL;

    HANDLE *hFile = NULL;
    DWORD dwWritten;

    szFullName = ALLOC_CHECK(malloc(strlen(szFilename) + strlen(szStreamName)));
    sprintf(szFullName, "%s:%s", szFilename, szStreamName);

    WHANDLEOK(hFile = CreateFile(szFullName, GENERIC_WRITE, FILE_SHARE_WRITE,
        NULL, CREATE_ALWAYS, 0, NULL));

    printf("Writing %u megabytes to %s\n", dwStreamSize, szFullName);

    dwStreamSize *= 1024 * 1024; // mb to bytes
    szFillData = ALLOC_CHECK(malloc(dwStreamSize));
    memset(szFillData, 'X', dwStreamSize);
    szFillData[dwStreamSize - 1] = '\0';

    WNONZERO(WriteFile(hFile, szFillData, dwStreamSize, &dwWritten, NULL));

cleanup:
    CloseHandle(hFile);
    free(szFullName);
    free(szFillData);
    return iRetCode;
}

void DumpStreamInfo(char *szFilename, WIN32_STREAM_ID *psStreamInfo)
{
    wchar_t arSeparators[] = L":";
    wchar_t *szStreamShortName = L"<noname>";
    const char *szStreamType;
    DWORD dwStreamSize = psStreamInfo->Size.QuadPart / (1024 * 1024);

    if (psStreamInfo->dwStreamNameSize > 0)
    {
        szStreamShortName = wcstok(psStreamInfo->cStreamName, arSeparators);
    }

    switch (psStreamInfo->dwStreamId)
    {
        case BACKUP_DATA: { szStreamType = "data"; break; }
        case BACKUP_EA_DATA: { szStreamType = "extended attributes"; break; }
        case BACKUP_SECURITY_DATA: { szStreamType = "security"; break; }
        case BACKUP_ALTERNATE_DATA: { szStreamType = "other"; break; }
        case BACKUP_LINK: { szStreamType = "link"; break; }
        default: { szStreamType = "unknown"; break; }
    }

    printf("[%s] %s:%S", szStreamType, szFilename, szStreamShortName);
    if (dwStreamSize > 0)
    {
        printf(" = %d mb\n", dwStreamSize);
    }
    else
    {
        printf("\n");
    }
}

int ListDataStreams(char *szFilename)
{
    int iRetCode = EXIT_SUCCESS;

    HANDLE *hFile = NULL;
    LPVOID lpContext = NULL;
    byte pbyBuffer[4096];
    WIN32_STREAM_ID *psStreamInfo = (WIN32_STREAM_ID *)pbyBuffer;
    int uiHeaderLength = 20;
    DWORD dwRead, dwDummy;

    WHANDLEOK(hFile = CreateFile(szFilename, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, 0, NULL));

    while (1)
    {
        WNONZERO(BackupRead(hFile, pbyBuffer, uiHeaderLength, &dwRead,
            FALSE, TRUE, &lpContext));

        if (dwRead == 0) break;

        if (psStreamInfo->dwStreamNameSize > 0)
        {
            WNONZERO(BackupRead(hFile, pbyBuffer + uiHeaderLength,
                psStreamInfo->dwStreamNameSize, &dwRead, FALSE, TRUE,
                &lpContext));

            if (dwRead != psStreamInfo->dwStreamNameSize) break;
        }

        DumpStreamInfo(szFilename, psStreamInfo);

        if (psStreamInfo->Size.QuadPart > 0)
        {
            BackupSeek(hFile, psStreamInfo->Size.LowPart,
                psStreamInfo->Size.HighPart, &dwDummy, &dwDummy, &lpContext);
        }
    }

cleanup:
    // release context
    BackupRead(hFile, pbyBuffer, 0, &dwRead, TRUE, FALSE, &lpContext);
    CloseHandle(hFile);
    return iRetCode;
}

int main(int argc, char **argv)
{
    int iRetCode = EXIT_SUCCESS;

    char *szFilename = NULL;
    int iOptionIndex, iArgIndex, iMode, iGetoptRet;
    struct option arLongOpts[] =
    {
        { "add", no_argument, &iMode, MODE_ADD },
        { "list", no_argument, &iMode, MODE_LIST },
        { 0, 0, 0, 0 }
    };

    do
    {
        iGetoptRet = getopt_long(argc, argv, "", arLongOpts, &iOptionIndex);
        if (iGetoptRet == '?')
        {
            return EXIT_FAILURE;
        }
    } while (iGetoptRet != -1);

    iArgIndex = 1 + 1; // executable name + option
    if (iArgIndex == argc)
    {
        printf("stream: no filename specified\n");
        return EXIT_FAILURE;
    }
    szFilename = argv[iArgIndex];

    switch (iMode)
    {
        case MODE_ADD:
        {
            char *szStreamName = NULL;
            DWORD dwStreamSize;

            if (++iArgIndex == argc)
            {
                printf("stream: no stream specified\n");
                return EXIT_FAILURE;
            }
            szStreamName = argv[iArgIndex];

            if (++iArgIndex == argc)
            {
                dwStreamSize = DEFAULT_STREAM_SIZE_MB;
            }
            else
            {
                dwStreamSize = atoi(argv[iArgIndex]);
            }

            iRetCode = AddDataStream(szFilename, szStreamName, dwStreamSize);
            break;
        }
        case MODE_LIST:
        {
            iRetCode = ListDataStreams(szFilename);
            break;
        }
        default:
        {
            printf("stream: unknown mode %d\n", iMode);
            return EXIT_FAILURE;
        }
    }

    return iRetCode;
}
