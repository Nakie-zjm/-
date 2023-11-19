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

#define Snake_size 500//�ߵ���󳤶�
#define filePath "score.txt"//���崢��ǰ10���ķ������ļ�·��

//���ֲ����߳�
DWORD WINAPI musicplay1(LPVOID Ipparaper);
DWORD WINAPI musicplay2(LPVOID Ipparaper);
DWORD WINAPI musicplay3(LPVOID Ipparaper);
DWORD WINAPI musicplay4(LPVOID Ipparaper);
DWORD WINAPI musicplay5(LPVOID Ipparaper);
DWORD WINAPI musicplay6(LPVOID Ipparaper);
DWORD WINAPI musicplay7(LPVOID Ipparaper);
DWORD WINAPI musicplay8(LPVOID Ipparaper);

void interfaceDraw();//������Ϸ����
void GameInit();//��ʼ����Ϸ
void GameDraw();//ͼ����
void snakeMove();//���ƶ�
void snakeStop();//�߲��Լ���
void keyboardControl();//���̿����߷���
void eatFood();//�߳�ʳ��
int snakeDeath();//�������ж�
void storeScore();//�������
int snakeHasDied();//���Ѿ���������ֵĲ˵�
int Register_Login();//ע���¼����

struct snake
{
	int size;//�ߵĳ���
	int r;//�ߵİ뾶
	int speed;//�ߵ��ٶ�
	int dir;//�ߵķ���
	POINT coor[Snake_size];//��ÿ������
}snake;

enum DIR//�ߵķ���
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

struct food
{
	int x;//ʳ�������
	int y;//ʳ��������
	int r;//ʳ��뾶
	bool flag;//�����ж�ʳ���Ƿ񱻳�
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
	char username[50];//�û���
	char password[50];//�û�����
	struct user* next;
};

int score=0;//����
int snaketime=0;//ʱ��
bool snakestop = true;//���Ƿ�
bool conti = false;//�Ƿ������Ϸ
bool invincible = false;//�Ƿ��޵�
time_t snakeTime;//���˶�ʱ��
time_t startTime;//�߿�ʼʱ��
time_t currentTime;//��ǰʱ��
time_t invincibleStartTime;//���޵�ʳ���ʱ��

int main()
{
	//Register_Login();//��¼ע��
	interfaceDraw();//������Ϸ�������

	while (1)
	{
		GameInit();//��ʼ����Ϸ
		GameDraw();//ͼ����
		snakeStop();//�������Լ�������
		while (1)
		{
			GameDraw();
			snakeMove();//���ƶ�
			if (snakeDeath() == 1)
				break;
			keyboardControl();//���̿����ߵķ���
			eatFood();//��ʳ��
			Sleep(snake.speed);//�����ߵ��ٶ�
		}
		storeScore();//��ʾ���յ÷ֲ�����
		conti=snakeHasDied();
		if (!conti)
		{
			break;
		}
	}
		system("pause");
		return 0;
}

