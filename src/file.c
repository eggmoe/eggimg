// file system

#include "file.h"
#include <stdio.h>
#include <stdint.h>

#define SIGNATURE_BMP 0x4D42

typedef enum
{
	UNKNOWN = 0,
	BMP,

}ImgFileType;

// take an open filestream and return its ImgFileType
// returns 0 for unknown file
ImgFileType DetectImageFormat(FILE* fp)
{
	printf("Detecting Image Format\n");
	static char buff[32];
	// reset position indicator to beginning of file
	fseek(fp,0, SEEK_SET);
	fread(buff, sizeof(char), sizeof(buff), fp);
	if (buff[0] == 'B' && buff[1] == 'M') {
		printf("Image is a BMP\n");
		printf("Bytes[0][1]: [%c][%c]\n", buff[0], buff[1]);
		return BMP;

	}
	else {
		printf("Unkown Image Format\n");
		printf("Bytes[0][1]: [%c][%c]\n", buff[0], buff[1]);

		return UNKNOWN;
	}
}



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

	DetectImageFormat(fp);
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