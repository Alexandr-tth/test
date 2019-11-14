#include "Controller.h"

Controller::Controller():maze(){
	maze.randomFill(352);
}

void Controller::setMaze(Maze & maze)
{
	this->maze = maze;
}

void Controller::setObjects(ObjArr & obj){
	this->obj = obj;
}

Maze & Controller::getMaze()
{
	return maze;
}

ObjArr & Controller::getObjects()
{
	return obj;
}

void Controller::update(float dtm){
	for (int i = 0; i < obj.getCount(); i++) {
		char* cross =0;
		obj[i].movement += dtm*obj[i].speed;
		while (obj[i].movement > 1.0f) {
			switch (obj[i].Action) {
			case Actions::UP:
				obj[i].xy[1] -= 1; nextAction(i, dtm);
				break;
			case Actions::RIGHT:
				obj[i].xy[0] += 1; nextAction(i, dtm);
				break;
			case Actions::DOWN:
				obj[i].xy[1] += 1; nextAction(i, dtm);
				break;
			case Actions::LEFT:
				obj[i].xy[0] -= 1; nextAction(i, dtm);
				break;
			case Actions::IDLE:
				cross = accros(i);
				if (cross[0] != 0) { obj[i].Action = cross[(rand() % cross[0]) + 1]; }
				break;
			case Actions::DESTROY:
				if (obj[i].drawSize != 0) {
					obj[i].drawSize -= 1;
				}
				else {
					obj.eraze(i);
				}
				break;
			case Actions::RANDOM:
				if(obj[i].Action<4)
				cross = accros(i);
				if (cross[0] != 0) { obj[i].Action = cross[(rand() % cross[0]) + 1]; }
				break;
			}
			obj[i].movement -= 1.0f;
		}
	}
}


void Controller::setAction(unsigned int ob, unsigned char action){
	char* cross;
	switch (action) {
	case Actions::UP:
		if (maze.GetField(obj[ob].xy[0], obj[ob].xy[1] - 1) != fldType::WALL) { obj[ob].Action = Actions::UP; return; }
		break;
	case Actions::RIGHT:
		if (maze.GetField(obj[ob].xy[0]+1, obj[ob].xy[1]) != fldType::WALL) { obj[ob].Action = Actions::RIGHT; return;}
		break;
	case Actions::DOWN:
		if (maze.GetField(obj[ob].xy[0], obj[ob].xy[1] + 1) != fldType::WALL) { obj[ob].Action = Actions::DOWN; return;}
		break;
	case Actions::LEFT:
		if (maze.GetField(obj[ob].xy[0]-1, obj[ob].xy[1]) != fldType::WALL) { obj[ob].Action = Actions::LEFT; return;}
		break;
	case Actions::BACK:
		if (obj[ob].Action < 4) { obj[ob].Action = Actions::backActions[obj[ob].Action]; return; }
		break;
	case Actions::DESTROY:
		obj[ob].Action = Actions::DESTROY;
		return;
		break;
	case Actions::RANDOM:
		cross = accros(ob);
		if (cross[0] != 0) { obj[ob].Action = cross[(rand() % cross[0]) + 1]; }
		else { obj[ob].Action = Actions::IDLE;  return;}
		break;
	}
	cross = accros(ob);
	if (cross[0] != 0) { obj[ob].Action = cross[(rand() % cross[0]) + 1]; }
}

char * Controller::accros(unsigned int ob)
{
	static char ret[5]; ret[0] = 0;
	if (maze.GetField(obj[ob].xy[0], obj[ob].xy[1] - 1) != fldType::WALL) { ret[0]++; ret[ret[0]] = Actions::UP; }
	if (maze.GetField(obj[ob].xy[0] + 1, obj[ob].xy[1]) != fldType::WALL) { ret[0]++; ret[ret[0]] = Actions::RIGHT; }
	if (maze.GetField(obj[ob].xy[0], obj[ob].xy[1] + 1) != fldType::WALL) { ret[0]++; ret[ret[0]] = Actions::DOWN; }
	if (maze.GetField(obj[ob].xy[0] - 1, obj[ob].xy[1]) != fldType::WALL) { ret[0]++; ret[ret[0]] = Actions::LEFT; }
	return ret;
}