//��ʼ��Ϸ����
void interfaceDraw()
{
	initgraph(1000, 600);
	setbkcolor(RGB(21, 44, 89));
	cleardevice();
	settextcolor(RED);
	settextstyle(70, 50, "�п�");
	outtextxy(180, 100, "̰���ߴ���ս");
	setbkcolor(WHITE);//�ٴ����ñ�����ɫ��ʹ�������򱳾���ɫΪ�ôα�����ɫ
	setfillcolor(WHITE);//���þ��α��������ɫ
	rectangle(350, 250, 550, 330);
	fillrectangle(350, 250, 550, 330);//��Ҫ����������ɫ��������������ֲ���ʾ
	settextstyle(40, 20, "�п�");
	outtextxy(370, 270, "������Ϸ");
	rectangle(350, 400, 550, 480);
	fillrectangle(350, 400, 550, 480);//��Ҫ����������ɫ��������������ֲ���ʾ
	settextstyle(40, 20, "�п�");
	outtextxy(370, 420, "�˳���Ϸ");
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

//��ʼ����Ϸ
void GameInit()
{
	//��Ϸ��������
	mciSendString("open ./res/bgm.mp3", NULL, 0, NULL);
	mciSendString("play ./res/bgm.mp3 repeat", NULL, 0, NULL);
	score = 0;
	//��ʼ��ͼ�δ���
	initgraph(1000, 640);
	//��ʼ����
	snake.size = 3;
	snake.r = 5;
	snake.speed = 100;

	for (int i = 0; i <snake.size ; i++)
	{
	snake.coor[i].x = 100-10*i;
	snake.coor[i].y = 100;
	}

	//��ʼ��ʳ��
	srand(time(NULL));//�������ʳ����������
	food.x = rand() % 950+10;
	food.y = rand() % 600+10;
	food.r = 8;
	food.flag = true;
	//��ʼ����ʳ��
	badfood.x = rand() % 950 + 10;
	badfood.y = rand() % 600 + 10;
	badfood.r = 8;
	badfood.flag = true;
	//��ʼ���޵�ʳ��
	goldenfood.x = rand() % 950 + 10;
	goldenfood.y = rand() % 600 + 10;
	goldenfood.r = 5;
	goldenfood.flag = true;
}

//ͼ����
void GameDraw()
{
	BeginBatchDraw();//˫�����ͼ
	IMAGE head, headbk;
	loadimage(&head, "./res/head.jpg",15,15);
	loadimage(&headbk, "./res/headbk.jpg",15,15);
	setbkcolor(RGB(28, 115, 119));//���ñ�����ɫ
	cleardevice();
	//������
	setfillcolor(RED);
	for (int i = 0; i <snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, snake.r);
	}
	putimage(snake.coor[0].x-4 , snake.coor[0].y-4, 10, 10, &headbk, 0, 0, SRCAND);
	putimage(snake.coor[0].x-4 , snake.coor[0].y-4, 10, 10, &head, 0, 0, SRCPAINT);

	//����ʳ��
	if (food.flag)
	{
		setfillcolor(RED);
		solidcircle(food.x, food.y, food.r);
	}
	//����badfood
	if (badfood.flag)
	{
		setfillcolor(BLACK);
		solidcircle(badfood.x, badfood.y, badfood.r);
	}
	//�����޵�ʳ��
	if (goldenfood.flag)
	{
		setfillcolor(RGB(246, 246, 123));
		solidcircle(goldenfood.x, goldenfood.y, goldenfood.r);
	}
	//ʵʱ��ʾ����
	char scorestr[20];
	sprintf_s(scorestr, "�÷�:%d", score);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(20, 10, _T("����"));
	outtextxy(850, 30, scorestr);
	//ʵʱ��ʾʱ��
	snaketime = snakeTime - startTime;
	char SnakeTime[20];
	sprintf_s(SnakeTime, "ʱ��:%d", snaketime);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(20, 10, _T("����"));
	outtextxy(700, 30, SnakeTime);
	EndBatchDraw();
}

//���ƶ�
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

//�߸տ�ʼ����
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
	startTime = time(NULL);//�߿�ʼ��ʱ��
}

