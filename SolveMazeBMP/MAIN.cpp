#include "AStar.h"
#include "BMP.h"

#include <cwchar>
#include <windows.h>

int main(int argc, char** argv) {

	bool saiCuPhap = false;
	if (argc == 2 || argc == 1) {
		string fileName;
		BMPFile fileBMP;

		if (argc == 1) {
			cout << "Nhap duong dan file: ";
			cin >> fileName;
		}
		else
			fileName = argv[1];

		if (!ReadBMP((char*)fileName.c_str(), fileBMP))
			cout << "File khong phai la file BMP" << endl;
		else {
			int** mazeMatrix;
			int imgw, imgh;
			Pos S, E;
			TurnPixelArrayToMazeMatrix(fileBMP, mazeMatrix, imgw, imgh, S, E);
			int** result;
			if (SolveMaze(mazeMatrix, result, imgh, imgw, S, E)) {
				/*
				for (int i = 0; i < imgh; i++) {
					for (int j = 0; j < imgw; j++) {
						cout << result[i][j] << " ";
					}
					cout << endl;
				}
				*/
				MakeResultBMP(fileName, result, fileBMP, S, E);

				CONSOLE_FONT_INFOEX cfi;
				cfi.cbSize = sizeof(cfi);
				cfi.nFont = 0;
				cfi.dwFontSize.X = 1000 / imgw;                   // Width of each character in the font
				cfi.dwFontSize.Y = 1000 / imgh;	// Height
				cfi.FontFamily = FF_DONTCARE;
				cfi.FontWeight = FW_NORMAL;
				wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
				SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

				for (int i = 0; i < imgh; i++) {
					for (int j = 0; j < imgw; j++) {
						if (result[i][j] == 0)
							cout << "_";
						else if (result[i][j] == 1)
							cout << ".";
						else
							cout << "*";
					}
					cout << endl;
				}

				for (int i = 0; i < imgh; i++)
					delete[] result[i];
				delete[] result;

				cout << "PATH FOUND!" << endl;
			}
			else
				cout << "IMPOSSIBLE!" << endl;
			for (int i = 0; i < imgh; i++)
				delete[] mazeMatrix[i];
			delete[] mazeMatrix;
		}
	}
	else
		saiCuPhap = true;

	if (saiCuPhap) cout << "Nhap sai cu phap" << endl;
	system("pause");
	return 0;
}