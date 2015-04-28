#include "Map.h"


Map::Map()
{
	lvlList = { "lvl1" };
	lvlMap.clear();
	currentMap = 0;
	blockToDestroy = 0;
	dashLength = 0;
	dashX = -1;
	ballX = -1;
	ballY = -1;
	ballAng = 0;
	dx = 0;
	dy = -1;
	ball = NULL;
	block = NULL;
	dash = NULL;
	wall = NULL;
	readMap();
}

Map::Map(SDL_Texture* bball, SDL_Texture* ddash, SDL_Texture* bblock, SDL_Texture* wwall)
{
	lvlList = { "lvl1" };
	lvlMap.clear();
	currentMap = 0;
	blockToDestroy = 0;
	dashLength = 0;
	dashX = -1;
	ballX = -1;
	ballY = -1;
	ballAng = 0;
	dx = 0;
	dy = -1;
	readMap();
	ball = bball;
	dash = ddash;
	block = bblock;
	wall = wwall;
}


Map::~Map()
{
	SDL_DestroyTexture(ball);
	SDL_DestroyTexture(block);
	SDL_DestroyTexture(dash);
	SDL_DestroyTexture(wall);
}

void Map::readMap(){
	std::ifstream inFile;
	inFile.open(lvlList[currentMap] + ".txt");
	std::string readed;
	while (inFile >> readed){
		lvlMap.push_back(readed);
	}
	prepareMapBeforeUse();
}

void Map::prepareMapBeforeUse(){
	for (int i = 0; i < lvlMap.size(); i++){
		for (int j = 0; j < lvlMap[i].length(); j++){
			if (lvlMap[i][j] == '_')
				lvlMap[i][j] = ' ';
			if (lvlMap[i][j] == '='){
				if (dashX == -1)
					dashX = j;
				dashLength++;
			}
			if (lvlMap[i][j] == 'o'){
				ballX = j;
				ballY = i;
			}
			if (lvlMap[i][j] == 'x')
				blockToDestroy++;
		}
	}
}

std::string Map::draw(){
	std::string toReturn = "";
	for (int i = 0; i < lvlMap.size(); i++){
		toReturn.append(lvlMap[i] + "\n");
	}
	return toReturn;
}

bool Map::dashToLeft(){
	int y = lvlMap.size() - 1;
	if (lvlMap[y][dashX - 1] != '|'){
		lvlMap[y][dashX - 1] = '=';
		lvlMap[y][dashX + dashLength - 1] = ' ';
		dashX--;
		return true;
	}
	return false;
}

bool Map::dashToRight(){
	int y = lvlMap.size() - 1;
	int x = dashX + dashLength - 1;
	if (lvlMap[y][x + 1] != '|'){
		lvlMap[y][x + 1] = '=';
		lvlMap[y][dashX] = ' ';
		dashX++;
		return true;
	}
	return false;
}

int Map::moveBall(){
	int tmp = 0;
	if (bounceOnBrick(ballX + dx, ballY + dy))
		tmp = 100;
	bounceOnTop(ballX + dx, ballY + dy);
	bounceOnVertical(ballX + dx, ballY + dy);
	if (ballY + dy == (lvlMap.size() - 1) && !bounceOnDash(ballX + dx, ballY + dy)){
		// ball out of play field
		tmp = -50;
	}
	else{
		if (!bounceOnVertical(ballX + dx, ballY + dy)){
			lvlMap[ballY][ballX] = ' ';
			ballX += dx;
			ballY += dy;
			lvlMap[ballY][ballX] = 'o';
		}
	}
	ballAng = ++ballAng % 33;
	return tmp;
}

bool Map::bounceOnVertical(int x, int y){
	if (lvlMap[y][x] == '|'){
		dx *= (-1);
		return true;
	}
	return false;
}

bool Map::bounceOnTop(int x, int y){
	if (lvlMap[y][x] == '-'){
		dy *= (-1);
		return true;
	}
	else if (lvlMap[y][x] == '+'){
		dx *= (-1);
		dy *= (-1);
		return true;
	}
	return false;
}

bool Map::bounceOnDash(int x, int y){
	if (lvlMap[y][x] == '='){
		if (ballX + dx == dashX){
			dx = -1;
		}
		else if (ballX + dx == dashX + 1){
			dx = 0;
		}
		else if (ballX + dx == dashX + dashLength - 1){
			dx = 1;
		}
		dy *= (-1);
		return true;
	}
	return false;
}

bool Map::bounceOnBrick(int x, int y){
	if (lvlMap[y][x] == 'x'){
		if (lvlMap[y + (dy * (-1))][x] == ' '){
			dy *= (-1);
		}
		else if (lvlMap[y][x + (dx * (-1))] == ' '){
			dx *= (-1);
		}
		else{
			dx *= (-1);
			dy *= (-1);
		}
		lvlMap[y][x] = ' ';
		blockToDestroy--;
		return true;
	}
	return false;
}

void Map::drawGL(void(dispTexture)(SDL_Texture* img, SDL_Rect* srcRect, SDL_Rect* destRect)){
	SDL_Texture* tmp_texture = NULL;
	SDL_Rect srcRect = { 0, 0, 25, 25 };
	SDL_Rect dstRect = { 0, 0, 25, 25 };
	for (int i = 0; i < lvlMap.size(); i++)
		for (int j = 0; j < lvlMap[i].length(); j++){
			if (lvlMap[i][j] == 'o'){
				srcRect = { ballAng*25, 0, 25, 25 };
				tmp_texture = ball;
			}
			else if (lvlMap[i][j] == 'x'){
				srcRect = { 0, 0, 25, 25 };
				tmp_texture = block;
			}
			else if (lvlMap[i][j] == '='){
				srcRect = { 0, 0, 25, 25 };
				tmp_texture = dash;
			}
			else if (lvlMap[i][j] == '|' || lvlMap[i][j] == '-' || lvlMap[i][j] == '+'){
				srcRect = { 0, 0, 25, 25 };
				tmp_texture = wall;
			}
			else
				tmp_texture = NULL;
			if (tmp_texture != NULL){
				dstRect = { j*25, i*25, 25, 25};
				dispTexture(tmp_texture, &srcRect, &dstRect);
			}
		}
}

bool Map::isWin(){
	return blockToDestroy == 0 ? true : false;
}