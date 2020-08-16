#pragma once
#include <Windows.h>

//宏定义坦克、炮弹类型
#define PLAYER		0
#define ENEMY		1

//宏定义坦克、炮弹等级
#define PRIMARY		0
#define JUNIOR		1 //射速高
#define SENIOR		2 //射速高+打铁墙

//宏定义响应速度
#define LOW			120
#define FAST		80

//宏定义方向
#define UP			0
#define DOWN		3
#define LEFT		1
#define RIGHT		2

class Weapon
{
protected:
	int ID;
	COORD location;
	int direction;
	int speed;
	int blood;
	int rank;
	const int faction;

public:
	Weapon(int id, COORD loc, int dir, int spd, int bld, int rk, int fac);
	
	int getID() { return ID; }
	COORD getLocation() { return location; }
	void setLocation(COORD loc) { location.X = loc.X; location.Y = loc.Y; }
	int getDirection() { return direction; }
	void setDirection(int dir) { direction = dir; }
	int getSpeed() { return speed; }
	void setSpeed(int spd) { speed = spd; }
	int getBlood() { return blood; }
	void setBlood(int bld) { blood = bld; };
	int getRank() { return rank; }
	void setRank(int rk) { rank = rk; }
	int getFaction() { return faction; }
};

class Tank :public Weapon
{
public:
	Tank(int id, COORD loc, int dir, int spd, int bld, int rk, int fac);
	bool onBody(COORD loc);
};

class Bullet :public Weapon
{
public:
	Bullet(int id, COORD loc, int dir, int spd, int bld, int rk, int fac);
	void goAhead();
};