#include "Manage.h"

Manage::Manage()
{
	init();
}

//初始化
void Manage::init()
{
	score = 0;
	killNumber = 0;
	flagNum = 4;
	initMap();
	preSetting("Tank War by 171830503", 120, 40);
	initTanks();
}

void Manage::initMap()
{
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			if (i == 0 || j == 0 || i == 39 || j == 39)
			{
				map[i][j] = BORDER; //边界
			}
			else
			{
				map[i][j] = BLANCK; //空地
			}
		}
	}
	for (int i = 8; i <= 31; ++i)
	{
		for (int j = 4; j <= 35; ++j)
		{
			map[i][j] = LAWN; //草地
		}
	}
	for (int i = 8; i <= 31; ++i)
	{
		for (int j = 12; j <= 14; ++j)
		{
			map[i][j] = BLANCK;
			map[i][j + 13] = BLANCK;
		}
	}
	for (int i = 10; i <= 14; ++i)
	{
		for (int j = 6; j <= 9; ++j) //砖墙
		{
			map[i][j] = BRICK;
			map[i][j + 12] = BRICK;
			map[i][j + 24] = BRICK;
			map[i + 15][j] = BRICK;
			map[i + 15][j + 12] = BRICK;
			map[i + 15][j + 24] = BRICK;
		}
	}
	for (int i = 15; i <= 16; ++i) //铁墙
	{
		for (int j = 6; j <= 9; ++j)
		{
			map[i][j] = IRON;
			map[i][j + 12] = IRON;
			map[i][j + 24] = IRON;
			map[i + 8][j] = IRON;
			map[i + 8][j + 12] = IRON;
			map[i + 8][j + 24] = IRON;
		}
	}
	for (int i = 18; i <= 21; ++i)  //河流
	{
		for (int j = 1; j <= 38; ++j)
		{
			map[i][j] = RIVER;
		}
	}
	for (int i = 35; i <= 38; ++i)
	{
		for (int j = 18; j <= 21; ++j)
		{
			map[i][j] = BRICK; //基地砖墙
		}
	}

	//基地旗帜
	map[36][19] = FLAG;
	map[36][20] = FLAG;
	map[37][19] = FLAG;
	map[37][20] = FLAG;
}

void Manage::writeLocation(COORD loc, const char * str, unsigned short color)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); //获取句柄
	SetConsoleTextAttribute(output, color); //设置颜色
	loc.X *= 2; //设置坐标位置，控制台界面每个字符高是宽的两倍，宽字符占两个字符位置，所以乘以2
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc); //打印设置
	cout << str;//打印字符串
}

//画出地图
void Manage::drawMap()
{
	for (short i = 0; i < 40; i++)
		for (short j = 0; j < 40; j++)
		{
			drawImage({ j,i }, map[i][j]);
		}
}

//画出该坐标处的图案
void Manage::drawImage(COORD loc, int imgType)
{
	switch (imgType)
	{
	case BLANCK:writeLocation(loc, "■",0x00); break;
	case BRICK:writeLocation(loc, "■", 0x0c); break;
	case IRON:writeLocation(loc, "■", 0x07); break;
	case LAWN:writeLocation(loc, "※", 0x2a); break;
	case RIVER:writeLocation(loc, "≈", 0x13); break;
	case FLAG:writeLocation(loc, "■", 0x44); break;
	case BORDER:writeLocation(loc, "■", 0xee); break;
	}
}

//画九宫格，用于清除坦克
void Manage::drawNineFoldGrid(COORD loc)
{
	for (short x = loc.X - 1; x < loc.X + 2; ++x)
	{
		for (short y = loc.Y - 1; y < loc.Y + 2; ++y)
		{
			if (x >= 0 && x < 40 && y >= 0 && y < 40)
			{
				drawImage({ x, y }, map[y][x]);
			}
		}
	}
}

