#pragma once
#include "Maze.h"
namespace Actions {
	const unsigned char LEFT = 0;
	const unsigned char RIGHT = 1;
	const unsigned char UP = 2;
	const unsigned char DOWN = 3;
	const unsigned char IDLE = 4;
};

class MazeObject {
public:
	MazeObject();
	float GetXY(int X_or_Y);
	void SetXY(int X_or_Y, float val);
	void SetAction(char Actions);
	void update(Maze& m, float dt);
	void draw(int, int);
	unsigned char getSize();
private:
	void nextAction(Maze& maze, float dtm);
	char* accros(Maze& maze);
	float speed, movement;
	unsigned int xy[2];
	char Action, drawSize;
};
class ObjArr {
public:
	ObjArr();
	~ObjArr();
	MazeObject* obj;
	unsigned int sizeArr, objCount;
	void push_back(MazeObject&);
	void setSize(unsigned int sizeArr);
	void eraze(unsigned int index);
	void update(Maze& m,float dtm);
	MazeObject& operator[](unsigned int index) { return obj[index]; }
};