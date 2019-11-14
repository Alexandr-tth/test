#pragma once
#include "Maze.h"
namespace Actions {
	const unsigned char LEFT = 0;
	const unsigned char RIGHT = 1;
	const unsigned char UP = 2;
	const unsigned char DOWN = 3;
	const unsigned char IDLE = 4;
	const unsigned char BACK = 5;
	const unsigned char DESTROY = 6;
	const unsigned char RANDOM = 7;

	const unsigned char actCount = 8;
	const unsigned char actName[actCount][12] = { "LEFT","RIGHT", "UP", "DOWN", "IDLE", "BACK", "DESTROY", "RANDOM" };

	const unsigned char forkActions = 0;
	const unsigned char impasseActions =1;
	const unsigned char objectActions = 2;
	const unsigned char backActions[] = { 1,0,3,2 };
	const unsigned char eventActCount = 3;
	const unsigned char eventActName[eventActCount][20] = { "fork","impasse", "object meeting"};
};

class MazeObject {
public:
	MazeObject();
	MazeObject(unsigned int x, unsigned int y);
	~MazeObject();
	float GetXY(int X_or_Y);
	void SetXY(int X_or_Y, float val);
	void SetAction(char Actions);
	unsigned char getSize();
	float speed, movement;
	unsigned int xy[2];
	unsigned char Action, drawSize;
	char eventAct[3];
	char eventActChance[3];
	char* objName;
	MazeObject& operator=(MazeObject & a);
private:
};

class ObjArr {
public:
	ObjArr();
	~ObjArr();
	
	void push_back(MazeObject&);
	void setSize(unsigned int sizeArr);
	void eraze(unsigned int index);
	MazeObject& operator[](unsigned int index) { return obj[index]; }
	unsigned int getCount() { return objCount; }
	unsigned int size() { return sizeArr; }
	ObjArr& operator=(ObjArr&);
private:
	MazeObject* obj;
	unsigned int sizeArr, objCount;
};