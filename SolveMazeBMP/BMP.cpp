#include "BMP.h"

bool ReadBMP(char* fileName, BMPFile& output) {
	FILE* f;
	fopen_s(&f, fileName, "rb");
	if (f) {
		char BM[2];
		fread(&BM, 2, 1, f);
		if (BM[0] != 'B' || BM[1] != 'M')
			return false;
		fread(&output.header, sizeof(output.header), 1, f);
		fread(&output.dib, sizeof(output.dib), 1, f);
		//fread(&output.colorTable, sizeof(output.colorTable), 1, f);

		int widthSize = output.dib.width * (output.dib.pixelDepth / 8);
		PixelArray pa = output.pixelArray;
		pa.paddingSize = (4 - (widthSize % 4)) % 4;
		pa.lineSize = widthSize + pa.paddingSize;
		pa.rawByteSize = pa.lineSize * output.dib.height;
		pa.rawBytes = new unsigned char[pa.rawByteSize];

		fseek(f, output.header.pixelArrayLocation, SEEK_SET);
		fread(pa.rawBytes, pa.rawByteSize, 1, f);

		pa.rows = output.dib.height;
		pa.cols = output.dib.width;

		pa.pixels = new Color * [pa.rows];
		for (int i = 0; i < (int)pa.rows; i++)
			pa.pixels[i] = (Color*)(pa.rawBytes + pa.lineSize * (pa.rows - 1 - i));

		output.pixelArray = pa;

		fclose(f);
	}
	else {
		cout << "Loi file" << endl;
		return false;
	}

	return true;
}

void WriteBMP(char* fileName, BMPFile input) {
	FILE* f;
	fopen_s(&f, (const char*)fileName, "wb");
	if (f) {
		char BM[2];
		BM[0] = 'B';	BM[1] = 'M';
		fwrite(&BM, 2, 1, f);
		fwrite(&input.header, sizeof(input.header), 1, f);
		fwrite(&input.dib, sizeof(input.dib), 1, f);

		//fwrite(&input.colorTable, sizeof(input.colorTable), 1, f);
		fseek(f, input.header.pixelArrayLocation, SEEK_SET);
		fwrite(input.pixelArray.rawBytes, input.pixelArray.rawByteSize, 1, f);

		fclose(f);
	}
	else {
		cout << "Loi file" << endl;
	}
}

void TurnPixelArrayToMazeMatrix(BMPFile bmp, int**& result, int& imgWidth, int& imgHeight, Pos& S, Pos& E) {
	imgWidth = bmp.dib.width, imgHeight = bmp.dib.height;
	result = new int* [imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		result[i] = new int[imgWidth];
		for (int j = 0; j < imgWidth; j++) {
			//white
			if (bmp.pixelArray.pixels[i][j].red >= 180 &&
				bmp.pixelArray.pixels[i][j].green >= 180 &&
				bmp.pixelArray.pixels[i][j].blue >= 180)
				result[i][j] = 0;
			//red
			else if (bmp.pixelArray.pixels[i][j].red >= 200 &&
				bmp.pixelArray.pixels[i][j].green <= 70 &&
				bmp.pixelArray.pixels[i][j].blue <= 70) {
				result[i][j] = 0;
				E = Pos{ i,j };
			}
			//green
			else if (bmp.pixelArray.pixels[i][j].red <= 70 &&
				bmp.pixelArray.pixels[i][j].green >= 200 &&
				bmp.pixelArray.pixels[i][j].blue <= 70) {
				result[i][j] = 0;
				S = Pos{ i,j };
			}
			//black
			else if (bmp.pixelArray.pixels[i][j].red < 180 &&
				bmp.pixelArray.pixels[i][j].green < 180 &&
				bmp.pixelArray.pixels[i][j].blue < 180)
				result[i][j] = 1;
			else
				result[i][j] = 1;
		}
	}
}

void MakeResultBMP(string ogFileName, int** result, BMPFile origin, Pos S, Pos E) {
	int b = 0;
	for (int i = origin.dib.height - 1; i >= 0; i--) {
		for (int j = 0; j < origin.dib.width; j++) {
			if (i == S.y && j == S.x) {
				origin.pixelArray.pixels[i][j].blue = 0;
				origin.pixelArray.pixels[i][j].green = 255;
				origin.pixelArray.pixels[i][j].red = 0;
			}
			else if (i == E.y && j == E.x) {
				origin.pixelArray.pixels[i][j].blue = 0;
				origin.pixelArray.pixels[i][j].green = 0;
				origin.pixelArray.pixels[i][j].red = 255;
			}
			else {
				if (result[i][j] == 0) {
					origin.pixelArray.pixels[i][j].blue = 255;
					origin.pixelArray.pixels[i][j].green = 255;
					origin.pixelArray.pixels[i][j].red = 255;
				}
				if (result[i][j] == 1) {
					origin.pixelArray.pixels[i][j].blue = 0;
					origin.pixelArray.pixels[i][j].green = 0;
					origin.pixelArray.pixels[i][j].red = 0;
				}
				if (result[i][j] == 2) {
					origin.pixelArray.pixels[i][j].blue = 255;
					origin.pixelArray.pixels[i][j].green = 0;
					origin.pixelArray.pixels[i][j].red = 0;
				}
			}

			*(origin.pixelArray.rawBytes + (b++)) = origin.pixelArray.pixels[i][j].blue;
			*(origin.pixelArray.rawBytes + (b++)) = origin.pixelArray.pixels[i][j].green;
			*(origin.pixelArray.rawBytes + (b++)) = origin.pixelArray.pixels[i][j].red;
		}
		for (int j = 0; j < origin.pixelArray.paddingSize; j++)
			*(origin.pixelArray.rawBytes + (b++)) = (unsigned char)0;
	}

	WriteBMP((char*)(ogFileName.substr(0, ogFileName.length() - 4) + "_result.bmp").c_str(), origin);
}

int CharArrToInt(char* cArr) {
	int result;
	stringstream ss;
	ss << cArr;
	ss >> result;
	ss.clear();
	return result;
}