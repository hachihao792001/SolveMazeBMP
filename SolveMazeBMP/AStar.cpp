#include <iostream>
#include <cmath>
#include "AStar.h"
using namespace std;

//Vars
int WIDTH, HEIGHT;
Cell** cellMatrix;
bool** priorityQMatrix; //check xem cell nao da duoc push vao priority queue
Pos S;
Pos E;
Cell* priorityQ;
int qS = 0, qE = -1;
int pushi = 0;
//Functions
float calH(Cell c) {
	return (float)sqrt(pow(c.pos.x - E.x, 2) + pow(c.pos.y - E.y, 2));
}
float calF(Cell c) {
	return c.g + c.h;
}
void pushQ(Cell c) {
	qE++;
	priorityQ[qE] = c;
	priorityQMatrix[c.pos.y][c.pos.x] = true;    //c da duoc push vao priorityQ
	for (int i = qE; i > qS; i--) { //sort
		if (calF(priorityQ[i]) < calF(priorityQ[i - 1]))
			swap(priorityQ[i], priorityQ[i - 1]);
		else
			break;
	}
	//cout << ++pushi << " ";
}
Cell popQ() {
	qS++;
	return priorityQ[qS - 1];
}
bool qEmpty() {
	return qE < qS;
}

void init() {
	priorityQ = new Cell[10000000];
	cellMatrix = new Cell * [HEIGHT];
	priorityQMatrix = new bool* [HEIGHT];
	for (int i = 0; i < HEIGHT; i++) {  //init vi tri tuong ung cho tung cell
		cellMatrix[i] = new Cell[WIDTH];
		priorityQMatrix[i] = new bool[WIDTH];
		for (int j = 0; j < WIDTH; j++) {
			cellMatrix[i][j].pos.y = i;
			cellMatrix[i][j].pos.x = j;
			cellMatrix[i][j].h = calH(cellMatrix[i][j]);
			priorityQMatrix[i][j] = false;
		}
	}

	cellMatrix[S.y][S.x].g = 0;
	cellMatrix[S.y][S.x].h = calH(cellMatrix[S.y][S.x]);
	cellMatrix[S.y][S.x].parentPos = cellMatrix[S.y][S.x].pos;
	pushQ(cellMatrix[S.y][S.x]);
}

bool Safe(int y, int x, int** matrix) {
	bool safe = x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && matrix[y][x] == 0;
	return safe;
}

void Expand(int** matrix) {
	Cell cell = popQ();

	for (int i = cell.pos.y - 1; i <= cell.pos.y + 1; i++) {  //check 8 cell xung quanh
		for (int j = cell.pos.x - 1; j <= cell.pos.x + 1; j++) {
			if (i != cell.pos.y || j != cell.pos.x) {    //khong phai chinh cell do
				if (Safe(i, j, matrix)) {
					if (!priorityQMatrix[i][j]) {
						cellMatrix[i][j].g = cell.g + 1;
						cellMatrix[i][j].h = calH(cellMatrix[i][j]);
						cellMatrix[i][j].parentPos = cell.pos;
						pushQ(cellMatrix[i][j]);
					}
					else {
						if (cell.g + 1 + cell.h < cellMatrix[i][j].g + cellMatrix[i][j].h) {  //cell nao du da duoc expand thi van chua chac la toi uu, check lai duong di 1 lan nua
							cellMatrix[i][j].g = cell.g + 1;
							cellMatrix[i][j].parentPos = cell.pos;
						}
					}
				}
			}
		}
	}
}

void getResult(Pos c, int** matrix) {
	//tu cell cuoi cung lay parent cua parent... (de quy) len lai cell dau
	while (!(c.x == S.x && c.y == S.y)) {
		matrix[c.y][c.x] = 2;
		c = cellMatrix[c.y][c.x].parentPos;
	}
}

//0 can go, 1 can't go, 2 way
bool SolveMaze(int** input, int**& result, int h, int w, Pos s, Pos e) {
	bool solveResult = false;
	S = s;
	E = e;
	WIDTH = w;
	HEIGHT = h;
	init();
	while (!qEmpty()) {
		Cell topOfOList = priorityQ[qS];  //luu lai truoc khi expand vi trong expand co pop
		Expand(input);
		if (topOfOList.pos.x == E.x && topOfOList.pos.y == E.y) {
			solveResult = true;
			break;
		}
		if (qEmpty()) 
			solveResult = false;
	}
	if (solveResult) {
		getResult(cellMatrix[E.y][E.x].pos, input);

		result = new int* [HEIGHT];
		for (int i = 0; i < HEIGHT; i++) {
			result[i] = new int[WIDTH];
			for (int j = 0; j < WIDTH; j++) {
				result[i][j] = input[i][j];
			}
		}
	}

	for (int i = 0; i < HEIGHT; i++)
		delete[] cellMatrix[i];
	delete[] cellMatrix;
	for (int i = 0; i < HEIGHT; i++)
		delete[] priorityQMatrix[i];
	delete[] priorityQMatrix;
	delete priorityQ;
	return solveResult;
}