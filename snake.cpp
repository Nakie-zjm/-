#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#undef UNICODE
#undef _UNICODE

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define Snake_size 500//蛇的最大长度
#define filePath "score.txt"//定义储存前10名的分数的文件路径

//音乐播放线程
DWORD WINAPI musicplay1(LPVOID Ipparaper);
DWORD WINAPI musicplay2(LPVOID Ipparaper);
DWORD WINAPI musicplay3(LPVOID Ipparaper);
DWORD WINAPI musicplay4(LPVOID Ipparaper);
DWORD WINAPI musicplay5(LPVOID Ipparaper);
DWORD WINAPI musicplay6(LPVOID Ipparaper);
DWORD WINAPI musicplay7(LPVOID Ipparaper);
DWORD WINAPI musicplay8(LPVOID Ipparaper);

void interfaceDraw();//进入游戏界面
void GameInit();//初始化游戏
void GameDraw();//图绘制
void snakeMove();//蛇移动
void snakeStop();//蛇不自己动
void keyboardControl();//键盘控制蛇方向
void eatFood();//蛇吃食物
int snakeDeath();//蛇死亡判定
void storeScore();//储存分数
int snakeHasDied();//蛇已经死亡后出现的菜单
int Register_Login();//注册登录功能

struct snake
{
	int size;//蛇的长度
	int r;//蛇的半径
	int speed;//蛇的速度
	int dir;//蛇的方向
	POINT coor[Snake_size];//蛇每节坐标
}snake;

enum DIR//蛇的方向
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

struct food
{
	int x;//食物横坐标
	int y;//食物纵坐标
	int r;//食物半径
	bool flag;//用于判断食物是否被吃
}food;

struct badfood
{
	int x;
	int y;
	int r;
	bool flag;
}badfood;

struct goldenfood
{
	int x;
	int y;
	int r;
	bool flag;
}goldenfood;

struct user
{
	char username[50];//用户名
	char password[50];//用户密码
	struct user* next;
};

int score=0;//分数
int snaketime=0;//时间
bool snakestop = true;//蛇是否动
bool conti = false;//是否继续游戏
bool invincible = false;//是否无敌
time_t snakeTime;//蛇运动时间
time_t startTime;//蛇开始时间
time_t currentTime;//当前时间
time_t invincibleStartTime;//吃无敌食物的时间

int main()
{
	//Register_Login();//登录注册
	interfaceDraw();//进入游戏界面绘制

	while (1)
	{
		GameInit();//初始化游戏
		GameDraw();//图绘制
		snakeStop();//不让蛇自己动起来
		while (1)
		{
			GameDraw();
			snakeMove();//蛇移动
			if (snakeDeath() == 1)
				break;
			keyboardControl();//键盘控制蛇的方向
			eatFood();//吃食物
			Sleep(snake.speed);//控制蛇的速度
		}
		storeScore();//显示最终得分并储存
		conti=snakeHasDied();
		if (!conti)
		{
			break;
		}
	}
		system("pause");
		return 0;
}

//开始游戏界面
void interfaceDraw()
{
	initgraph(1000, 600);
	setbkcolor(RGB(21, 44, 89));
	cleardevice();
	settextcolor(RED);
	settextstyle(70, 50, "行楷");
	outtextxy(180, 100, "贪吃蛇大作战");
	setbkcolor(WHITE);//再次设置背景颜色，使文字区域背景颜色为该次背景颜色
	setfillcolor(WHITE);//设置矩形背景填充颜色
	rectangle(350, 250, 550, 330);
	fillrectangle(350, 250, 550, 330);//需要先填充矩形颜色，否则矩形中文字不显示
	settextstyle(40, 20, "行楷");
	outtextxy(370, 270, "进入游戏");
	rectangle(350, 400, 550, 480);
	fillrectangle(350, 400, 550, 480);//需要先填充矩形颜色，否则矩形中文字不显示
	settextstyle(40, 20, "行楷");
	outtextxy(370, 420, "退出游戏");
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		if (m.x >= 350 && m.x <= 550 && m.y >= 250 && m.y <= 330 && m.uMsg == WM_LBUTTONDOWN)
		{
			closegraph();
			break;
		}
		if (m.x >= 350 && m.x <= 550 && m.y >= 400 && m.y <= 480 && m.uMsg == WM_LBUTTONDOWN)
		{
			closegraph();
			exit(0);
		}
	}
}

