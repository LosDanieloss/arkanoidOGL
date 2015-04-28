#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <SDL.h>

class Map
{
private:
	SDL_Texture* ball;
	SDL_Texture* block;
	SDL_Texture* dash;
	SDL_Texture* wall;
	std::vector<std::string> lvlList;
	std::vector<std::string> lvlMap;
	int currentMap;
	int blockToDestroy;
	int dashX, dashLength;
	int ballX, ballY, ballAng;
	int dx, dy; // change of x & y of our ball 'o'

	void readMap();
	// change _ to :space:, count blockToDestroy, find startin' place of dash and count it length, find coordinates of ball
	void prepareMapBeforeUse();

	bool bounceOnVertical(int x, int y);
	bool bounceOnTop(int x, int y);
	bool bounceOnDash(int x, int y);
	bool bounceOnBrick(int x, int y);

public:
	Map();
	Map(SDL_Texture* bball, SDL_Texture* ddash, SDL_Texture* bblock, SDL_Texture* wwall);
	~Map();
	std::string draw();
	void drawGL(void(dispTexture)(SDL_Texture* img, SDL_Rect* srcRect, SDL_Rect* destRect));
	bool dashToLeft();
	bool dashToRight();
	int moveBall();
	bool isWin();
};

