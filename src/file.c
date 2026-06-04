// file system

#include "file.h"
#include <stdio.h>

void ei_OpenFile(const wchar_t* f)
{
	FILE* fp;
	
	errno_t err = _wfopen_s(&fp, f, L"r");
	if (err == 0)
	{
		wprintf(f);
		printf(" successfully opened\n");
	}
	else
	{
		wprintf(f);
		printf(" failed to open\n");
	}

	// close for now
	if (fp)
	{
		err = fclose(fp);
		if (err == 0)
		{
			wprintf(f);
			printf(" successfully closed\n");
		}
		else
		{
			wprintf(f);
			printf(" failed to close\n");
		}
	}
}