//初始化游戏
void GameInit()
{
	//游戏背景音乐
	mciSendString("open ./res/bgm.mp3", NULL, 0, NULL);
	mciSendString("play ./res/bgm.mp3 repeat", NULL, 0, NULL);
	score = 0;
	//初始化图形窗口
	initgraph(1000, 640);
	//初始化蛇
	snake.size = 3;
	snake.r = 5;
	snake.speed = 100;

	for (int i = 0; i <snake.size ; i++)
	{
	snake.coor[i].x = 100-10*i;
	snake.coor[i].y = 100;
	}

	//初始化食物
	srand(time(NULL));//随机产生食物坐标种子
	food.x = rand() % 950+10;
	food.y = rand() % 600+10;
	food.r = 8;
	food.flag = true;
	//初始化坏食物
	badfood.x = rand() % 950 + 10;
	badfood.y = rand() % 600 + 10;
	badfood.r = 8;
	badfood.flag = true;
	//初始化无敌食物
	goldenfood.x = rand() % 950 + 10;
	goldenfood.y = rand() % 600 + 10;
	goldenfood.r = 5;
	goldenfood.flag = true;
}

//图绘制
void GameDraw()
{
	BeginBatchDraw();//双缓冲绘图
	IMAGE head, headbk;
	loadimage(&head, "./res/head.jpg",15,15);
	loadimage(&headbk, "./res/headbk.jpg",15,15);
	setbkcolor(RGB(28, 115, 119));//设置背景颜色
	cleardevice();
	//绘制蛇
	setfillcolor(RED);
	for (int i = 0; i <snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, snake.r);
	}
	putimage(snake.coor[0].x-4 , snake.coor[0].y-4, 10, 10, &headbk, 0, 0, SRCAND);
	putimage(snake.coor[0].x-4 , snake.coor[0].y-4, 10, 10, &head, 0, 0, SRCPAINT);

	//绘制食物
	if (food.flag)
	{
		setfillcolor(RED);
		solidcircle(food.x, food.y, food.r);
	}
	//绘制badfood
	if (badfood.flag)
	{
		setfillcolor(BLACK);
		solidcircle(badfood.x, badfood.y, badfood.r);
	}
	//绘制无敌食物
	if (goldenfood.flag)
	{
		setfillcolor(RGB(246, 246, 123));
		solidcircle(goldenfood.x, goldenfood.y, goldenfood.r);
	}
	//实时显示分数
	char scorestr[20];
	sprintf_s(scorestr, "得分:%d", score);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(20, 10, _T("宋体"));
	outtextxy(850, 30, scorestr);
	//实时显示时间
	snaketime = snakeTime - startTime;
	char SnakeTime[20];
	sprintf_s(SnakeTime, "时间:%d", snaketime);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(20, 10, _T("宋体"));
	outtextxy(700, 30, SnakeTime);
	EndBatchDraw();
}

//蛇移动
void snakeMove()
{
	snakeTime = time(NULL);
	while (snakestop)
	{
		return;
	}
	for (int i = snake.size - 1; i > 0; i--)
		snake.coor[i] = snake.coor[i - 1];
	switch (snake.dir)
	{
		case RIGHT:
			snake.coor[0].x+=10;
			break;
		case LEFT:
			snake.coor[0].x-=10;
			break;
		case UP:
			snake.coor[0].y-=10;
			break;
		case DOWN:
			snake.coor[0].y+=10;
			break;
	}
}

//蛇刚开始不动
void snakeStop()
{
	while (1)
	{
		if (_kbhit())
		{
			int key = _getch();
			if (key == 'w' || key == 'W')
			{
				snake.dir = UP;
				snakestop = false;
				break;
			}
			else if(key=='d'||key=='D')
			{
				snake.dir = RIGHT;
				snakestop = false;
				break;
			}
			else if (key == 's' || key == 'S')
			{
				snake.dir = DOWN;
				snakestop = false;
				break;
			}
		}
	}
	startTime = time(NULL);//蛇开始动时间
}