void Controller::nextAction(int ob,float dtm)
{
	char* cross=0; unsigned int objAcc=-1;
		if (obj[ob].eventActChance[Actions::forkActions] >= rand() % 100) {// если выпало действие на развилке
			cross = accros(ob);
			if (cross[0] > 2) {//если развилка
				if (obj[ob].eventAct[Actions::forkActions] < 4) {//если действие двигаться
					for (int i = 1; i < cross[0] + 1; i++) {
						if (obj[ob].eventAct[Actions::forkActions] == cross[i]) {//действие возможно
							obj[ob].Action = cross[i];
							return;
						}
					}
				}
				else {
					setAction(ob, obj[ob].eventAct[Actions::forkActions]);
					return;
				}
			}
		}

		if (obj[ob].eventActChance[Actions::impasseActions] >= rand() % 100) {// если выпало действие в тупике
			cross = accros(ob);
			if (cross[0] < 2) {//если тупик
				if (obj[ob].eventAct[Actions::impasseActions] < 4) {//если действие двигаться
					for (int i = 1; i < cross[0] + 1; i++) {
						if (obj[ob].eventAct[Actions::impasseActions] == cross[i]) {//действие возможно
							obj[ob].Action = cross[i];
							return;
						}
					}
				}else {
					setAction(ob, obj[ob].eventAct[Actions::impasseActions]);
					return;
				}
			}
		}

		if (obj[ob].eventActChance[Actions::objectActions] >= rand() % 100) {// если выпало действие встреча с объектом
			objAcc = objAccros(ob);
			if (objAcc != -1) {//если встретился обЬект
				if (obj[ob].eventAct[Actions::objectActions] < 4) {//если действие двигаться
					cross = accros(ob);
					for (int i = 1; i < cross[0] + 1; i++) {
						if (obj[ob].eventAct[Actions::objectActions] == cross[i]) {//действие возможно
							obj[ob].Action = cross[i];
							return;
						}
					}
				}else {
					switch (obj[ob].eventAct[Actions::objectActions]) {
					case Actions::BACK:
						if (obj[objAcc].xy[0] < obj[ob].xy[0]) {
							setAction(ob, Actions::RIGHT); return;
						}
						if (obj[objAcc].xy[0] > obj[ob].xy[0]) {
							setAction(ob, Actions::LEFT); return;
						}
						if (obj[objAcc].xy[1] < obj[ob].xy[1]) {
							setAction(ob, Actions::DOWN); return;
						}
						if (obj[objAcc].xy[1] > obj[ob].xy[1]) {
							setAction(ob, Actions::UP); return;
						}
						break;
					case Actions::DESTROY:
							setAction(objAcc, Actions::DESTROY);
							setAction(ob, Actions::RANDOM);
							return;
							break;
					}
					setAction(ob, obj[ob].eventAct[Actions::objectActions]);
					return;
				}
			}
		}
		if (obj[ob].Action < 4) {
			if (80 >= rand() % 100) {
				setAction(ob, obj[ob].Action);
				return;
			}
		}
		cross = accros(ob);
		obj[ob].Action = cross[(rand() % cross[0]) + 1];
		if (cross[0] > 1 && rand() % 2 == 1) { obj[ob].Action = cross[(rand() % cross[0]) + 1]; }
}

unsigned int Controller::objAccros(unsigned int ob)
{
	for (int i = 0; i < ob; i++) {
		if (obj[i].GetXY(0)> obj[ob].GetXY(0) - 1 && obj[i].GetXY(0) > obj[ob].GetXY(0) + 1 && obj[i].GetXY(1) == obj[ob].GetXY(1))return i;
		if (obj[i].GetXY(1)> obj[ob].GetXY(1) - 1 && obj[i].GetXY(1) > obj[ob].GetXY(1) + 1 && obj[i].GetXY(0) == obj[ob].GetXY(0))return i;
	}
	for (int i = ob+1; i < obj.getCount(); i++) {
		if (obj[i].GetXY(0)> obj[ob].GetXY(0) - 1 && obj[i].GetXY(0) > obj[ob].GetXY(0) + 1 && obj[i].GetXY(1) == obj[ob].GetXY(1))return i;
		if (obj[i].GetXY(1)> obj[ob].GetXY(1) - 1 && obj[i].GetXY(1) > obj[ob].GetXY(1) + 1 && obj[i].GetXY(0) == obj[ob].GetXY(0))return i;
	}
	return -1;
}