//���̿����߷���
void keyboardControl()
{
	if (_kbhit())//�ж���û�м�������,�з�����ֵ
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
		case ' '://��Ϸ��ͣ
			CreateThread(NULL, NULL, musicplay1, NULL, NULL, NULL);//������ͣ��Ч

			while (1)
			{
				if (_getch() == ' ')
				{
					CreateThread(NULL, NULL, musicplay2, NULL, NULL, NULL);//���ż�����Ϸ��Ч
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


//�߳�ʳ��
void eatFood()
{
	//������ʳ��
	if (food.flag && snake.coor[0].x >= food.x - food.r && snake.coor[0].x <= food.x + food.r 
		&& snake.coor[0].y >= food.y - food.r && snake.coor[0].y <= food.y + food.r)
	{
		CreateThread(NULL, NULL, musicplay3, NULL, NULL, NULL);//���ų�ʳ����Ч
		snake.size++;
		score += 5;
		snake.speed -= 3;
		food.flag = false;
	}
	if (!food.flag)//ˢ��ʳ��
	{
		srand(time(NULL));//�������ʳ��
		food.x = rand() % 950+10;
		food.y = rand() % 600+10;
		food.r = 8;
		food.flag = true;
	}
	//�Ի�ʳ��
	if (badfood.flag && snake.coor[0].x >= badfood.x - badfood.r && snake.coor[0].x <= badfood.x + badfood.r
		&& snake.coor[0].y >= badfood.y - badfood.r && snake.coor[0].y <= badfood.y + badfood.r)
	{
		CreateThread(NULL, NULL, musicplay4, NULL, NULL, NULL);//���ų�ʳ����Ч
		if(!invincible)
		snake.size--;
		badfood.flag = false;
	}
	if (badfood.x == food.x && badfood.y == food.y)//��ֹ��ʳ�������ʳ��������һ��
	{
		badfood.flag = false;
	}
	if (!badfood.flag)//ˢ�»�ʳ��
	{
		srand(time(NULL));//���������ʳ��
		badfood.x = rand() % 950 + 10;
		badfood.y = rand() % 600 + 10;
		badfood.r = 8;
		badfood.flag = true;
	}

	// ��ȡ��ǰʱ��
	currentTime = time(NULL);
	//���޵�ʳ��
	if (goldenfood.flag && snake.coor[0].x >= goldenfood.x - goldenfood.r && snake.coor[0].x <= goldenfood.x + goldenfood.r
		&& snake.coor[0].y >= goldenfood.y - goldenfood.r && snake.coor[0].y <= goldenfood.y + goldenfood.r)
	{
		CreateThread(NULL, NULL, musicplay5, NULL, NULL, NULL);//���ų��޵�ʳ����Ч
		goldenfood.flag = false;
		invincible = true;
		invincibleStartTime = time(NULL);//��ȡ���޵�ʳ��ʱ��ʱ��
	}
	if (!goldenfood.flag)//ˢ���޵�ʳ��
	{
		srand(time(NULL));//��������޵�ʳ��
		goldenfood.x = rand() % 950 + 10;
		goldenfood.y = rand() % 600 + 10;
		goldenfood.r = 5;
		int i = rand() % 20;//�޵�ʳ����ֵĸ���Ϊ5%
		if (i == 1)
		goldenfood.flag = true;
	}
}
DWORD WINAPI musicplay3(LPVOID Ipparaper)
{
	mciSendString("open ./res/ye.mp3", NULL, 0, NULL);//��ʳ����Ч
	mciSendString("play ./res/ye.mp3", NULL, 0, NULL);
	return 0;
}
DWORD WINAPI musicplay4(LPVOID Ipparaper)
{
	mciSendString("open ./res/qi.mp3", NULL, 0, NULL);//�Ի�ʳ����Ч
	mciSendString("play ./res/qi.mp3", NULL, 0, NULL);
	return 0;
}
DWORD WINAPI musicplay5(LPVOID Ipparaper)
{
	mciSendString("open ./res/invincible.mp3", NULL, 0, NULL);//���޵�ʳ����Ч
	mciSendString("play ./res/invincible.mp3", NULL, 0, NULL);
	return 0;
}

//�ж�������
int snakeDeath()
{
	//�ж��޵�״̬�Ƿ��Ѿ����ڣ�ʱ������10�룩
	if (difftime(currentTime, invincibleStartTime) > 10)
		//�޵�״̬�Ѿ����ڣ���Ҫ����Ϊ���޵�״̬
		invincible = false;
	if (!invincible)
	{
		if (snake.coor[0].x <= 0 || snake.coor[0].x >= 1000 ||
			snake.coor[0].y <= 0 || snake.coor[0].y >= 640)
		{
			CreateThread(NULL, NULL, musicplay6, NULL, NULL, NULL);//���Ž�����Ϸ��Ч
			return 1;
		}

		for (int i = snake.size - 1; i > 0; i--)
		{
			if (snake.coor[0].x == snake.coor[i].x && snake.coor[0].y == snake.coor[i].y)
			{
				CreateThread(NULL, NULL, musicplay6, NULL, NULL, NULL);//���Ž�����Ϸ��Ч
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
	mciSendString("open ./res/KoNodio.mp3", NULL, 0, NULL);//������Ч
	mciSendString("play ./res/Konodio.mp3", NULL, 0, NULL);
	return 0;
}

//��ʾ���յ÷֡��������10���÷�
void storeScore()
{
	//��ʾ���յ÷�
	char scorestr[20];
	sprintf_s(scorestr, "���յ÷�Ϊ:%d", score);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(60, 30, _T("����"));
	outtextxy(300, 175, scorestr);
	//�������
	int _score[10];
	FILE* fp;
	//���ļ��ж�ȡ����������_score������
	errno_t err = fopen_s(&fp, filePath, "rb");
	if (err != 0)
	{
		fopen_s(&fp, filePath, "wb");//û�и��ļ��ʹ���һ��
		fclose(fp);
		exit(0);
	}
	fread(_score, sizeof(int), 10, fp);
	fclose(fp);
	//��score��_score�еķ����Աȣ����ܴ���_score�з�����д��_socre
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
		MessageBox(NULL, "��ϲ�����¼�¼", "�÷�", MB_OK);//������ʾ��
	}
	if (score == _score[1] || score == _score[2])
	{
		CreateThread(NULL, NULL, musicplay8, NULL, NULL, NULL);
		MessageBox(NULL, "��ϲ����ǰ��", "�÷�", MB_OK);//������ʾ��
	}

	//������д���ļ� 
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

//����������ʾ�˵�
int snakeHasDied()
{
	int _score[10];
	FILE* fp;
	//���ļ��ж�ȡ����������_score������
	errno_t err = fopen_s(&fp, filePath, "rb");
	if (err != 0)
	{
		fopen_s(&fp, filePath, "wb");//û�и��ļ��ʹ���һ��
		fclose(fp);
		exit(0);
	}
	fread(_score, sizeof(int), 10, fp);
	fclose(fp);
	//��score��_score�еķ����Ա�,�������
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

	setbkcolor(WHITE);//�ٴ����ñ�����ɫ��ʹ�������򱳾���ɫΪ�ôα�����ɫ
	setfillcolor(WHITE);//���þ��α��������ɫ
	rectangle(350, 275, 550, 355);
	fillrectangle(350, 275, 550, 355);//��Ҫ����������ɫ��������������ֲ���ʾ
	settextcolor(RED);
	settextstyle(40, 20, "�п�");
	outtextxy(370, 295, "���¿�ʼ");

	rectangle(350, 395, 550, 475);
	fillrectangle(350, 395, 550, 475);//��Ҫ����������ɫ��������������ֲ���ʾ
	settextstyle(40, 20, "�п�");
	outtextxy(390, 415, "���а�");

	rectangle(350, 515, 550, 595);
	fillrectangle(350, 515, 550, 595);//��Ҫ����������ɫ��������������ֲ���ʾ
	settextstyle(40, 20, "�п�");
	outtextxy(370, 535, "�˳���Ϸ");
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
			//��ʾ���а�
			char scoreStr[10][20];
			for (int i = 0; i < 10; i++)
			{
				sprintf_s(scoreStr[i], "��%d��:  %d", i + 1, _score[i]);
				setbkmode(TRANSPARENT);
				settextcolor(WHITE);
				settextstyle(20, 10, _T("����"));
				outtextxy(50, 20 + i * 25, scoreStr[i]);
			}
			setfillcolor(BLACK);//���þ��α��������ɫ
			rectangle(75, 338, 145, 363);
			fillrectangle(75, 338, 145, 363);//��Ҫ����������ɫ��������������ֲ���ʾ
			settextcolor(WHITE);
			settextstyle(20, 10, "�п�");
			outtextxy(90, 340, "����");
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

//��¼��ע�ᡢע��
//���ļ��е����ݶ�������
struct user* readUserFile(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("���ļ�ʧ��\n");
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

//�������е����ݱ������ļ���
void writeUserFile(const char* filename, struct user* head)
{
	FILE* file = fopen(filename, "w");
	if (file == NULL)
	{
		printf("���ļ�ʧ��\n");
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

//ע��
struct user* registerUser(struct user* head)
{
	char username[50];
	char password[50];

	MessageBox(NULL, "��ȥ����̨�����˺ź�����", "ע��", MB_OK);//������ʾ��
	printf("�����û���");
	scanf("%s", username);
	//����û����Ƿ��Ѿ�����
	struct user* currentNode = head;
	for (currentNode; currentNode != NULL; currentNode = currentNode->next)
	{
		if (strcmp(currentNode->username, username) == 0)
		{
			printf("�û����Ѿ�����\n");
			return head;
		}
	}

	printf("��������");
	scanf("%s", password);

	struct user* newNode = (struct user*)malloc(sizeof(struct user));
	strcpy(newNode->username, username);
	strcpy(newNode->password, password);
	newNode->next = NULL;
	//��ע����û������������������
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
	MessageBox(NULL, "ע��ɹ�", "ע��", MB_OK);//������ʾ��
	printf("ע��ɹ�\n");
	return head;
}

//��¼
struct user* loginUser(struct user* head)
{
	char username[20];
	char password[20];
	
	MessageBox(NULL, "��ȥ����̨�����˺ź�����", "��¼", MB_OK);//������ʾ��
	printf("�����û���");
	scanf("%s", username);
	//����û����Ƿ���ȷ
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
		printf("�û�������\n");
		return NULL;
	}

	printf("��������");
	scanf("%s", password);
	//��������Ƿ���ȷ
	if (strcmp(currentUser->password, password) != 0)
	{
		printf("�������\n");
		return NULL;
	}
	MessageBox(NULL, "��¼�ɹ�", "��¼", MB_OK);//������ʾ��
	printf("��¼�ɹ�\n");
	return currentUser;
}

//ע����ǰ�˺�
struct user* logoutUser(struct user* currentUser)
{
	int i;
	i = MessageBox(NULL, "ע���ɹ�", "ע��", MB_OK);//������ʾ��
	printf("ע���ɹ�\n");
	return NULL;
}

//��ע�ắ������¼������ע������ʹ�û�ѡ��
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
		settextstyle(70, 50, "�п�");
		outtextxy(180, 100, "̰���ߴ���ս");
		setbkcolor(WHITE);//�ٴ����ñ�����ɫ��ʹ�������򱳾���ɫΪ�ôα�����ɫ
		setfillcolor(WHITE);//���þ��α��������ɫ
		rectangle(350, 250, 550, 330);
		fillrectangle(350, 250, 550, 330);//��Ҫ����������ɫ��������������ֲ���ʾ
		settextstyle(40, 20, "�п�");
		outtextxy(400, 270, "��¼");
		rectangle(350, 400, 550, 480);
		fillrectangle(350, 400, 550, 480);//��Ҫ����������ɫ��������������ֲ���ʾ
		settextstyle(40, 20, "�п�");
		outtextxy(400, 420, "ע��");

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
						printf("\n1.ע��\n2.������Ϸ\n����ѡ��: ");
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