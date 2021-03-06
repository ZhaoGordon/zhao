#include "stdafx.h"
#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>
#include<iostream>
#include<ctime>
#include<mmsystem.h>
#pragma comment(lib,"Winmm.lib")

#define ROW 25
#define COL 50

int arr[1111][1111] = { 0 };
int boardx, boardy;
int ballx, bally;

int velocity_x = 1;
int velocity_y = -1;
bool starting;
bool iscatch = 0;

void CreatBrick(int num, int *t, int *m);//砖块制作
void initGame();//小球与木板的初始参数及绘制
void BoardMove();//板的运动
void BallMove(int *X, int *Y, int len);//球的运动

void move_Treasure();   //移动宝石
void get_Treasure();    //获得宝石
void time_Treasure();   //宝石失效

clock_t treasureLast, treasureNow;   //宝物下落过程
clock_t treaTime_Last, treaTime_Now; //宝物持续时间
int intTreasure = 0;  //宝石数
int getTreasure = 0;  //获得宝石
int length = 80;
int speed = 2;
int slong = 2;
struct TREASURE //宝石
{
	int x;
	int y;
	int type;
}Treasure;

void maininterface()//主界面
{
	void color(int a);
	void gotoxy(int x, int y);

	static const char *game[] = { "☆Game Help:","☆Press enter and 2,you can make your own maps.","☆Press enter and 3-7,you can choose one map. ","☆Be careful of the tools,they might be harmful" };
	color(4);
	gotoxy(35, 5);
	printf(" ------------------------");
	gotoxy(35, 6);
	printf("┃%2s%s%2s┃", "", "★Bricks Breaking★", "");
	gotoxy(35, 7);
	printf(" ------------------------");

	color(2);
	gotoxy(0, 10);
	printf("%2s%s%2s", "", game[0], "");

	color(3);
	gotoxy(10, 14);
	printf("%2s%s%2s", "", game[1], "");

	color(1);
	gotoxy(10, 18);
	printf("%2s%s%2s", "", game[2], "");

	color(5);
	gotoxy(10, 22);
	printf("%2s%s%2s", "", game[3], "");
	getchar();
}
void gotoxy(int x, int y)//光标位置函数  
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void color(int a)//颜色函数  
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
}
int main()//主函数
{
START:
	PlaySound("d:\\360Downloads\\sounds.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	maininterface();
	int number, t[10], m[10];//用于选择地图
	scanf("%d", &number);//选择地图
	initgraph(550, 600);//显示面大小，宽550，长600.
	initGame();
	if (number == 2)
	{
		for (int p = 0; p<10; p++) {
			scanf("%d%d", &t[p], &m[p]);
			solidrectangle(m[p] * 22 - 22, t[p] * 12 - 12, m[p] * 22 - 2, t[p] * 12 - 2);
		}
	}
	while (1)
	{
		CreatBrick(number, t, m);
		if (_kbhit())
		{
			BoardMove();
		}
		if (intTreasure == 1)
		{
			move_Treasure();
		}
		if (getTreasure == 1)
		{
			treaTime_Now = clock();
			if (treaTime_Now - treaTime_Last >21.1F * CLOCKS_PER_SEC)
			{
				time_Treasure();
				getTreasure = 0;
			}

		}
		switch (speed)
		{
		case 2:
			Sleep(2);//小球运动速度
			break;
		case 1:
			Sleep(1.8);
			break;
		case 3:
			Sleep(3);
			break;
		}
		switch (slong)
		{
		case 1:
			length = 40;
			break;
		case 2:
			length = 80;
			break;
		case 3:
			length = 160;
			break;
		}

		if (bally > 590)
		{
			int RE = MessageBox(NULL, TEXT("You Lose!"), TEXT("Bricks Breaking"), MB_OKCANCEL | MB_ICONQUESTION);
			if (RE == IDYES)
			{
				MessageBox(NULL, TEXT("You have done well!"), TEXT("Try again!"), MB_OK);
			}
			else
			{
				MessageBox(NULL, TEXT("You have done well!"), TEXT("Try again!"), MB_OK);
			}
			closegraph();
			goto START;
			return 0;
		}
	}
	_getch();
	//system("pause");
	return 0;
}

void initGame()//小球与木板的初始参数及绘制
{
	boardx = 250;//木板初始位置
	boardy = 590;

	ballx = 290;//小球初始位置
	bally = 580;

	setfillcolor(BLUE);
	solidrectangle(boardx, boardy, boardx + length, boardy + 10);//木板宽80，高10；

	setfillcolor(WHITE);
	solidcircle(ballx, bally, 10);//球半径10
}

void CreatBrick(int num, int *t, int *m)//绘制砖块地图共七种模式以及地图
{
	switch (num)//对地图进行选择
	{
	case 2://在界面中自己绘制想要的地图
	{
		int X2[10]; int Y2[10];
		int n = 0;
		setfillcolor(BLUE);
		for (int tt = 0; tt<10; tt++) {
			for (int i = t[tt] - 1; i<t[tt]; i++)
			{
				for (int j = m[tt] - 1; j<m[tt]; j++)
				{
					int x = j * 22;
					int y = i * 12;
					if (arr[i][j] == 0)
						solidrectangle(x, y, x + 20, y + 10);
					X2[n] = x; Y2[n] = y;
					n++;
				}
			}
		}BallMove(X2, Y2, 10);
	}break;

	case 3://固定地图2
	{
		int X3[152], Y3[152];
		setfillcolor(BLUE);
		for (int i = 0; i<16; i = i + 3)
		{
			for (int j = 0; j<20; j = j + 3)
			{
				int x = (j + 2) * 22;
				int y = i * 12;
				X3[j / 3 + 7 * i / 3] = x; Y3[j / 3 + 7 * i / 3] = y;
			}
		}
		for (int a = 1; a<16; a = a + 2)
		{
			for (int b = 1; b<20; b = b + 2)
			{
				int x = (b + 2) * 22;
				int y = a * 12;
				X3[41 + (b + 1) / 2 + (a - 1) / 2 * 10] = x; Y3[41 + (b + 1) / 2 + (a - 1) / 2 * 10] = y;
			}
		}
		for (int c = 2; c<16; c = c + 3)
		{
			for (int d = 2; d<20; d = d + 3)
			{
				int x = (d + 2) * 22;
				int y = c * 12;
				X3[121 + (d + 1) / 3 + (c - 2) / 3 * 6] = x; Y3[121 + (d + 1) / 3 + (c - 2) / 3 * 6] = y;
			}
		}//将砖块坐标以数组元素形式进行储存
		setlinecolor(BLACK);
		for (int tt = 0; tt<152; tt++)
		{
			if (arr[tt][tt] == 0)
				solidrectangle(X3[tt], Y3[tt], X3[tt] + 20, Y3[tt] + 10);
		}//绘制地图
		BallMove(X3, Y3, 152);//相应地图下小球的运动
	}break;

	case 4://固定地图3
	{
		setfillcolor(RED);
		int X4[168], Y4[168];
		for (int q = 0, i = 5; i<15; i++, q++)
		{
			for (int j = 2 + q; j<22 - q; j++)
			{
				int x = j * 22;
				int y = i * 12;
				X4[j - 2 + (20 - q)*q] = x;
				Y4[j - 2 + (20 - q)*q] = y;
			}
		}

		for (int w = 0, a = 1; a<4; a++, w++)
		{
			for (int b = 5 - 2 * w; b<8 + w; b++)
			{
				int x = b * 22;
				int y = a * 12;
				X4[102 + b + (3 + 3 * a)*a / 2] = x;
				Y4[102 + b + (3 + 3 * a)*a / 2] = y;
			}
		}
		for (int ww = 0, aa = 1; aa<4; aa++, ww++)
		{
			for (int bb = 18 - 2 * ww; bb<21 + ww; bb++)
			{
				int x = bb * 22;
				int y = aa * 12;
				X4[107 + bb + (3 + 3 * aa)*aa / 2] = x;
				Y4[107 + bb + (3 + 3 * aa)*aa / 2] = y;
			}
		}
		for (int c = 1; c<23; c++)
		{

			int x = c * 22;
			int y = 48;
			X4[145 + c] = x;
			Y4[145 + c] = y;
		}//将砖块坐标以数组元素形式进行储存		
		for (int tt = 0; tt<168; tt++)
		{
			if (arr[tt][tt] == 0)
				solidrectangle(X4[tt], Y4[tt], X4[tt] + 20, Y4[tt] + 10);//绘制地图
		}	BallMove(X4, Y4, 168);//相应地图下小球的运动情况
	}break;

	case 5://固定地图4--“昊”
	{
		int X5[98], Y5[98];
		setfillcolor(RED);
		for (int i = 1; i<11; i = i + 3)
		{
			for (int j = 2; j<14; j++)
			{
				int x = (2 + j) * 22;
				int y = i * 12;
				X5[j - 2 + 12 * (i - 1) / 3] = x;
				Y5[j - 2 + 12 * (i - 1) / 3] = y;
			}
		}
		for (int d = 2, mm = 0; d<15; d = d + 11, mm++)
		{
			for (int c = 1; c<8; c++)
			{
				int x = (d + 2) * 22;
				int y = c * 12;
				X5[47 + c + 7 * mm] = x;
				Y5[47 + c + 7 * mm] = y;
			}
		}
		for (int w = 11; w<15; w++)
		{
			int y = w * 12;
			int x = 9 * 22;
			X5[51 + w] = x;
			Y5[51 + w] = y;
		}
		for (int q = 0; q<16; q++)
		{
			int x = (q + 2) * 22;
			int y = 180;
			X5[66 + q] = x;
			Y5[66 + q] = y;
		}
		for (int a = 16; a<23; a++)
		{
			int b;
			b = a - 8;
			int x = (b + 2) * 22;
			int y = a * 12;
			X5[66 + a] = x;
			Y5[66 + a] = y;
		}
		for (int m = 16; m<24; m++)
		{
			int n;
			n = 22 - m;
			int x = (n + 2) * 22;
			int y = m * 12;
			X5[74 + m] = x;
			Y5[74 + m] = y;
		}//将砖块坐标以数组元素形式进行储存
		for (int tt = 0; tt<98; tt++)
		{
			if (arr[tt][tt] == 0)
				solidrectangle(X5[tt], Y5[tt], X5[tt] + 20, Y5[tt] + 10);//绘制地图
		}BallMove(X5, Y5, 98);//相应地图下小球的运动情况
	}break;

	case 6://固定地图5--“AT”
	{
		int	X6[28], Y6[28];
		setfillcolor(GREEN);
		for (int q = 1; q<12; q = q + 2)
		{
			int w;
			w = q / 2 + 6;
			int x = (w + 2) * 22;
			int y = q * 12;
			X6[(q - 1) / 2] = x;
			Y6[(q - 1) / 2] = y;
		}
		for (int w = 1; w<12; w = w + 2)
		{
			int e;
			e = 6 - w / 2;
			int x = (e + 2) * 22;
			int y = w * 12;
			X6[5 + (w + 1) / 2] = x;
			Y6[5 + (w + 1) / 2] = y;
		}
		for (int z = 3; z<10; z = z + 2)
		{
			int x = (z + 2) * 22;
			int y = 7 * 12;
			X6[11 + (z - 1) / 2] = x;
			Y6[11 + (z - 1) / 2] = y;
		}//"A"

		setfillcolor(RED);
		for (int a = 13; a<25; a = a + 2)
		{
			int x = (a + 2) * 22;
			int y = 24;
			X6[15 + (a - 11) / 2] = x;
			Y6[15 + (a - 11) / 2] = y;
		}
		for (int b = 2; b<13; b = b + 2)
		{
			int y = (b + 1) * 12;
			int x = 418;
			X6[21 + b / 2] = x;
			Y6[21 + b / 2] = y;
		}//“T”
		 //将砖块坐标以数组元素形式进行储存
		for (int tt = 0; tt<28; tt++)
		{
			if (arr[tt][tt] == 0)
				solidrectangle(X6[tt], Y6[tt], X6[tt] + 20, Y6[tt] + 10);//绘制地图
		}BallMove(X6, Y6, 28);//相应地图下小球的运动情况

	}break;

	case 7://固定地图6--围墙型
	{
		int X7[127], Y7[127];
		setfillcolor(BLUE);
		for (int i = 1; i<27; i = i + 18)
		{
			for (int j = 3; j<18; j++)
			{
				int x = (j + 2) * 22;
				int y = i * 12;
				X7[94 + j + (i - 1) / 18 * 15] = x;
				Y7[94 + j + (i - 1) / 18 * 15] = y;
			}
		}
		for (int b = 3, m = 0; b<18; b = b + 14, m++)
		{
			for (int a = 1; a<20; a++)
			{
				int x = (b + 2) * 22;
				int y = a * 12;
				X7[a - 1 + 19 * m] = x;
				Y7[a - 1 + 19 * m] = y;
			}
		}
		for (int q = 3; q<20; q = q + 3)
		{
			for (int w = 6; w<16; w = w + 2)
			{
				int x = (w + 2) * 22;
				int y = q * 12;
				X7[37 + (w - 4) / 2 + 5 * (q - 3) / 3] = x;
				Y7[37 + (w - 4) / 2 + 5 * (q - 3) / 3] = y;
			}
		}
		for (int c = 4; c<18; c = c + 5)
		{
			for (int d = 6; d<16; d++)
			{
				int x = (d + 2) * 22;
				int y = c * 12;
				X7[62 + d + 10 * (c - 4) / 5] = x;
				Y7[62 + d + 10 * (c - 4) / 5] = y;
			}
		}//将砖块坐标以数组元素形式进行储存
		for (int tt = 0; tt<127; tt++)
		{
			if (arr[tt][tt] == 0)
				solidrectangle(X7[tt], Y7[tt], X7[tt] + 20, Y7[tt] + 8);//绘制地图
		}BallMove(X7, Y7, 127);//相应地图下小球的运动情况

	}break;
	}
}
void BoardMove()//关于木板运动的函数
{
	setfillcolor(BLACK);
	solidrectangle(boardx, boardy, boardx + length, boardy + 10);
	char c = _getch();
	switch (c)
	{
	case 75:
	case'A':
	case'a':
		boardx -= 15;
		break;
	case 77:
	case 'd':
	case 'D':
		boardx += 15;
		break;
	default:
		break;
	}
	if (boardx <= 0)
		boardx = 0;
	if (boardx >= 550 - length)
		boardx = 550 - length;
	setfillcolor(BLUE);
	solidrectangle(boardx, boardy, boardx + length, boardy + 10);
}

void BallMove(int *X, int *Y, int len)//有关小球运动的函数
{
	iscatch = 0;
	setfillcolor(BLACK);
	solidcircle(ballx, bally, 10);

	BeginBatchDraw();
	if (ballx >= 540 || ballx <= 10)	//左右壁
	{
		velocity_x *= -1;
	}
	if (bally <= 10)					//上壁
		velocity_y *= -1;
	int flag = 0;

	for (int m = 0; m != len; m++)
	{
		int x = X[m];
		int y = Y[m];

		if (arr[m][m] == 0 && ballx >= x - 10 && ballx <= x + 32 && bally <= y + 22 && bally >= y - 10)
		{
			if (bally >= y && bally <= y + 12)	//左右
				velocity_x *= -1;
			else if (ballx >= x && ballx <= x + 22)	//上下
				velocity_y *= -1;
			else  if (pow(ballx - x, 2) + pow(bally - y, 2) <= 100 || pow(ballx - x - 22, 2) + pow(bally - y, 2) <= 100 || pow(ballx - x, 2) + pow(bally - y - 12, 2) <= 100 || pow(ballx - x - 22, 2) + pow(bally - y - 12, 2) <= 100)
			{
				velocity_x *= -1;
				velocity_y *= -1;
			}
			else
				continue;
			arr[m][m] = 1;
			flag = 1;
			setfillcolor(BLACK);
			solidrectangle(x, y, x + 20, y + 10);
			if (rand() % 100<50 && intTreasure == 0 && getTreasure == 0)    //30%的概率出现宝物    rand()%100<60 && intTreasure==0 && getTreasure==0
			{

				Treasure.x = x;
				Treasure.y = y;
				intTreasure = 1;

				if (rand() % 100<25)  //加速
				{
					Treasure.type = 1;
				}
				else if (rand() % 100<50)  //减速
				{
					Treasure.type = 2;

				}
				else if (rand() % 100<75)  //板加长
				{
					Treasure.type = 3;

				}
				else           //板缩短
				{
					Treasure.type = 4;

				}
			}
		}
	}
	if (starting&&ballx >= boardx - 10 && ballx <= boardx + length + 10 && bally >= boardy - 10 && bally < boardy)
	{
		iscatch = 1;
		if (ballx >= boardx && ballx <= boardx + length)
			velocity_y *= -1;
		else if (pow(ballx - boardx, 2) + pow(bally - boardy, 2) <= 100 || pow(ballx - boardx - length, 2) + pow(bally - boardy, 2) <= 100)
		{
			velocity_y *= -1;
			velocity_x *= -1;
		}
	}
	starting = 1;
	ballx += velocity_x;
	bally += velocity_y;
	setfillcolor(WHITE);
	solidcircle(ballx, bally, 10);
	FlushBatchDraw();
}
void move_Treasure()
{
	setfillcolor(BLACK);
	solidrectangle(Treasure.x, Treasure.y, Treasure.x + 22, Treasure.y + 12);
	if (Treasure.y + 12 >= boardy && Treasure.x>boardx - 22 && Treasure.x <boardx + length)
	{
		get_Treasure();
		intTreasure = 0;
		return;
	}
	else if (Treasure.y + 1<600)
	{
		Treasure.y = Treasure.y + 3;
	}
	else
	{
		intTreasure = 0;
		return;
	}
	setfillcolor(RED);
	solidrectangle(Treasure.x, Treasure.y, Treasure.x + 22, Treasure.y + 12);

}
void get_Treasure()
{
	switch (Treasure.type)
	{
	case 1: //加速
		speed = 1;
		break;

	case 2: //减速
		speed = 3;
		break;

	case 3: //伸长
		slong = 3;
		if (boardx>390)
			boardx = 390;
		break;

	case 4: //缩

		slong = 1;
		setfillcolor(BLACK);
		solidrectangle(boardx + length, boardy, boardx + length + length, boardy + 10);//木板宽80，高10
		break;
	}
	getTreasure = 1;  //获得宝石
	treaTime_Last = clock();    //此处开始宝石实效计时
}
void time_Treasure()
{
	switch (Treasure.type)
	{
	case 1: //加速

	case 2: //减速
		speed = 2;
		break;

	case 3: //伸长
		slong = 2;
		setfillcolor(BLACK);
		solidrectangle(boardx + length, boardy, boardx + length + length, boardy + 10);//木板宽80，高10
	case 4: //缩短
		slong = 2;

		break;
	}
}