//预设值
void Manage::preSetting(const char* title, short x, short y)
{
	CONSOLE_CURSOR_INFO cursorInfo; //光标结构
	cursorInfo.bVisible = false; //设置光标不可见
	cursorInfo.dwSize = sizeof(cursorInfo); 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); //隐藏光标

	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0); //切换至英文输入法

	SetConsoleTitleA(title); //设置控制台标题

	HANDLE stdIn, stdOut; //获取句柄
	stdIn = GetStdHandle(STD_INPUT_HANDLE); 
	stdOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	
	COORD pos = GetLargestConsoleWindowSize(stdOut); //获取最大控制台的窗口大小
	COORD BufferSize = { pos.X + 1,pos.Y + 1 };
	
	SetConsoleScreenBufferSize(stdOut, BufferSize); //设置控制台缓冲区大小
	
	SMALL_RECT srctWindow = { 0,0,x,y }; //指定界面大小
	SetConsoleWindowInfo(stdOut, true, &srctWindow); 
	COORD Buffer = { x + 1,y + 1 }; 
	
	SetConsoleScreenBufferSize(stdOut, Buffer); //设置控制台缓冲区大小
}

//初始化坦克
void Manage::initTanks()
{
	Tank* playerTank = new Tank(0, { 16,37 }, UP, LOW, 100, PRIMARY, PLAYER);
	tanks.push_back(playerTank);
	Tank* enemyTank;
	for (short i = 1; i < 10; ++i)
	{
		enemyTank = new Tank(i, { (i - 1) * 4 + 3,2 }, DOWN, LOW, 100, SENIOR, ENEMY);
		tanks.push_back(enemyTank);
	}
}

//状态栏
void Manage::printStatusBar()
{
	char buffer[40];
	writeLocation({ 42, 1 }, "坦克类型:", 0x0E);
	writeLocation({ 44, 2 }, "青色:    普通型", 0x01);
	writeLocation({ 44, 3 }, "红色:    迅捷型", 0x0C);
	writeLocation({ 44, 4 }, "紫色:    破坏型", 0x0C);

	writeLocation({ 42, 6 }, "操作指南:", 0x0E);
	writeLocation({ 44, 7 }, "开始:    B", 0x0E);
	writeLocation({ 44, 8 }, "上:      W", 0x0A);
	writeLocation({ 44, 9 }, "下:      S", 0x0B);
	writeLocation({ 44, 10 }, "左:      A", 0x0C);
	writeLocation({ 44, 11 }, "右:      D", 0x0D);
	writeLocation({ 44, 12 }, "攻击:    J", 0x0E);
	writeLocation({ 44, 13 }, "退出:    Q", 0x0E);

	writeLocation({ 42, 16 }, "玩家状态:", 0x0E);
	writeLocation({ 44, 17 }, "血量:", 0x0E);
	sprintf_s(buffer, "%d", tanks[0]->getBlood());
	writeLocation({ 50, 17 }, buffer, 0x0B);
	writeLocation({ 44, 18 }, "速度:", 0x0E);
	sprintf_s(buffer, "%d", 200 - tanks[0]->getSpeed());
	writeLocation({ 50, 18 }, buffer, 0x0B);
	writeLocation({ 44, 19 }, "攻击力:", 0x0E);
	sprintf_s(buffer, "%d", tanks[0]->getRank() + 1);
	writeLocation({ 50, 19 }, buffer, 0x0B);
	writeLocation({ 44, 20 }, "杀敌数:", 0x0E);
	sprintf_s(buffer, "%d", killNumber);
	writeLocation({ 50, 20 }, buffer, 0x0B);
	writeLocation({ 44, 21 }, "得分:", 0x0E);
	sprintf_s(buffer, "%d", score);
	writeLocation({ 50, 21 }, buffer, 0x0B);
}

//创建炮弹
void Manage::createBullet(Tank *& tank)
{
	Bullet* bullet;
	int id = tank->getID();
	COORD loc = tank->getLocation();
	int dir = tank->getDirection();
	int spd = tank->getSpeed();
	int bld = 100;
	int rk = tank->getRank();
	int fac = tank->getFaction();
	bullet = new Bullet(id, loc, dir, spd, bld, rk, fac);
	bullets.push_back(bullet);
}

