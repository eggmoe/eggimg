// file system interface
#ifndef FILE_H
#define FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Windows.h>

void ei_OpenFile(const wchar_t* f);

#ifdef __cplusplus
}
#endif

#endif // !FILE_H
