#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

int map[50][25] = { 0 }, k = 1, d = 1,m=1; 
int len = 2;//蛇长
int speed=200;//移动速度(sleep参数)
int Speed=0;//判断是否加速
int score = 0;//分数
int count = 0;//run函数每执行一次，count++，毒草产生与闪烁的参数
int poison_speed = 10;//毒草产生的速度参数
int level = 1;//难度or关卡
int bomb_5 = 1;//是否先产生5个炸弹，1产生，0不产生
int di = 2;//初始化方向为右
int big5[6] = { 0 };//用来储存排行榜分数的数组
int live = 1;//判断gameover后是否调用menu
int pause = 0;//判断开始游戏前是否暂停
FILE * mem;//存档文件指针
//gotoxy及隐藏光标
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x; c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);//隐藏控制台光标
}
//节点结构体
struct node
{
	int x;
	int y;
	struct node *next;
	struct node *previous;
}*head, *tail;
//第一关墙
void creatwall_1()
{
	int x, y=0;
	for (x = 0; x < 50; x++)
	{
		gotoxy(x, 0);
		printf("#");
		map[x][0] = 4;
		gotoxy(x, 24);
		printf("#");
		map[x][24] = 4;
	}

	for (y = 0; y < 25; y++)
	{
		gotoxy(0, y);
		printf("#");
		map[0][y] = 4;
		gotoxy(49, y);
		printf("#");
		map[49][y] = 4;
	}

}
//第二关墙
void creatwall_2() 
{
	int x, y = 0;

	//上一关地图重置
	for (x = 0; x < 50; x++)
	{
		for (y = 0; y < 25; y++)
		{
			map[x][y] = 0;
		}
	}

	for (x = 0; x < 50; x++)
	{
		gotoxy(x, 0);
		printf("#");
		map[x][0] = 4;
		gotoxy(x, 24);
		printf("#");
		map[x][24] = 4;
	}

	for (y = 0; y < 25; y++)
	{
		gotoxy(0, y);
		printf("#");
		map[0][y] = 4;
		gotoxy(49, y);
		printf("#");
		map[49][y] = 4;
	}

	for (x = 10; x < 40; x++)
	{
		gotoxy(x, 11);
		printf("#");
		map[x][11] = 4;
	}

	for (y = 5; y < 20; y++)
	{
		gotoxy(24, y);
		printf("#");
		map[24][y] = 4;
	}
}
//第三关墙
void creatwall_3()
{
	int x, y = 0;

	//上一关地图重置
	for (x = 0; x < 50; x++)
	{
		for (y = 0; y < 25; y++)
		{
			map[x][y] = 0;
		}
	}

	for (x = 0; x < 50; x++)
	{
		gotoxy(x, 0);
		printf("#");
		map[x][0] = 4;
		gotoxy(x, 24);
		printf("#");
		map[x][24] = 4;
	}

	for (y = 0; y < 25; y++)
	{
		gotoxy(0, y);
		printf("#");
		map[0][y] = 4;
		gotoxy(49, y);
		printf("#");
		map[49][y] = 4;
	}

	for (y = 5; y < 20; y++)
	{
		gotoxy(24, y);
		printf("#");
		map[24][y] = 4;
	}

	for (x = 9; x < 41; x++)
	{
		gotoxy(x, 9);
		printf("#");
		map[x][9] = 4;
	}

	for (x = 9; x < 41; x++)
	{
		gotoxy(x, 15);
		printf("#");
		map[x][15] = 4;
	}
}
//炸弹
void bomb()
{
	int x, y;
	if (bomb_5 == 1)
	{
		for (int i = 0; i < 5; i++)
		{
			do
			{
				x = rand() % 48 + 1;
				y = rand() % 23 + 1;
			} while (map[x][y] == 1 || map[x][y] == 2 || map[x][y] == 3 || map[x][y] == 4);
			gotoxy(x, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			printf("$");
			map[x][y] = 5;
		}
		bomb_5 = 1;
	}
	do
	{
		x = rand() % 48 + 1;
		y = rand() % 23 + 1;
	} while (map[x][y] == 1 || map[x][y] == 2 || map[x][y] == 3 || map[x][y] == 4);

			gotoxy(x, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			printf("$");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
			map[x][y] = 5;
			m=0;//m=1时创造

}
//食物
void creatfood()
{
	int x, y;
	do
	{
		x = rand() % 48 + 1;
		y = rand() % 23 + 1;
	} while (map[x][y] == 1 || map[x][y] == 2|| map[x][y] == 3||map[x][y] == 4);//map,1蛇,2食物,3毒,4墙,5雷
	gotoxy(x, y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("@");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	map[x][y] = 2;
	k = 0;//k=1创造食物
}
//毒草
void poison()
{
	int x = 0, y = 0;
			do
			{
				x = rand() % 48 + 1;
				y = rand() % 23 + 1;
			} while (map[x][y] == 1 || map[x][y] == 2 || map[x][y] == 3 || map[x][y] == 4||map[x][y]==5);
			gotoxy(x, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("*");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
			map[x][y] = 3;
			d=0;//d=1时创造毒草
}
//初始化蛇的坐标及头尾指针
void snake()
{
	di = 2;
	struct node *p;
	p = (struct node*)malloc(sizeof(struct node));
	head=p;
	head->x = 3;
	head->y = 3;
	p = (struct node*)malloc(sizeof(struct node));
	tail = p;
	tail->x = 2;
	tail->y = 3;
	tail->next = 0;
	tail->previous = head;
	head->next=tail;
	head->previous = 0;
	gotoxy(head->x, head->y);
	printf("O");
	gotoxy(tail->x, tail->y);
	printf("O");
}
//键盘输入改变方向
void input()
{
	int a;
	if (_kbhit())
	{
		a = _getch();
		if (a == 224)
		{
			a = _getch();
			if (a == 72 && di != 3)di = 1;//1上
			if (a == 77 && di != 4)di = 2;//2右
			if (a == 80 && di != 1)di = 3;//3下
			if (a == 75 && di != 2)di = 4;//4左
		}
		else if (a == 97)	Speed = 1;
		else _getch();
	}
}
//动蛇头以及吃到食物长头
void movehead()
{
	struct node *p;
	p = (struct node*)malloc(sizeof(node));
	head->previous = p;
	p->next = head;
	p->previous = NULL;
	if (di == 4)
	{
		p->x = head->x - 1;
		p->y = head->y;
	}
	else if (di == 3)
	{
		p->x = head->x;
		p->y = head->y + 1;
	}
	else if (di == 2)
	{
		p->x = head->x + 1;
		p->y = head->y;
	}
	else
	{
		p->x = head->x;
		p->y = head->y - 1;
	}
	/*if (p->x>48)p->x = 1;穿地图
	else if (p->x<0)p->x = 48;
	if (p->y>24)p->y = 1;
	else if (p->y<0)p->y = 24;*/
	head = p;
	gotoxy(head->x, head->y);
	printf("O");
}
//吃到毒草去尾
void cuttail()
{
	struct node *p;
	p = (struct node*)malloc(sizeof(node));
	gotoxy(tail->x, tail->y);
	printf(" ");
 	p = tail->previous;
	p->next = 0;
	free(tail);
	tail = p;
}
//重置节点
void reset()
{
	struct node *p;
	p = (struct node*)malloc(sizeof(struct node));
	head->next = p;
	p->previous = head;
	tail->next = 0;
	do
	{
		map[p->x][p->y] = 1;
		p = p->next;
	} while (p != 0);
}
//存档
void save() {
	
	//打开存档文件
	if (fopen_s(&mem, "memory.txt", "w") != NULL)
	{
		gotoxy(17, 12);
		printf("无法打开文件!");
	}
	//保存分数到数组
	for (int n = 0; n < 5; n++)
	{
		if (score >= big5[0])
		{
			for (n = 0; n < 5; n++)
			{
				big5[n + 1] = big5[n];
			}
			big5[0] = score;
		}
		else
		{
			for (n = 1; n < 5; n++)
			{
				if (score >= big5[n] && score <= big5[n - 1])
				{
					for (int i=3; i >=n ; i--)
					{
						big5[i+1] = big5[i];
					}
					big5[n] = score;
					break;
				}
				break;
			}
		}
	}
	//将分数写入文件
	fwrite(big5, 4, 5, mem);
	fclose(mem);
}
//读档
void load()
{
	if (fopen_s(&mem, "memory.txt", "r") != NULL)
	{
		fprintf(stderr, "No Game Cache Here!\n");
	}
	fread(big5, sizeof(int),5, mem);

	fclose(mem);
}
//循环的运行函数
void run()
{
	int x, y;
	gotoxy(0, 25);
	printf("得分:%d",score);
	char z;
	while (len != 0 )
	{
		gotoxy(0, 25);
		printf("得分:%d", score);
		input();
		if (count%poison_speed == 0)poison();
		if (k == 1)creatfood();
		if (m == 1)bomb();
		if (pause == 1)
		{
			z = _getch();
			pause = 0;
		}
		if (map[head->x][head->y] == 2)//蛇头与食物重合
		{
			map[head->x][head->y] = 0;
			movehead();
			k = 1;//创造食物标志
			len++;
			speed = speed - 10;
			//score = len * 10;
			score = score + 10;
			gotoxy(0, 25);
			printf("得分:%d    ", score);
		}
		if (map[head->x][head->y] == 3)//蛇头与毒草重合
		{
			map[head->x][head->y] = 0;
			if (len > 1)	cuttail();
			d = 1;
			len--;
			score = score - 5;
			gotoxy(0, 25);
			printf("得分:%d   ", score);
		}
		if (map[head->x][head->y] == 5)//蛇头与炸弹重合
		{
			map[head->x][head->y] = 0;
			m = 1;
			for (int i = 1; i <= len - (len / 2); i++)
			{
				cuttail();
			}
			len = len / 2;
			score = score - 30;
			gotoxy(0, 25);
			printf("得分:%d    ", score);
		}
		if (map[head->x][head->y] == 1 || map[head->x][head->y] == 4 || len == 0)
		{
			live = 0;
			system("cls");
			gotoxy(17, 10);
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			printf("GAME OVER!");
			gotoxy(17, 11);
			printf("得分:%d   ", score);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
			save();
			score = 0;
			getchar();
			for (x = 0; x < 50; x++)
			{
				for (y = 0; y < 25; y++)
				{
					map[x][y] = 0;
				}
			}
			break;
		}
		if (score >= 10 && level == 1)
		{
			system("cls");
			gotoxy(17, 10);
			printf("恭喜难度升级!");
			gotoxy(17, 11);
			printf("得分:%d   ", score);
			score = 20;
			level = 2;
			k = 1;
			m = 1;
			bomb_5 = 1;
			pause = 1;
			getchar();
			system("cls");
			//snake();
			creatwall_2();
			head->x = 3;
			head->y = 3;
			continue;
		}
		if (score >= 30 && level == 2)
		{
			system("cls");
			gotoxy(17, 10);
			printf("恭喜难度升级!");
			gotoxy(17, 11);
			printf("得分:%d   ", score);
			score = 40;
			level = 3;
			m = 1;
			k = 1;
			bomb_5 = 1;
			pause = 1;
			getchar();
			system("cls");
			//snake();
			creatwall_3();
			head->x = 3;
			head->y = 3;
			continue;
		}
		if (score >= 50 && level == 3)
		{
			live = 0;
			system("cls");
			gotoxy(17, 10);
			printf("恭喜通关!");
			gotoxy(17, 11);
			printf("得分:%d   ", score);
			save();
			score = 0;
			getchar();
			//地图重置
			for (x = 0; x < 50; x++)
			{
				for (y = 0; y < 25; y++)
				{
					map[x][y] = 0;
				}
			}
			break;
		}
		for (x = 1; x < 49; x++)
		{
			for (y = 1; y < 24; y++)
			{
				if (map[x][y] == 3)
				{
					if (count % 2 == 0)
					{
						if (count % 4 == 0)
						{
							gotoxy(x, y);
						printf(" ");
						}				
						else
						{
							gotoxy(x, y);
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
							printf("*");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
						}
					
					}
				}
			}
		}
		movehead();
		cuttail();		
		if (Speed == 1)
		{
			Sleep(100);
			Speed = 0;
		}
		else Sleep(speed);
		count++;
	}
}
//开始新游戏
void starts()
{
	k = 1;
	d = 1;
	m = 1;
	bomb_5 = 1;
	snake();
	creatwall_1();
	run();
}
//开始菜单
void menu()
{
		system("cls");
		gotoxy(0, 0);
		printf("->开始新游戏\n\n\n  读取存档\n\n\n  难度选择\n\n\n  查看排行\n\n\n  退出游戏");
		int a = 1;
		int n = 0;
		int i = 0,j = 0;
		int x = 0, y = 0;
		char ch;
		while ((ch = _getch()) != 13)
		{
			if (ch == 80)
			{
				if (i == 12)
				{
					gotoxy(0, i); printf(" ");
					gotoxy(1, i); printf(" ");
					i = 0;
					gotoxy(0, i); printf("-");
					gotoxy(1, i); printf(">");
					a = 1;
				}
				else {
					gotoxy(0, i); printf(" ");
					gotoxy(1, i); printf(" ");
					gotoxy(0, i + 3); printf("-");
					gotoxy(1, i + 3); printf(">");
					i = i + 3;
					a = a + 1;
				}
			}

			if (ch == 72)
			{
				if (i == 0)
				{
					gotoxy(0, i); printf(" ");
					gotoxy(1, i); printf(" ");
					i = 12;
					gotoxy(0, i); printf("-");
					gotoxy(1, i); printf(">");
					a = 5;
				}
				else {
					gotoxy(0, i); printf(" ");
					gotoxy(1, i); printf(" ");
					gotoxy(0, i - 3); printf("-");
					gotoxy(1, i - 3); printf(">");
					i = i - 3;
					a = a - 1;
				}
			}

		}
		//回车
		if (ch == 13)
		{
			//开始新游戏
			if (a == 1)
			{
				system("cls");
				poison_speed = 10;
				starts();
			}
			//读取存档
			if (a == 2)
			{
				live = 0;
			}
			//难度选择
			if (a == 3)
			{
				system("cls");
				gotoxy(0, 0);
				printf("->简单  \n\n\n  中等  \n\n\n  困难  \n\n\n  返回主菜单");
				int b=1;
				while ( (ch = _getch()) != 13)
				{
					//向下
					if (ch == 80)
					{
						if (j == 9)
						{
							gotoxy(0, j); printf(" ");
							gotoxy(1, j); printf(" ");
							j = 0;
							gotoxy(0, j); printf("-");
							gotoxy(1, j); printf(">");
							b = 1;
						}
						else {
							gotoxy(0, j); printf(" ");
							gotoxy(1, j); printf(" ");
							gotoxy(0, j + 3); printf("-");
							gotoxy(1, j + 3); printf(">");
							j =j + 3;
							b = b + 1;
						}
					}
					//向上
					if (ch == 72)
					{
						if (j == 0)
						{
							gotoxy(0, j); printf(" ");
							gotoxy(1, j); printf(" ");
							j = 9;
							gotoxy(0, j); printf("-");
							gotoxy(1, j); printf(">");
							b = 4;
						}
						else {
							gotoxy(0, j); printf(" ");
							gotoxy(1, j); printf(" ");			
							gotoxy(0, j - 3); printf("-");
							gotoxy(1, j - 3); printf(">");		
							j = j - 3;			
							b = b - 1;
						}
					}
				}
				//回车
				if (ch == 13)
				{
					switch (b)
					{
					case 1:poison_speed = 10; system("cls"); starts(); break;//简单
					case 2:poison_speed = 8; system("cls"); starts(); break;//普通
					case 3:poison_speed = 5; system("cls"); starts(); break;//困难
					default:live = 0;break;//返回主菜单
					}
				}				
			}
			//查看排行
			if (a == 4)
			{
				system("cls");
				load();
					for (y = 5; y <= 17; y=y+3)
					{
						gotoxy(19, y);
						printf("第%d名:%d", n + 1, big5[n]);
						n = n + 1;
					}

				getchar();
				live = 0;
			}
			//退出游戏
			if (a == 5)
			{
				exit(0);
			}
		}
}	

int main()
{
	srand((unsigned)time(NULL));//初始化time种子
	system("mode con cols=50 lines=26");//控制台大小
	menu();//第一次运行菜单
	while (live == 0)//死亡、通关、返回主菜单
	{
		menu();
	}		
	getchar();
}