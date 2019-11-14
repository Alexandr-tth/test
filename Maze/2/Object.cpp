#ifndef MazeObject_2235236457458678542434576535645
#define MazeObject_2235236457458678542434576535645
#include "Object.h"
#include <random>
MazeObject::MazeObject()
{
	speed = rand()%10;
	xy[0] = 1; xy[1] = 1;
	Action = Actions::IDLE;
	movement = 0;
	drawSize = rand() % 30;
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

void MazeObject::update(Maze& maze, float dtm){
	char* cross;
	movement += dtm*speed;
	while (movement > 1.0f) {
		switch (Action) {
		case Actions::UP:
			 xy[1] -= 1; nextAction(maze, dtm);
			break;
		case Actions::RIGHT:
			 xy[0] += 1; nextAction(maze, dtm);
			break;
		case Actions::DOWN:
			 xy[1] += 1; nextAction(maze, dtm);
			break;
		case Actions::LEFT:
			 xy[0] -= 1; nextAction(maze, dtm);
			break;
		case Actions::IDLE:
			cross = accros(maze);
			if (cross[0] != 0) { Action = cross[(rand() % cross[0]) + 1]; }
			break;
		}
		movement -= 1.0f;
	}
	
}

void MazeObject::draw(int, int)
{
}

unsigned char MazeObject::getSize()
{
	return drawSize;
}

char* MazeObject::accros(Maze& maze) {
	static char ret[5]; ret[0] = 0;
	if (maze.GetField(xy[0],     xy[1] - 1) == fldType::EMPTY) { ret[0]++; ret[ret[0]] = Actions::UP;}
	if (maze.GetField(xy[0] + 1, xy[1]    ) == fldType::EMPTY) { ret[0]++; ret[ret[0]] = Actions::RIGHT;}
	if (maze.GetField(xy[0],     xy[1] + 1) == fldType::EMPTY) { ret[0]++; ret[ret[0]] = Actions::DOWN;}
	if (maze.GetField(xy[0] - 1, xy[1]    ) == fldType::EMPTY) { ret[0]++; ret[ret[0]] = Actions::LEFT;}
	return ret;
}
void MazeObject::nextAction(Maze& maze, float dtm){
	char* cross;
	/*switch (Action)
	{
	case Actions::UP:
		if (maze.GetField(GetXY(0), GetXY(1) - 1) == fldType::WALL){Action =  Actions::IDLE;}
		break;
	case Actions::RIGHT:if (maze.GetField(GetXY(0) + 1, GetXY(1)) == fldType::WALL) { Action = Actions::IDLE;}
		break;
	case Actions::DOWN:if (maze.GetField(GetXY(0), GetXY(1) + 1) == fldType::WALL) { Action = Actions::IDLE;}
		break;
	case Actions::LEFT:if (maze.GetField(GetXY(0) - 1, GetXY(1)) == fldType::WALL) { Action = Actions::IDLE;}
		break;
	case Actions::IDLE:
		cross = accros(maze);
		if (cross[0] != 0) { Action = cross[(rand() % cross[0]) + 1];}
		break;
	default:
		break;
	}*/
	cross = accros(maze);
	Action = cross[(rand() % cross[0]) + 1];
	if (cross[0] > 1 && rand() % 2 == 1) { Action = cross[(rand() % cross[0]) + 1];}
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

void ObjArr::update(Maze& maze, float dtm){
	for (int i = 0; i < objCount; i++) {
		obj[i].update(maze, dtm);
	}
}

#endif