#include "AStar.h"
#include "BMP.h"

int main(int argc, char** argv) {

	bool saiCuPhap = false;
	if (argc == 2) {

		BMPFile fileBMP;
		if (!ReadBMP(argv[1], fileBMP))
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
				MakeResultBMP(argv[1], result, fileBMP, S, E);
				for (int i = 0; i < imgh; i++)
					delete[] result[i];
				delete[] result;
			}else
				cout << "IMPOSSIBLE!" << endl;

			for (int i = 0; i < imgh; i++)
				delete[] mazeMatrix[i];
			delete[] mazeMatrix;
		}
	}
	else
		saiCuPhap = true;

	if (saiCuPhap) cout << "Nhap sai cu phap" << endl;

	return 0;
}