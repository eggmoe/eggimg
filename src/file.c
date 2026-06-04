// file system

#include "file.h"
#include <stdio.h>

void OpenFile(const char* f)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, f, "r");
	if (err == 0)
	{
		printf("%s successfully opened\n", f);
	}
	else
	{
		printf("%s failed to open\n", f);
	}

	// close for now
	if (fp)
	{
		err = fclose(fp);
		if (err == 0)
		{
			printf("%s successfully closed\n", f);
		}
		else
		{
			printf("%s failed to close\n", f);
		}
	}
}