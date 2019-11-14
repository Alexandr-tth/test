#include "Maze.h"
#include <random>
#include <Windows.h>

const char Maze::errors[2][30] = { "¬ыход за пределы массива","ed" };
Maze::Maze() {
	field = 0;
	SetSize(50, 50);
	SetField(5, 5, fldType::WALL);
	SetField(6, 5, fldType::WALL);
}
Maze::~Maze() {

}
void Maze::SetSize(unsigned int widthF, unsigned int heightF) {
	unsigned char* temp = new unsigned char[widthF*heightF];
	for (int i = 0; i < widthF*heightF; i++) {
		temp[i] = fldType::WALL;
	}
	int x, y;
	if (widthF > this->widthF) { x = this->widthF; }
	else { x = widthF; }
	if (heightF > this->heightF) { y = this->heightF; }
	else { y = heightF; }
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			temp[i*widthF+j] = field[i*this->widthF + j];
		}
	}
	if(field!=0)delete[] field;
	field = temp;
	this->widthF = widthF;
	this->heightF = heightF;
}
void Maze::SetField(unsigned int x, unsigned int y, unsigned char fldType) {
	if (x >= widthF || y >= heightF)return;// throw 0;
	field[y*widthF + x] = fldType;
}
unsigned char Maze::GetField(unsigned int x, unsigned int y) {
	if (x >= widthF || y >= heightF)return fldType::WALL;// throw 0;
	return field[y*widthF + x];
}
unsigned int Maze::GetWidth() {
	return widthF;
}
unsigned int Maze::GetHeight() {
	return heightF;
}
#define circleMaze
void Maze::randomFill(unsigned int _srand) {
	int x = 1, y = 1;
	int target;
	int counter=0;
	bool stop = true;
	srand(_srand);
	// очистка
	for (int i = 0; i < widthF*heightF; i++) {
		field[i] = fldType::WALL;
	}
	// заполнение
	while (stop) {
		target = rand() % 4;
		switch (target) {
		case 0:
			if (y - 2 > 0 && y - 2 < heightF ) {
#ifdef circleMaze
				if (GetField(x, y - 2) == fldType::EMPTY && GetField(x, y - 1) == fldType::WALL)break;
#endif
				SetField(x, y - 1, fldType::EMPTY);
				SetField(x, y - 2, fldType::EMPTY);
				y -= 2;
			}
			break;

		case 1:
			if (x + 2 > 0 && x + 2 < widthF ) {
#ifdef circleMaze
				if (GetField(x + 2, y) == fldType::EMPTY && GetField(x + 1, y) == fldType::WALL)break;
#endif
				SetField(x + 1, y, fldType::EMPTY);
				SetField(x + 2, y, fldType::EMPTY);
				x += 2;
			}
			break;
		case 2:
			if (y + 2 > 0 && y + 2 < heightF ) {
#ifdef circleMaze
				if (GetField(x, y + 2) == fldType::EMPTY && GetField(x, y + 1) == fldType::WALL)break;
#endif
				SetField(x, y + 1, fldType::EMPTY);
				SetField(x, y + 2, fldType::EMPTY);
				y += 2;
			}
			break;
		case 3:
			if (x - 2 > 0 && x - 2 < widthF ) {
#ifdef circleMaze
				if (GetField(x - 2, y) == fldType::EMPTY && GetField(x - 1, y) == fldType::WALL)break;
#endif
				SetField(x - 1, y, fldType::EMPTY);
				SetField(x - 2, y, fldType::EMPTY);
				x -= 2;
			}
			break;
		}
		if (counter == 15) {
			for (int i = 1; i < heightF; i+=2) {
				for (int j = 1; j < widthF ; j += 2) {
					if (GetField(j, i) == fldType::WALL) { 
						counter = -1; break;
					}
				}
			}
			if (counter != -1)stop = false;
			counter = 0;
		}
		++counter;
	}

}