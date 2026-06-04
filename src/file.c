// file system

#include "file.h"
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <assert.h>

#define SIGNATURE_BMP 0x4D42
#define BMP_HDR_LEN 14
#define BMP_WH_OFFSET 18
#define BMP_DEPTH_OFFSET 28

typedef struct
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
} bgr_pixel;


typedef enum
{
	UNKNOWN = 0,
	BMP,

}ImgFileType;

typedef struct {
	uint32_t width;
	uint32_t height;
}BMPDimensions;

static BMPDimensions dim = { 0,0 };
static uint32_t dibHdrSize;
static uint16_t bitDepth;
void ReadBitmapBitDepth(FILE* fp)
{
	fseek(fp, BMP_DEPTH_OFFSET, SEEK_SET);
	fread(&bitDepth, sizeof(uint16_t), 1, fp);
	printf("BMP Bit Depth: [%u]\n", bitDepth);
}
void ReadBitmapInfoHeaderSize(FILE* fp)
{
	fseek(fp, BMP_HDR_LEN, SEEK_SET);
	fread(&dibHdrSize, sizeof(uint32_t), 1, fp);
	printf("DIB header size: [%u]\n", dibHdrSize);
}
void ReadBitmapWidthHeight(FILE* fp)
{
	fseek(fp, BMP_WH_OFFSET, SEEK_SET);
	fread(&dim, sizeof(uint32_t), 2, fp);
	printf("BMP width x height: {%u x %u}\n", dim.width, dim.height);
}

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
char* LoadBMP(FILE* fp)
{

	bgr_pixel* data = NULL;
	ReadBitmapInfoHeaderSize(fp);
	ReadBitmapWidthHeight(fp);
	ReadBitmapBitDepth(fp);
	int bytesPerPixel = bitDepth / 8;
	const int numPixels = dim.width * dim.height;
	int rowSize = ((dim.width * bytesPerPixel));
	int padBytes = (4 - (rowSize % 4)) % 4; // num of bytes padded at end of row to align to 4 bytes
	int totalPadBytes = padBytes * dim.height;
	printf("padbytes per row: [%i]\n", padBytes);
	printf("padbytes * num rows: [%i]\n", totalPadBytes);
	switch (bitDepth)
	{
	case 24:
		data = malloc((sizeof(bgr_pixel) * numPixels)+totalPadBytes);
		assert(data != NULL);
		if (!data) break;
		fseek(fp, BMP_HDR_LEN + dibHdrSize, SEEK_SET);
		//for (unsigned i = 0; i < dim.height; ++i)
		{
			fread(data, sizeof(char), (numPixels*3)+totalPadBytes, fp);
			//if (i < dim.height - 1)
			//	fseek(fp, padBytes, SEEK_CUR);
		}
		break;
	default:
		printf("Unsupported BMP type!\n");
		break;
	}
	return (char*)data;
}
char* LoadImageFile(ImgFileType type, FILE* fp)
{

	switch (type)
	{
	case BMP:
		return LoadBMP(fp);
	default:
		return NULL;
	}
}


char* ei_OpenFile(const wchar_t* f)
{
	FILE* fp;
	ImgFileType imgType;
	char* imageData = NULL;
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
	if (fp)
	{
		imgType = DetectImageFormat(fp);
		imageData = LoadImageFile(imgType, fp);
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
	return imageData;
}

void ei_FreeImage(char* data)
{
	free(data);
}