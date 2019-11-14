#pragma once
#ifndef Maze_2235236457458678542434576535645
#define Maze_2235236457458678542434576535645

namespace fldType {
	const char EMPTY = 0;
	const char WALL = 1;
	const char OBJECT = 2;
};
class Maze {
public:
	Maze();
	~Maze();
	void SetSize(unsigned int widthF, unsigned int heightF);
	void SetField(unsigned int x, unsigned int y, unsigned char fldType);
	unsigned char GetField(unsigned int x, unsigned int y);
	unsigned int GetWidth();
	unsigned int GetHeight();
	static const char errors[2][30];
	void randomFill(unsigned int srand);
private:
	unsigned char* field;
	unsigned int widthF, heightF;

};

#endif