//炮弹碰撞检测
void Manage::bulletCollision(Bullet *& bullet)
{
	char buffer[40];
	COORD loc = bullet->getLocation();
	int imgType = getMapImage(loc);
	switch (imgType) //炮弹碰撞到地图元素
	{
	case RIVER:
	case LAWN:
	case BLANCK:
		break;
	case BRICK:
		bullet->setBlood(0);
		if (bullet->getFaction() == PLAYER)
		{
			score += 5; //更新分数
			sprintf_s(buffer, "%d", score);
			writeLocation({ 50, 21 }, buffer, 0x0B); //打印分数
		}
		setMapImage(loc, BLANCK);
		drawImage(loc, BLANCK);
		break;
	case IRON:
		if (bullet->getRank() == SENIOR)
		{
			setMapImage(loc, BLANCK);
			drawImage(loc, BLANCK);
			if (bullet->getFaction() == PLAYER)
			{
				score += 20;
				sprintf_s(buffer, "%d", score);
				writeLocation({ 50, 21 }, buffer, 0x0B);
			}
		}
		else
		{
			drawImage(loc, IRON);
		}
		bullet->setBlood(0);
		break;
	case BORDER:
		drawImage(loc, BORDER);
		bullet->setBlood(0);
		break;
	case FLAG:
		if (bullet->getFaction() == ENEMY)
		{
			bullet->setBlood(0);
			setMapImage(loc, BLANCK);
			drawImage(loc, BLANCK);
			flagNum--;
			if (flagNum == 0)
			{
				gameOver(false);
			}
		}
		break;
	default:
		break;
	}
	vector<Bullet*>::iterator pos;
	int fac = bullet->getFaction();
	for (pos = bullets.begin(); pos != bullets.end(); ++pos) //炮弹相互碰撞
	{
		if (fac != (*pos)->getFaction())
		{
			COORD loc1 = (*pos)->getLocation();
			COORD loc2 = bullet->getLocation();
			int dx = loc1.X - loc2.X;
			int dy = loc1.Y - loc2.Y;
			if (dx == 0 && dy == 0)
			{
				drawImage(loc, imgType);
				bullet->setBlood(0);
				(*pos)->setBlood(0);
				score += 10;
				sprintf_s(buffer, "%d", score);
				writeLocation({ 50, 21 }, buffer, 0x0B);
			}
			else if ((dx == 0 && abs(dy) == 1) || (dy == 0 && abs(dx) == 1))
			{
				drawImage(loc, imgType);
				drawImage((*pos)->getLocation(), imgType);
				bullet->setBlood(0);
				(*pos)->setBlood(0);
				score += 10;
				sprintf_s(buffer, "%d", score);
				writeLocation({ 50, 21 }, buffer, 0x0B);
			}
		}
	}
	if (fac == ENEMY) //炮弹碰撞到坦克
	{
		if (tanks[0]->onBody(loc))
		{
			bullet->setBlood(0);
			int bld = tanks[0]->getBlood();
			bld -= 20;
			tanks[0]->setBlood(bld);
			sprintf_s(buffer, "%d", bld);
			buffer[2] = ' ';
			buffer[3] = ' ';
			buffer[4] = '\0';
			writeLocation({ 50, 17 }, buffer, 0x0B);
		}
	}
	else
	{
		for (int i = 1; i <= 9; ++i)
		{
			if (tanks[i]->onBody(loc))
			{
				bullet->setBlood(0);
				int bld = tanks[i]->getBlood();
				bld -= 10;
				score += 10 * (tanks[i]->getRank() + 1);
				tanks[i]->setBlood(bld);
				sprintf_s(buffer, "%d", score);
				writeLocation({ 50, 21 }, buffer, 0x0B);
			}
		}
	}
}

//获取该坐标处地图图案
int Manage::getMapImage(COORD loc)
{
	return map[loc.Y][loc.X];
}