//键盘控制蛇方向
void keyboardControl()
{
	if (_kbhit())//判断有没有键盘输入,有返回真值
	{
		switch (_getch())
		{
		case 'w':
		case 'W':
		case 72:if(snake.dir!=DOWN)
			snake.dir = UP; break;
		case 's':
		case 'S':
		case 80:
			if(snake.dir!=UP)
			snake.dir = DOWN; break;
		case 'a':
		case 'A':
		case 75:if(snake.dir!=RIGHT)
			snake.dir = LEFT; break;
		case 'd':
		case 'D':
		case 77:if(snake.dir!=LEFT)
			snake.dir = RIGHT; break;
		case ' '://游戏暂停
			CreateThread(NULL, NULL, musicplay1, NULL, NULL, NULL);//播放暂停音效

			while (1)
			{
				if (_getch() == ' ')
				{
					CreateThread(NULL, NULL, musicplay2, NULL, NULL, NULL);//播放继续游戏音效
					break;
				}

			}
			break;
		}
	}
}
DWORD WINAPI musicplay1(LPVOID Ipparamer)
{
	mciSendString("open ./res/TheWorld.mp3", NULL, 0, NULL);
	mciSendString("play ./res/TheWorld.mp3", NULL, 0, NULL);
	return 0;
}
DWORD WINAPI musicplay2(LPVOID Ipparamer)
{
	mciSendString("open ./res/TimeFlow.mp3", NULL, 0, NULL);
	mciSendString("play ./res/TimeFlow.mp3", NULL, 0, NULL);
	return 0;
}


//蛇吃食物
void eatFood()
{
	//吃正常食物
	if (food.flag && snake.coor[0].x >= food.x - food.r && snake.coor[0].x <= food.x + food.r 
		&& snake.coor[0].y >= food.y - food.r && snake.coor[0].y <= food.y + food.r)
	{
		CreateThread(NULL, NULL, musicplay3, NULL, NULL, NULL);//播放吃食物音效
		snake.size++;
		score += 5;
		snake.speed -= 3;
		food.flag = false;
	}
	if (!food.flag)//刷新食物
	{
		srand(time(NULL));//随机产生食物
		food.x = rand() % 950+10;
		food.y = rand() % 600+10;
		food.r = 8;
		food.flag = true;
	}
	//吃坏食物
	if (badfood.flag && snake.coor[0].x >= badfood.x - badfood.r && snake.coor[0].x <= badfood.x + badfood.r
		&& snake.coor[0].y >= badfood.y - badfood.r && snake.coor[0].y <= badfood.y + badfood.r)
	{
		CreateThread(NULL, NULL, musicplay4, NULL, NULL, NULL);//播放吃食物音效
		if(!invincible)
		snake.size--;
		badfood.flag = false;
	}
	if (badfood.x == food.x && badfood.y == food.y)//防止坏食物和正常食物生成在一起
	{
		badfood.flag = false;
	}
	if (!badfood.flag)//刷新坏食物
	{
		srand(time(NULL));//随机产生坏食物
		badfood.x = rand() % 950 + 10;
		badfood.y = rand() % 600 + 10;
		badfood.r = 8;
		badfood.flag = true;
	}

	// 获取当前时间
	currentTime = time(NULL);
	//吃无敌食物
	if (goldenfood.flag && snake.coor[0].x >= goldenfood.x - goldenfood.r && snake.coor[0].x <= goldenfood.x + goldenfood.r
		&& snake.coor[0].y >= goldenfood.y - goldenfood.r && snake.coor[0].y <= goldenfood.y + goldenfood.r)
	{
		CreateThread(NULL, NULL, musicplay5, NULL, NULL, NULL);//播放吃无敌食物音效
		goldenfood.flag = false;
		invincible = true;
		invincibleStartTime = time(NULL);//获取吃无敌食物时的时间
	}
	if (!goldenfood.flag)//刷新无敌食物
	{
		srand(time(NULL));//随机产生无敌食物
		goldenfood.x = rand() % 950 + 10;
		goldenfood.y = rand() % 600 + 10;
		goldenfood.r = 5;
		int i = rand() % 20;//无敌食物出现的概率为5%
		if (i == 1)
		goldenfood.flag = true;
	}
}
DWORD WINAPI musicplay3(LPVOID Ipparaper)
{
	mciSendString("open ./res/ye.mp3", NULL, 0, NULL);//吃食物音效
	mciSendString("play ./res/ye.mp3", NULL, 0, NULL);
	return 0;
}
DWORD WINAPI musicplay4(LPVOID Ipparaper)
{
	mciSendString("open ./res/qi.mp3", NULL, 0, NULL);//吃坏食物音效
	mciSendString("play ./res/qi.mp3", NULL, 0, NULL);
	return 0;
}
DWORD WINAPI musicplay5(LPVOID Ipparaper)
{
	mciSendString("open ./res/invincible.mp3", NULL, 0, NULL);//吃无敌食物音效
	mciSendString("play ./res/invincible.mp3", NULL, 0, NULL);
	return 0;
}

