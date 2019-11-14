#ifndef MazeObject_2235236457458678542434576535645
#define MazeObject_2235236457458678542434576535645
#include "Object.h"
#include <random>
MazeObject::MazeObject()
{
	speed = rand()%20+5;
	xy[0] = 1; xy[1] = 1;
	Action = Actions::IDLE;
	movement = 0;
	drawSize = rand() % 30;
	eventAct[Actions::forkActions] = Actions::BACK;
	eventActChance[Actions::forkActions] = 0;
	eventAct[Actions::impasseActions] = Actions::BACK;
	eventActChance[Actions::impasseActions] = 0;
	eventAct[Actions::objectActions] = Actions::DESTROY;
	eventActChance[Actions::objectActions] = 100;
}

float MazeObject::GetXY(int X_or_Y)
{
	if (X_or_Y) {
		if (Action == Actions::UP) {
			return xy[1] - movement;
		}
		else if(Action == Actions::DOWN)
		{
			return xy[1] + movement;
			
		}else{ return xy[1]; }
	}else {
		if (Action == Actions::LEFT) {
			return xy[0] - movement;
		}
		else if (Action == Actions::RIGHT)
		{
			return xy[0] + movement;

		}
		else { return xy[0]; }
	}
	//return xy[X_or_Y] + ((Action & 3) && !X_or_Y ? movement[1] : movement[0]);// (Action & 3) && !X_or_Y ? movement: 0;
}

void MazeObject::SetXY(int X_or_Y, float val)
{
	xy[X_or_Y] = val;
}

void MazeObject::SetAction(char Actions){
	if(Action== Actions::IDLE)Action = Actions;
}


void MazeObject::draw(int, int)
{
}

unsigned char MazeObject::getSize()
{
	return drawSize;
}




ObjArr::ObjArr()
{
	const int defSize = 30;
	obj = new MazeObject[defSize];
	sizeArr = defSize;
	objCount = 30;
}

ObjArr::~ObjArr()
{
	delete[]obj;
}

void ObjArr::push_back(MazeObject & object)
{
	if (objCount + 1 < sizeArr) {
		obj[++objCount] = object;
	}
	else {
		setSize(sizeArr * 2);
		obj[++objCount] = object;
	}
}

void ObjArr::setSize(unsigned int _size)
{
	MazeObject* temp = new MazeObject[_size];
	for (int i = 0; i < objCount; i++) {
		temp[i] = obj[i];
	}
	delete[] obj;
	obj = temp;
	sizeArr = _size;
}

void ObjArr::eraze(unsigned int index)
{
	for (int i = index; i < objCount-1; i++) {
		obj[i] = obj[i+1];
	}
}




#endif