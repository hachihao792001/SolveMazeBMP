#pragma once

//Structs
struct Pos {
	int y, x;
};
struct Cell {
	float g, h;
	Pos parentPos, pos;
};

float calH(Cell c);
float calF(Cell c);
void pushQ(Cell c);
Cell popQ();
bool qEmpty();
void init();
bool Safe(int y, int x, int** matrix);
void Expand(int** matrix);
void getResult(Pos c, int** matrix);

bool SolveMaze(int** input, int**& result, int h, int w, Pos s, Pos e);
