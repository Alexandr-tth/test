#pragma once
#include "Maze.h"
#include "Object.h"
#include <random>
#include "Windows.h"
class Controller {
public:
	Controller();
	void setMaze(Maze& maze);
	void setObjects(ObjArr& obj);
	Maze& getMaze();
	ObjArr& getObjects();
	void update(float dtm);
private:
	Maze maze;
	ObjArr obj;
	char* accros(unsigned int ob);
	void nextAction(int ob,float dtm);
	void setAction(unsigned int ob, unsigned char action);
	unsigned int objAccros(unsigned int ob);
};