//判定蛇死亡
int snakeDeath()
{
	//判断无敌状态是否已经过期（时间差大于10秒）
	if (difftime(currentTime, invincibleStartTime) > 10)
		//无敌状态已经过期，需要设置为非无敌状态
		invincible = false;
	if (!invincible)
	{
		if (snake.coor[0].x <= 0 || snake.coor[0].x >= 1000 ||
			snake.coor[0].y <= 0 || snake.coor[0].y >= 640)
		{
			CreateThread(NULL, NULL, musicplay6, NULL, NULL, NULL);//播放结束游戏音效
			return 1;
		}

		for (int i = snake.size - 1; i > 0; i--)
		{
			if (snake.coor[0].x == snake.coor[i].x && snake.coor[0].y == snake.coor[i].y)
			{
				CreateThread(NULL, NULL, musicplay6, NULL, NULL, NULL);//播放结束游戏音效
				return 1;
			}
		}
	}
	if(invincible)
	{

		if (snake.coor[0].x <= 0)
			snake.coor[0].x = 1000;
		else if (snake.coor[0].x >= 1000)
			snake.coor[0].x = 0;
		if (snake.coor[0].y <= 0)
			snake.coor[0].y = 640;
		else if (snake.coor[0].y >= 640)
			snake.coor[0].y = 0;
	}
	return 0;
}
DWORD WINAPI musicplay6(LPVOID Ipparaper)
{
	mciSendString("open ./res/KoNodio.mp3", NULL, 0, NULL);//死亡音效
	mciSendString("play ./res/Konodio.mp3", NULL, 0, NULL);
	return 0;
}

//显示最终得分、储存最高10名得分
void storeScore()
{
	//显示最终得分
	char scorestr[20];
	sprintf_s(scorestr, "最终得分为:%d", score);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(60, 30, _T("宋体"));
	outtextxy(300, 175, scorestr);
	//储存分数
	int _score[10];
	FILE* fp;
	//从文件中读取分数储存在_score数组中
	errno_t err = fopen_s(&fp, filePath, "rb");
	if (err != 0)
	{
		fopen_s(&fp, filePath, "wb");//没有该文件就创建一个
		fclose(fp);
		exit(0);
	}
	fread(_score, sizeof(int), 10, fp);
	fclose(fp);
	//将score与_score中的分数对比，若能大于_score中分数，写入_socre
	for (int i = 0; i < 10; i++)
	{
		if (score > _score[i])
		{
			if (i < 9)
			{
				for (int j = i; j < 9; j++)
				{
					_score[j + 1] = _score[j];
					_score[i] = score;
				}
			}
			else
				_score[i] = score;
			break;
		}
	}

	if (score == _score[0])
	{
		CreateThread(NULL, NULL, musicplay7, NULL, NULL, NULL);
		MessageBox(NULL, "恭喜你达成新纪录", "得分", MB_OK);//弹出提示框
	}
	if (score == _score[1] || score == _score[2])
	{
		CreateThread(NULL, NULL, musicplay8, NULL, NULL, NULL);
		MessageBox(NULL, "恭喜进入前三", "得分", MB_OK);//弹出提示框
	}

	//将分数写入文件 
	fopen_s(&fp, filePath, "wb");
	if (fp)
	{
		fwrite(_score, sizeof(int), 10, fp);
		fclose(fp);
	}
}
DWORD WINAPI musicplay7(LPVOID Ipparaper)
{
	mciSendString("open ./res/High.mp3", NULL, 0, NULL);
	mciSendString("play ./res/High.mp3", NULL, 0, NULL);
	return 0;
}
DWORD WINAPI musicplay8(LPVOID Ipparaper)
{
	mciSendString("open ./res/abit.mp3", NULL, 0, NULL);
	mciSendString("play ./res/abit.mp3", NULL, 0, NULL);
	return 0;
}