//设置该坐标处地图图案
void Manage::setMapImage(COORD loc, int imgType)
{
	map[loc.Y][loc.X] = imgType;
}

//子弹管理
void Manage::bulletsManage()
{
	vector<Bullet*>::iterator pos; //遍历子弹的迭代器
	for (pos = bullets.begin(); pos != bullets.end(); ++pos) 
	{
		COORD loc = (*pos)->getLocation();
		int imgType = getMapImage(loc);
		drawImage(loc, imgType); //清除子弹原位置图案
	}
	for (pos = bullets.begin(); pos != bullets.end();)
	{
		bulletCollision(*pos);
		if ((*pos)->getBlood() == 0)
		{
			pos = bullets.erase(pos); //清除死亡子弹
		}
		else
		{
			++pos;
		}
	}
	for (pos = bullets.begin(); pos != bullets.end(); ++pos)
	{
		COORD loc = (*pos)->getLocation();
		for (int i = 0; i <= 9; ++i)
		{
			if (tanks[i]->onBody(loc))
			{
				drawTank(tanks[i]);
			}
		}
		(*pos)->goAhead(); //子弹前进
		writeLocation((*pos)->getLocation(), "■", 0x06);
	}
}

//坦克管理
void Manage::tanksManage()
{
	char buffer[40];
	for (int i = 0; i <= 9; ++i)
	{
		COORD loc = tanks[i]->getLocation();
		if (loc.X < 45 && tanks[i]->getBlood() == 0)
		{
			if (i == 0)
			{
				gameOver(false); //败北
			}
			else
			{
				clearTank(tanks[i]);
				tanks[i]->setLocation({ 50,2 }); //死亡坦克坐标设置为战区之外
				killNumber++;
				if (killNumber == 6) //杀敌六人升级为SENIOR
				{
					tanks[0]->setRank(SENIOR);
					sprintf_s(buffer, "%d", tanks[0]->getRank() + 1);
					writeLocation({ 50, 19 }, buffer, 0x0B);
				}
				else if (killNumber == 3) //杀敌3人升级为JUNIOR
				{
					tanks[0]->setRank(JUNIOR);
					sprintf_s(buffer, "%d", 200 - tanks[0]->getSpeed());
					writeLocation({ 50, 18 }, buffer, 0x0B);
				}
				sprintf_s(buffer, "%d", killNumber);
				writeLocation({ 50, 20 }, buffer, 0x0B);
				if (killNumber == 9)
				{
					gameOver(true); //胜利
				}
			}
		}
	}
}

//敌方随机行动
void Manage::enemyAction()
{
	for (int i = 1; i <= 9; ++i)
	{
		if (tanks[i]->getLocation().X < 40)
		{
			bool fire = ((rand() * 29) % 30 < 4) ? true : false;
			
			if (fire) //开火
			{
				createBullet(tanks[i]);
			}
			else
			{
				bool move = ((rand() * 7) % 10 < 4) ? true : false;
				if (move) //移动
				{
					int dir;
					if (tanks[i]->getLocation().Y < 25)
					{
						dir = rand() % 9;
						if (dir >= 4)
						{
							dir = 3;
						}
					}
					else
					{
						dir = rand() % 4;
					}
					moveTank(tanks[i], dir);
				}
			}
		}
	}
}

//坦克移动
void Manage::moveTank(Tank *& tank, int dir)
{
	COORD loc = tank->getLocation();
	if (dir == FIRE) //开火
	{
		createBullet(tank);
	}
	else if (dir != tank->getDirection()) //方向和原方向一致，前进
	{
		clearTank(tank);
		tank->setDirection(dir);
		drawTank(tank);
	}
	else //方向和原方向不一致，调转方向
	{
		switch (dir)
		{
		case UP:loc.Y--; break;
		case DOWN:loc.Y++; break;
		case LEFT:loc.X--; break;
		case RIGHT:loc.X++; break;
		}
		if (!tankCollision(tank, loc))
		{
			clearTank(tank);
			tank->setLocation(loc);
			drawTank(tank);
		}
	}
}

