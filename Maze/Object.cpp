#ifndef MazeObject_2235236457458678542434576535645
#define MazeObject_2235236457458678542434576535645
#include "Object.h"
#include <random>
MazeObject::MazeObject():MazeObject(1, 1)
{

}

MazeObject::MazeObject(unsigned int x, unsigned int y){
	speed = rand()%10+1;
	xy[0] = x; xy[1] = y;
	Action = Actions::IDLE;
	movement = 0;
	drawSize = rand() % 5+5;
	eventAct[Actions::forkActions] = Actions::BACK;
	eventActChance[Actions::forkActions] = rand() % 10;
	eventAct[Actions::impasseActions] = Actions::BACK;
	eventActChance[Actions::impasseActions] = rand() % 10;
	eventAct[Actions::objectActions] = Actions::BACK;
	eventActChance[Actions::objectActions] = rand() % 10;
	objName = (char*)new int[40];
	strcpy_s(objName,11, "new object");
}

MazeObject::~MazeObject(){
	if (objName)delete objName;
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
}

void MazeObject::SetXY(int X_or_Y, float val)
{
	xy[X_or_Y] = val;
}

void MazeObject::SetAction(char Actions){
	if(Action== Actions::IDLE)Action = Actions;
}
unsigned char MazeObject::getSize()
{
	return drawSize;
}

MazeObject & MazeObject::operator=(MazeObject & a){
	memcpy(this, &a, sizeof(MazeObject));
	int nameSize = strlen(a.objName)+1;
	this->objName = new char[nameSize];
	for (int i = 0; i < nameSize; i++)
		this->objName[i] = a.objName[i];
	return *this;
}






ObjArr::ObjArr()
{
	const int defSize = 10;
	obj = new MazeObject[defSize];
	sizeArr = defSize;
	objCount = 10;
}

ObjArr::~ObjArr()
{
	delete[] obj;
}

void ObjArr::push_back(MazeObject & object)
{
	if (objCount  < sizeArr) {
		obj[objCount++] = object;
	}
	else {
		setSize(sizeArr * 2);
		obj[objCount++] = object;
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
	--objCount;
}

ObjArr& ObjArr::operator=(ObjArr& a){
	delete[] obj;
	memcpy(this, &a, sizeof(Maze));
	obj = new MazeObject[a.getCount()];
	for (int i = 0; i < a.getCount(); i++) {
		obj[i] = a.obj[i];
	}
	return *this;
}


#endif