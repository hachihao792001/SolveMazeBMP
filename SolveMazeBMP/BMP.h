#pragma once
#include <iostream>
#include <cmath>
#include <sstream>
#include <stdint.h>
#include "AStar.h"
using namespace std;

struct Header {
	uint32_t fileSize;
	uint16_t specific1;
	uint16_t specific2;
	uint32_t pixelArrayLocation;
};

struct DIB {
	uint32_t DIBSize;
	int32_t width;
	int32_t height;
	uint16_t colorLayerCount;
	uint16_t pixelDepth;
	uint32_t compressionMethod;
	uint32_t imageDataSize;
	int32_t horizontalResolution;
	int32_t verticalResolution;
	uint32_t colorCount;
	uint32_t importantColorCount;
};

struct Color {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};


struct ColorTable {
	Color* colors;
	uint32_t length;
};

struct PixelArray {
	unsigned char* rawBytes;
	int rawByteSize;
	int lineSize;
	char paddingSize;

	Color** pixels;
	uint32_t rows;
	uint32_t cols;
};

struct BMPFile {
	Header header;
	DIB dib;
	ColorTable colorTable;
	PixelArray pixelArray;
};

bool ReadBMP(char* fileName, BMPFile& output);

void WriteBMP(char* fileName, BMPFile input);

void TurnPixelArrayToMazeMatrix(BMPFile bmp, int**& result, int& imgWidth, int& imgHeight, Pos& S, Pos& E);

void MakeResultBMP(string ogFileName, int** result, BMPFile origin, Pos S, Pos E);

int CharArrToInt(char* cArr);
