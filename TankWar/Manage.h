#pragma once
#include "Weapon.h"
#include <vector>
#include <conio.h>
#include <iostream>

//判断指定的按键是否按下
#define KEY_DOWN(key) ((GetAsyncKeyState(key)&0x8000)?1:0)

//宏定义地图上的元素
#define BORDER		0
#define BRICK		1
#define IRON		2
#define LAWN		3
#define RIVER		4
#define BLANCK		5
#define FLAG		6

//宏定义按键操作
#define FIRE		4
#define QUIT		6
#define BEGIN		7

using namespace std;

class Manage
{
private:
	vector<Tank*> tanks;
	vector<Bullet*> bullets;
	int map[40][40];
	int score;
	int killNumber;
	int flagNum;
	char tankShape[2][4][3][7] = {
		{
			{"  ■  ","■■■","■■■"},		//上
			{"  ■■","■■■","  ■■"},		//左
			{"■■  ","■■■","■■  "},		//右
			{"■■■","■■■","  ■  "}		//下
		},
		{
			{"  ■  ","■■■","■  ■"},		//上
			{"  ■■","■■  ","  ■■"},		//左
			{"■■  ","  ■■","■■  "},		//右
			{"■  ■","■■■","  ■  "}		//下
		}
	};
	
public:
	Manage();
	void run();

private:
	void init();
	void initMap();
	void writeLocation(COORD loc, const char* str, unsigned short color = 0x0f);
	void drawMap();
	void drawImage(COORD loc, int imgType);
	void drawNineFoldGrid(COORD loc);
	void preSetting(const char* title, short x, short y);
	void initTanks();
	void printStatusBar();
	void createBullet(Tank*& tank);
	void bulletCollision(Bullet*& bullet);
	int getMapImage(COORD loc);
	void setMapImage(COORD loc, int imgType);
	void bulletsManage();
	void tanksManage();
	void enemyAction();
	void moveTank(Tank*& tank, int dir);
	void clearTank(Tank*& tank);
	void drawTank(Tank*& tank);
	bool tankCollision(Tank*& tank, COORD loc);
	int getPlayerInput();
	void gameOver(bool win);
};