//清除坦克
void Manage::clearTank(Tank *& tank)
{
	drawNineFoldGrid(tank->getLocation());
}

//画坦克
void Manage::drawTank(Tank *& tank)
{
	short color = 0x03; //不同等级颜色不同
	if (tank->getRank() == JUNIOR)
	{
		color = 0x04;
	}
	else if (tank->getRank() == SENIOR)
	{
		color = 0x0d;
	}
	short Y = tank->getLocation().Y;
	short X = tank->getLocation().X;
	for (short y = Y - 1, i = 0; y < Y + 2; ++y, ++i)
	{
		writeLocation({ X - 1,y }, tankShape[tank->getFaction()][tank->getDirection()][i], color);
	}
}

//获取输入
int Manage::getPlayerInput()
{
	if (KEY_DOWN('W') || KEY_DOWN('w'))
	{
		return UP;
	}
	if (KEY_DOWN('S') || KEY_DOWN('s'))
	{
		return DOWN;
	}
	if (KEY_DOWN('A') || KEY_DOWN('a'))
	{
		return LEFT;
	}
	if (KEY_DOWN('D') || KEY_DOWN('d'))
	{
		return RIGHT;
	}
	if (KEY_DOWN('J') || KEY_DOWN('j'))
	{
		return FIRE;
	}
	if (KEY_DOWN('Q') || KEY_DOWN('q'))
	{
		return QUIT;
	}
	if (KEY_DOWN('B') || KEY_DOWN('b'))
	{
		return BEGIN;
	}
	return -1;
}

//运行
void Manage::run()
{
	drawMap();
	drawTank(tanks[0]);
	printStatusBar();
	while (true)
	{
		int key = getPlayerInput();
		if (key == QUIT) //退出命令
		{
			exit(0);
		}
		if (key == BEGIN) //开始命令
		{
			for (int i = 1; i <= 9; ++i)
			{
				drawTank(tanks[i]); //画出敌方坦克
			}
			while (true)
			{
				key = getPlayerInput();
				if (key == QUIT)
				{
					exit(0);
				}
				if (0 <= key && key <= 4)
				{
					moveTank(tanks[0], key);
				}
				enemyAction();
				bulletsManage();
				tanksManage();
				Sleep(tanks[0]->getSpeed());
			}
		}
		Sleep(100);
	}
}

//游戏结束
void Manage::gameOver(bool win)
{
	char buffer[40];
	if (win)
	{
		writeLocation({ 42,24 }, "Ending: victory!", 0x0e);
	}
	else
	{
		writeLocation({ 42,24 }, "Ending: defeated!", 0x0e);
	}
	writeLocation({ 44, 26 }, "得分:", 0x0E);
	sprintf_s(buffer, "%d", score);
	writeLocation({ 50, 26 }, buffer, 0x0B);

	while (true)
	{
		int key = getPlayerInput();
		if (key == QUIT)
		{
			exit(0);
		}
		Sleep(100);
	}
}

//坦克碰撞检测
bool Manage::tankCollision(Tank *& tank, COORD loc)
{
	if (loc.X <= 1 || loc.X >= 38 || loc.Y <= 1 || loc.Y >= 38)
	{
		return true;
	}
	for (int i = 0; i < 10; ++i)
	{
		if (tank != tanks[i])
		{
			short dx = tanks[i]->getLocation().X - loc.X;
			short dy = tanks[i]->getLocation().Y - loc.Y;
			if (abs(dx) < 3 && abs(dy) < 3)
			{
				return true;
			}
		}
	}
	for (short x = loc.X - 1; x < loc.X + 2; ++x)
	{
		for (short y = loc.Y - 1; y < loc.Y + 2; ++y)
		{
			if (map[y][x] == BRICK || map[y][x] == IRON || map[y][x] == FLAG)
			{
				return true;
			}
		}
	}
	return false;
}