//蛇死亡后显示菜单
int snakeHasDied()
{
	int _score[10];
	FILE* fp;
	//从文件中读取分数储存在_score数组中
	errno_t err = fopen_s(&fp, filePath, "rb");
	if (err != 0)
	{
		fopen_s(&fp, filePath, "wb");//没有该文件就创建一个
		fclose(fp);
		exit(0);
	}
	fread(_score, sizeof(int), 10, fp);
	fclose(fp);
	//将score与_score中的分数对比,排序输出
	for (int i = 0; i < 10; i++)
	{
		if (score > _score[i])
		{
			if (i < 9)
			{
				for (int j = i; j < 9; j++)
				{
					_score[j + 1] = _score[j];
					_score[i] = score;
				}
			}
			else
				_score[i] = score;
			break;
		}
	}

	setbkcolor(WHITE);//再次设置背景颜色，使文字区域背景颜色为该次背景颜色
	setfillcolor(WHITE);//设置矩形背景填充颜色
	rectangle(350, 275, 550, 355);
	fillrectangle(350, 275, 550, 355);//需要先填充矩形颜色，否则矩形中文字不显示
	settextcolor(RED);
	settextstyle(40, 20, "行楷");
	outtextxy(370, 295, "重新开始");

	rectangle(350, 395, 550, 475);
	fillrectangle(350, 395, 550, 475);//需要先填充矩形颜色，否则矩形中文字不显示
	settextstyle(40, 20, "行楷");
	outtextxy(390, 415, "排行榜");

	rectangle(350, 515, 550, 595);
	fillrectangle(350, 515, 550, 595);//需要先填充矩形颜色，否则矩形中文字不显示
	settextstyle(40, 20, "行楷");
	outtextxy(370, 535, "退出游戏");
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		if (m.x >= 350 && m.x <= 550 && m.y >= 275 && m.y <= 355 && m.uMsg == WM_LBUTTONDOWN)
		{
			conti = true;
			break;
		}
		if (m.x >= 350 && m.x <= 550 && m.y >= 395 && m.y <= 475 && m.uMsg == WM_LBUTTONDOWN)
		{
			closegraph();
			initgraph(200, 400);
			//显示排行榜
			char scoreStr[10][20];
			for (int i = 0; i < 10; i++)
			{
				sprintf_s(scoreStr[i], "第%d名:  %d", i + 1, _score[i]);
				setbkmode(TRANSPARENT);
				settextcolor(WHITE);
				settextstyle(20, 10, _T("宋体"));
				outtextxy(50, 20 + i * 25, scoreStr[i]);
			}
			setfillcolor(BLACK);//设置矩形背景填充颜色
			rectangle(75, 338, 145, 363);
			fillrectangle(75, 338, 145, 363);//需要先填充矩形颜色，否则矩形中文字不显示
			settextcolor(WHITE);
			settextstyle(20, 10, "行楷");
			outtextxy(90, 340, "继续");
			while (1)
			{
				m = GetMouseMsg();
				if (m.x >= 75 && m.x <= 145 && m.y >= 338 && m.y <= 363 && m.uMsg == WM_LBUTTONDOWN)
				{
					closegraph();
					conti = true;
					break;
				}
			}
			break;
		}
		if (m.x >= 350 && m.x <= 550 && m.y >= 515 && m.y <= 595 && m.uMsg == WM_LBUTTONDOWN)
		{
			closegraph();
			exit(0);
		}
	}
	return conti;
}

//登录、注册、注销
//将文件中的数据读入链表
struct user* readUserFile(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("打开文件失败\n");
		return NULL;
	}
	struct user* head = NULL;
	struct user* tail = NULL;
	char username[50];
	char password[50];
	while (fscanf(file, "%s %s", username, password) == 2)
	{
		struct user* newNode = (struct user*)malloc(sizeof(struct user));
		strcpy(newNode->username, username);
		strcpy(newNode->password, password);
		newNode->next = NULL;

		if (head == NULL)
		{
			head = newNode;
			tail = newNode;
		}
		else
		{
			tail->next = newNode;
			tail = newNode;
		}
	}
	fclose(file);
	file = NULL;
	return head;
}

//将链表中的数据保存在文件中
void writeUserFile(const char* filename, struct user* head)
{
	FILE* file = fopen(filename, "w");
	if (file == NULL)
	{
		printf("打开文件失败\n");
		return;
	}
	struct user* currentNode = head;
	for (currentNode; currentNode != NULL; currentNode = currentNode->next)
	{
		fprintf(file, "%s %s\n", currentNode->username, currentNode->password);
	}

	fclose(file);
	file = NULL;
}

//注册
struct user* registerUser(struct user* head)
{
	char username[50];
	char password[50];

