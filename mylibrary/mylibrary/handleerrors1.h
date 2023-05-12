#pragma once

#ifndef HANDLE_ERRORS_H
#define HANDLE_ERRORS_H

#define WIN32_LEAN_AND_MEAN             // exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <strsafe.h>

//refer to https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
static void ErrorExit(LPTSTR lpszFunction)
{
    DWORD err = GetLastError();

    if (err) {
        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
           // (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
        (lstrlen((LPCWSTR)lpMsgBuf) + lstrlen((LPCWSTR)lpszFunction)  + 40) * sizeof(TCHAR));
        StringCchPrintf((LPTSTR)lpDisplayBuf,
            (size_t)LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s failed with error %d: %s"), lpszFunction, err, lpMsgBuf);
        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error: Exit Program"), MB_OK);


        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
        ExitProcess(err);
    }

}


#endif
