// file system interface
#ifndef FILE_H
#define FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Windows.h>

char* ei_OpenFile(const wchar_t* f, int* width, int* height);
void ei_FreeImage(char* data);

#ifdef __cplusplus
}
#endif

#endif // !FILE_H
