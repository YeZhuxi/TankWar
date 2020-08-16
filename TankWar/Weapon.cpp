#include "Weapon.h"

Weapon::Weapon(int id, COORD loc, int dir, int spd, int bld, int rk, int fac)
	:ID(id), location(loc), direction(dir), speed(spd), blood(bld), rank(rk), faction(fac) {}

Tank::Tank(int id, COORD loc, int dir, int spd, int bld, int rk, int fac)
	: Weapon(id, loc, dir, spd, bld, rk, fac) {}

bool Tank::onBody(COORD loc)
{
	short dx = location.X - loc.X;
	short dy = location.Y - loc.Y;
	if (abs(dx) < 3 && abs(dy) < 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Bullet::Bullet(int id, COORD loc, int dir, int spd, int bld, int rk, int fac)
	: Weapon(id, loc, dir, spd, bld, rk, fac) {}

void Bullet::goAhead()
{
	switch (direction)
	{
	case UP:location.Y--; break;
	case DOWN:location.Y++; break;
	case LEFT:location.X--; break;
	case RIGHT:location.X++; break;
	default:break;
	}
}
