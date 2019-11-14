#pragma once
#include "Maze.h"
#include "Object.h"
#include <random>
#include "Windows.h"
class Controller {
public:
	Controller();
	Maze maze;
	ObjArr obj;
	void update(float dtm);
private:
	void setAction(unsigned int ob, unsigned char action);
	char* accros(unsigned int ob);
	void nextAction(int ob,float dtm);
	unsigned int objAccros(unsigned int ob);
};