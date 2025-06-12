#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

enum { RETURN_OK = 1, RETURN_ERROR = 0 };

/* Ensure that the directory is empty.
 * returns RETURN_OK (1) on success,
 * returns RETURN_ERROR (0) if the directory is not empty or an error occurs.
 */
static int dir_is_empty(const wchar_t *dir) {
    wchar_t pattern[MAX_PATH];

    if (_snwprintf_s(pattern, MAX_PATH, _TRUNCATE, L"%s\\*", dir) < 0) {
        return RETURN_ERROR;
    };

    WIN32_FIND_DATAW file_desc;
    HANDLE f_handle = FindFirstFileW(pattern, &file_desc);

    if (f_handle == INVALID_HANDLE_VALUE) {
        return RETURN_ERROR;
    };

    int empty = RETURN_OK; /* assume directory is empty */
    do {
        if (wcscmp(file_desc.cFileName, L".") != 0 && wcscmp(file_desc.cFileName, L"..") != 0) {
            empty = RETURN_ERROR; /* found a non-dot entry */
            break;
        }
    } while (FindNextFileW(f_handle, &file_desc));

    FindClose(f_handle);
    return empty;
}

/* Complete validations on the provided path
 * returns RETURN_OK (1) on success,
 * returns RETURN_ERROR (0) if validation fails.
 */
int validation_check(const wchar_t *path) {

    /* Ensure that the path is not NULL or empty,
     * reject if it is.
     */
    if (!path || !*path) {
        fwprintf(stderr, L"Path is NULL or empty\n");
        return RETURN_ERROR;
    };

    const DWORD attrs = GetFileAttributesW(path);
    fwprintf(stderr, L"Path is %s\n", path);
    fwprintf(stderr, L"File attributes is %d\n", attrs);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        fwprintf(stderr, L"Path \"%s\" does not exist\n", path);
        return RETURN_ERROR;
    };
    if (!(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
        fwprintf(stderr, L"Path \"%s\" is not a directory\n", path);
        return RETURN_ERROR;
    };

    if (wcslen(path) >= MAX_PATH) {
        fwprintf(stderr, L"Path is too long\n");
        return RETURN_ERROR;
    };

    return RETURN_OK;
}

int wmain(const int argc, wchar_t *argv[]) {
    if (argc != 2) {
        fwprintf(stderr, L"Usage: %s <folder>\n", argv[0]);
        return RETURN_ERROR;
    };

    /* Keep trailing back-slash if the user passed root (C:\) */
    const size_t len = wcslen(argv[1]);
    if (len > 3 && argv[1][len - 1] == L'\\') {
        argv[1][len - 1] = L'\0';
    };

    if (validation_check(argv[1]) == RETURN_ERROR) {
        return RETURN_ERROR;
    }

    /* Enumerate first-level children of root path */
    wchar_t pattern[MAX_PATH];
    if (_snwprintf_s(pattern, MAX_PATH, _TRUNCATE, L"%s\\*", argv[1]) < 0) {
        fwprintf(stderr, L"Path too long\n");
        return RETURN_ERROR;
    };

    WIN32_FIND_DATAW file_desc;
    HANDLE handle = FindFirstFileW(pattern, &file_desc);
    if (handle == INVALID_HANDLE_VALUE) {
        fwprintf(stderr, L"Cannot enumerate %s (err %lu)\n", argv[1], GetLastError());
        return RETURN_ERROR;
    };

    do {
        if (!(file_desc.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            continue; /* not a dir */
        };
        if (!wcscmp(file_desc.cFileName, L".") || !wcscmp(file_desc.cFileName, L"..")) {
            continue; /* skip . and .. */
        };

        wchar_t sub[MAX_PATH];
        if (_snwprintf_s(sub, MAX_PATH, _TRUNCATE, L"%s\\%s", argv[1], file_desc.cFileName) < 0) {
            continue;
        };

        if (dir_is_empty(sub)) { /* If return is 1 (true), the directory is empty */
            if (RemoveDirectoryW(sub)) {
                wprintf(L"Deleted empty directory: %s\n", sub);
            } else {
                fwprintf(stderr, L"Failed to delete %s (error %lu)\n", sub, GetLastError());
            };
        }
    } while (FindNextFileW(handle, &file_desc));

    FindClose(handle);
    return RETURN_OK;
}