	MessageBox(NULL, "请去控制台输入账号和密码", "注册", MB_OK);//弹出提示框
	printf("输入用户名");
	scanf("%s", username);
	//检查用户名是否已经存在
	struct user* currentNode = head;
	for (currentNode; currentNode != NULL; currentNode = currentNode->next)
	{
		if (strcmp(currentNode->username, username) == 0)
		{
			printf("用户名已经存在\n");
			return head;
		}
	}

	printf("输入密码");
	scanf("%s", password);

	struct user* newNode = (struct user*)malloc(sizeof(struct user));
	strcpy(newNode->username, username);
	strcpy(newNode->password, password);
	newNode->next = NULL;
	//将注册的用户名、密码存入链表中
	if (head == NULL)
	{
		head = newNode;
	}
	else
	{
		struct user* tail = head;
		while (tail->next != NULL)
		{
			tail = tail->next;
		}
		tail->next = newNode;
	}
	MessageBox(NULL, "注册成功", "注册", MB_OK);//弹出提示框
	printf("注册成功\n");
	return head;
}

//登录
struct user* loginUser(struct user* head)
{
	char username[20];
	char password[20];
	
	MessageBox(NULL, "请去控制台输入账号和密码", "登录", MB_OK);//弹出提示框
	printf("输入用户名");
	scanf("%s", username);
	//检查用户名是否正确
	struct user* currentUser = NULL;
	for (struct user* currentNode = head; currentNode != NULL; currentNode = currentNode->next)
	{
		if (strcmp(currentNode->username, username) == 0)
		{
			currentUser = currentNode;
			break;
		}
	}

	if (currentUser==NULL)
	{
		printf("用户名错误\n");
		return NULL;
	}

	printf("输入密码");
	scanf("%s", password);
	//检查密码是否正确
	if (strcmp(currentUser->password, password) != 0)
	{
		printf("密码错误\n");
		return NULL;
	}
	MessageBox(NULL, "登录成功", "登录", MB_OK);//弹出提示框
	printf("登录成功\n");
	return currentUser;
}

//注销当前账号
struct user* logoutUser(struct user* currentUser)
{
	int i;
	i = MessageBox(NULL, "注销成功", "注销", MB_OK);//弹出提示框
	printf("注销成功\n");
	return NULL;
}

//将注册函数、登录函数、注销函数使用户选择
int Register_Login()
{

	MOUSEMSG m;
	struct user* userList = readUserFile("users.txt");
	if (userList == NULL)
	{
		userList = (struct user*)malloc(sizeof(struct user));
		userList->next = NULL;
	}

	while (1)
	{
		Sleep(1000);
		initgraph(1000, 600);
		setbkcolor(RGB(21, 44, 89));
		cleardevice();
		settextcolor(RED);
		settextstyle(70, 50, "行楷");
		outtextxy(180, 100, "贪吃蛇大作战");
		setbkcolor(WHITE);//再次设置背景颜色，使文字区域背景颜色为该次背景颜色
		setfillcolor(WHITE);//设置矩形背景填充颜色
		rectangle(350, 250, 550, 330);
		fillrectangle(350, 250, 550, 330);//需要先填充矩形颜色，否则矩形中文字不显示
		settextstyle(40, 20, "行楷");
		outtextxy(400, 270, "登录");
		rectangle(350, 400, 550, 480);
		fillrectangle(350, 400, 550, 480);//需要先填充矩形颜色，否则矩形中文字不显示
		settextstyle(40, 20, "行楷");
		outtextxy(400, 420, "注册");

		while (1)
		{
			m = GetMouseMsg();
			if (m.x >= 350 && m.x <= 550 && m.y >= 250 && m.y <= 330 && m.uMsg == WM_LBUTTONDOWN)
			{
				Sleep(1000);
				closegraph();
				struct user* currentUser = loginUser(userList);
				if (currentUser != NULL)
				{
					while (1)
					{
						int choice;
						printf("\n1.注销\n2.进入游戏\n输入选择: ");
						scanf("%d", &choice);
						if (choice == 1)
						{
							userList = logoutUser(currentUser);
							break;
						}
						else if (choice == 2)
						{
							return 0;
						}
					}
				}
				else
				{
					break;
				}
				break;
			}
			if (m.x >= 350 && m.x <= 550 && m.y >= 400 && m.y <= 480 && m.uMsg == WM_LBUTTONDOWN)
			{
				Sleep(1000);
				closegraph();
				userList = registerUser(userList);
				writeUserFile("users.txt", userList);
				break;
			}
		}
	}
}