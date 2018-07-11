#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<list>
#include"Astar.h"
#include<time.h>
#include<Windows.h>         //鼠标点击添加障碍物
#define msize 200
#define barrier_num 30     //障碍物数量
#define safe_distance 1    //安全距离
#define safe_value 9     //等待转换为1的安全值
char out = '>';
 unsigned long _time=100;
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
 
typedef struct plane_direct {
	int dx;
	int dy;
}pdirect;
typedef struct plane_point {
	int x;
	int y;
	//int direct; //以右上方向为1，顺时针加
	int dx;
	int dy;
	int step;
}Plane;


void changehead(Plane p[], Point &endpoint);
void add_barrier(int ox, int oy, std::vector<std::vector<int>>&maze)
{
	for (int i = -10; i < 10; i++)
		for (int j = -10; j < 10; j++)
		{
			if (abs(i) + abs(j) <= 15)
				maze[i + ox][j + oy] = 1;
		}
}

void resetmap(std::vector<std::vector<int>>& maze)
{
	for (int i = 0; i < msize; i++)
	{
		for (int j = 0; j < msize; j++)
		{
			if (maze[j][i] != 0 && maze[j][i] != 1 && maze[j][i] != 5)   //注意，\n是换y，所以遍历一个y对应的x，然后\n
				maze[j][i] = 0;
		}
	}
}


void init_map(std::vector<std::vector<int>>& maze)
{
	for (int i = 0; i<msize; i++) {
		maze[i].resize(msize);
	}
	for (int i = 0; i < msize; i++)
	{
		if (i == 0 || i == msize - 1)
		{
			for (int j = 0; j < msize - 1; j++)
			{
				maze[i][j] = 1;
			//	gotoxy(i, j);
				//cout << maze[i][j];
			}
		}
		else {
			maze[i][0] = 1; 
					//gotoxy(i, 0);
					//cout << maze[i][0];
			maze[i][msize - 1] = 1;
					//gotoxy(i, msize - 1);
					//cout << maze[i][msize - 1];
					for (int j = 1; j < msize - 2; j++)
						maze[i][j] = 0;
		}
		/*
		add_barrier(20, 20, maze);
		add_barrier(50, 20, maze);
		add_barrier(70, 20, maze);
		add_barrier(110, 20, maze);
		add_barrier(140, 20, maze);
		add_barrier(170, 20, maze);
		add_barrier(20, 50, maze);
		add_barrier(50, 50, maze);
		add_barrier(70, 50, maze);
		add_barrier(110, 50, maze);
		add_barrier(140, 50, maze);
		add_barrier(170, 50, maze);
		add_barrier(20, 70, maze);
		add_barrier(50, 70, maze);
		add_barrier(70, 70, maze);
		add_barrier(110, 70, maze);
		add_barrier(140, 70, maze);
		add_barrier(170, 70, maze);
		add_barrier(20, 110, maze);
		add_barrier(50,110, maze);
		add_barrier(70, 110, maze);
		add_barrier(110, 110, maze);
		add_barrier(140, 110, maze);
		add_barrier(170, 110, maze);
		add_barrier(20, 140, maze);
		add_barrier(50, 140, maze);
		add_barrier(70, 140, maze);
		add_barrier(110, 140, maze);
		add_barrier(140, 140, maze);
		add_barrier(170, 140, maze);
		add_barrier(20, 170, maze);
		add_barrier(50, 170, maze);
		add_barrier(70, 170, maze);
		add_barrier(110, 170, maze);
		add_barrier(140, 170, maze);
		add_barrier(170, 170, maze);
		*/
		srand(time(0));
		for (int i = 0; i < barrier_num; i++)
		{
			int ox = rand() % 150 + 20;
			int oy = rand() % 150 + 20;
			add_barrier(ox, oy, maze);
		}

	}
	
	for(int i=0;i<msize;i++)
	{
		for (int j = 0; j < msize; j++)
		{
			
			if(maze[j][i] == 1)
			{
				cout << "1";
			//add broad，暂定为只加上下左右的安全距离
				if (safe_distance>0&&i > safe_distance&&i<msize - safe_distance&&j>safe_distance&&j < msize - safe_distance)
				{
					if (maze[j - 1][i] == 0)
						maze[j - 1][i] = safe_value;     //9为暂转换为1的值。

					if (maze[j + 1][i] == 0)
						maze[j + 1][i] = safe_value;     //9为暂转换为1的值。

					if (maze[j][i + 1] == 0)
						maze[j][i + 1] = safe_value;     //9为暂转换为1的值。

					if (maze[j][i - 1] == 0)
						maze[j][i - 1] = safe_value;     //9为暂转换为1的值。
				}

			}
			else
				cout << " ";
		}
		cout << endl;
	}

	//转换为1
	for (int i = 0; i < msize; i=i++)
	{
		for (int j = 0; j < msize; j=j++)
		{
			if (maze[i][j] == 9)
				maze[i][j] = 1;
		}
	}

}

//手动函数
int rcal_x(int x)
{
	switch (x)
	{
	case 1:
		return -2+1; break;
	case 2:
	case 6:
		return -3+1; break;
	case 3:
	case 5:
	case 7:
		return -4+1; break;
	case 4:
	case 8:
		return -5+1; break;
	case 9:
		return -6+1; break;
	default:
		return 0;
	}
}
int lcal_x(int x)
{
	switch (x)
	{
	case 1:
		return  2 - 1; break;
	case 2:
	case 6:
		return  3 - 1; break;
	case 3:
	case 5:
	case 7:
		return  4 - 1; break;
	case 4:
	case 8:
		return  5 - 1; break;
	case 9:
		return  6 - 1; break;
	default:
		return 0;
	}
}
int cal_y(int y)
{
	switch (y)
	{
	case 3:
		return 2; break;
	case 2:
	case 4:
		return 1; break;
	case 1:
	case 5:
	case 9:
		return 0; break;
	case 6:
	case 8:
		return -1; break;
	case 7:
		return -2; break;
	default:
		return 0;
	}
}

int comp(int x)
{
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
}




int get_direct(std::list<Point*> &path,pdirect *direct)   //方向
{
	if (!path.empty())
	{
		int temp_x = path.front()->x;
		int temp_y = path.front()->y;
		//Point temp = { ,path.front()->y };
		path.pop_front();
		if (!path.empty())
		{
			direct->dx = comp(path.front()->x - temp_x);
			direct->dy = comp(path.front()->y - temp_y);
			return 1;
		}
	}
	return 0;
}


void show(Plane p[])
{
	
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Point past(0, 0);
	for (int i = 0; i < 10; i++)
	{
		past.x = p[i].x - p[i].dx;
		past.y = p[i].y - p[i].dy;
		//消past，显示p[i]
		gotoxy(past.x, past.y);
		cout<<" ";
		gotoxy(p[i].x, p[i].y);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
		//printf(">");
		//cout << i;
		cout << out;
	}
}



void startpoint(int x, int y,Plane p[], std::vector<std::vector<int>>& maze,int lr)
{
	//左右头两种方案
	int(*cal_x)(int x);
	if (lr < 0) {
		cal_x = lcal_x;
		out = '<';
	}
	else {
		cal_x = rcal_x;
		 out = '>';
	}
	for (int i = 0; i < 10; i++)
	{
		p[i].x = x + cal_x(i);
		p[i].y = y + cal_y(i);
		maze[p[i].x][p[i].y] = 5;//标记飞机的位置。
		/*gotoxy(p[i].x, p[i].y);
		//printf("\b");
		cout << ">";*/
	}

}

void adjust(Plane p[], std::vector<std::vector<int>>& maze, std::list<Point *> &path,int lr,Point &end,int &t)  //t表示冲突数
{
	//左右头两种方案
	int(*cal_x)(int x);
	if (lr < 0 ) cal_x = lcal_x;
	else cal_x = rcal_x;
	bool flag = 1;
	pdirect *direct = new pdirect;
	//while()，循环！！
	//显示一下
	while (flag||(!path.empty()))
	{
		flag = 0;//不移动
		//显示
		show(p);
		//延时
		Sleep(_time);
		if (get_direct(path, direct))  //未接触终点
		{
			//判断头领可以前进，若不能，换头领，或整体移动，重新进行导航！！！！之后再加！
			if (maze[p[0].x + direct->dx][p[0].y + direct->dy] != 1 && maze[p[0].x + direct->dx][p[0].y + direct->dy] != 5)
			{
				p[0].dx = direct->dx;//记录走过的方向
				p[0].dy = direct->dy;
				maze[p[0].x][p[0].y] = 0;
				p[0].x += direct->dx;
				p[0].y += direct->dy;
				p[0].step ++;
				maze[p[0].x][p[0].y] = 5;
				int xi, yi;
				for (int i = 1; i < 10; i++)//巧用continue，更新一次十架无人机的位置
				{

					//掉队办法：(先判定此轮没走，再)判定是否掉队以及是否没被堵住，没跟上上一个
					if ((abs(p[i].x - p[i - 1].x) + abs(p[i].y - p[i - 1].y))>5)
					{
						Astar astari;
						astari.InitAstar(maze);
						list<Point *> pathi = astari.GetfPath(Point(p[i].x, p[i].y), Point(p[i - 1].x, p[i - 1].y), true);
						resetmap(maze);
						Point *tempi = nullptr;
						if (!pathi.empty())
							pathi.pop_front();//本身的起点去掉
						if (!pathi.empty())
						{
							tempi = pathi.front();//先赋值
												  //如何跟得住？一是没走完路径，二是没撞上大部队，但起始时要避开相撞导致“堵死”问题
							while ((!pathi.empty()) && (maze[tempi->x][tempi->y] != 5 || (abs(p[i].x - p[i - 1].x) + abs(p[i].y - p[i - 1].y))>5))
							{
								tempi = pathi.front();
								p[i].dx = comp(tempi->x - p[i].x);
								p[i].dy = comp(tempi->y - p[i].y);
								maze[p[i].x][p[i].y] = 0;
								p[i].x = tempi->x;
								p[i].y = tempi->y;
								maze[p[i].x][p[i].y] = 5;
								pathi.pop_front();
									p[i].step++;
								show(p);
								Sleep(_time);
							}
						}
					}

					flag = 0;
					//1.趋向正常位置
					if (p[i].x != p[0].x + cal_x(i) || p[i].y != p[0].y + cal_y(i))
					{
						xi = comp(p[0].x + cal_x(i) - p[i].x);
						yi = comp(p[0].y + cal_y(i) - p[i].y);
						int check = maze[p[i].x + xi][p[i].y + yi];        
						if (check != 1 && check != 5)
						{
							maze[p[i].x][p[i].y] = 0;
							p[i].x += xi;
							p[i].y += yi;
							maze[p[i].x][p[i].y] = 5;
									p[i].step++;
							p[i].dx = xi;
							p[i].dy = yi;
							flag = 1;
							continue;
						}
						
						else if ((check = maze[p[i].x + xi][p[i].y])&&check != 1 && check!= 5)
						{
							t++;
							maze[p[i].x][p[i].y] = 0;
							p[i].x += xi;
							p[i].y += 0;
							maze[p[i].x][p[i].y] = 5;
									p[i].step++;
							p[i].dx = xi;
							p[i].dy = 0;
							flag = 1;
							continue;
							}
						
					else if ((check = maze[p[i].x ][p[i].y + yi]) && check != 1 && check != 5)
						{
						t++;
						maze[p[i].x][p[i].y] = 0;
						p[i].x += 0;
						p[i].y += yi;
						maze[p[i].x][p[i].y] = 5;
									p[i].step++;
						p[i].dx = 0;
						p[i].dy = yi;
						flag = 1;
						continue;
							}
						}


					//不能用else，容易漏判
					//2.同方向移动，方法2
					if (maze[p[i].x + direct->dx][p[i].y + direct->dy] != 1 && maze[p[i].x + direct->dx][p[i].y + direct->dy] != 5)
					{
						t++;
						maze[p[i].x][p[i].y] = 0;
						p[i].dx = direct->dx;//记录走过的方向
						p[i].dy = direct->dy;
						p[i].x += p[i].dx;
						p[i].y += p[i].dy;
						maze[p[i].x][p[i].y] = 5;
										p[i].step++;
						flag = 1;
						continue;
					}
					//若有相同方向分量的移动，需判断使其距离大部队不会变远
							/*		else if (maze[p[i].x + direct->dx][p[i].y] != 1 && maze[p[i].x + direct->dx][p[i].y] != 5)
									{
										int temp_distance1 = abs(p[i].x - p[0].x) + abs(p[i].y - p[0].y);
										int temp_distance2 = abs(p[i].x + direct->dx - p[0].x) + abs(p[i].y - p[0].y);
										if (temp_distance1 >= temp_distance2)
										{
											maze[p[i].x][p[i].y] = 0;
											p[i].dx = direct->dx;//记录走过的方向
											p[i].dy = 0;
											p[i].x += p[i].dx;
											p[i].y += 0;
											maze[p[i].x][p[i].y] = 5;
											flag = 1;
											continue;
										}
									}
									else if (maze[p[i].x][p[i].y + direct->dy] != 1 && maze[p[i].x][p[i].y + direct->dy] != 5)
									{
										int temp_distance1 = abs(p[i].x - p[0].x) + abs(p[i].y - p[0].y);
										int temp_distance2 = abs(p[i].x - p[0].x) + abs(p[i].y + direct->dy - p[0].y);
										if (temp_distance1 >= temp_distance2)
										{
											maze[p[i].x][p[i].y] = 0;
											p[i].dx = 0;//记录走过的方向
											p[i].dy = direct->dx;
											p[i].x += 0;
											p[i].y += p[i].dy;
											maze[p[i].x][p[i].y] = 5;
											flag = 1;
											continue;
										}
									}
									*/

					//3.一条龙移动，
					xi = comp(p[i - 1].x - p[i - 1].dx - p[i].x);
					yi = comp(p[i - 1].y - p[i - 1].dy - p[i].y);
					if (maze[p[i].x + xi][p[i].y + yi] != 5 && maze[p[i].x + xi][p[i].y + yi] != 1)
					{
						t++;
						p[i].dx = xi;//记录走过的方向
						p[i].dy = yi;
						maze[p[i].x][p[i].y] = 0;
						p[i].x += p[i].dx;
						p[i].y += p[i].dy;
						maze[p[i].x][p[i].y] = 5;
										p[i].step++;
						flag = 1;
						continue;
					}
					


					//else,原地不动，没有移动的次数增加，前面的flag=1后再添个把停滞次数置零的操作。
				}

			}
			
			else  if (maze[p[0].x + direct->dx][p[0].y + direct->dy] == 5)  //被自己堵住
			{
				t++;
				int xtemp = p[0].x;
				int ytemp = p[0].y;
				for (int i = 9; i >= 0; i--)
				{
					if (maze[p[i].x + direct->dx][p[i].y + direct->dy] != 1 && maze[p[i].x + direct->dx][p[i].y + direct->dy] != 5)
					{
						p[i].dx = direct->dx;//记录走过的方向
						p[i].dy = direct->dy;
						maze[p[i].x][p[i].y] = 0;
						p[i].x += direct->dx;
						p[i].y += direct->dy;
						maze[p[i].x][p[i].y] = 5;
										p[i].step++;
						flag = 1;
						continue;
					}
					
					//单纯逆序不能解决，则跳棋
					else if (maze[p[i].x + 2 * direct->dx][p[i].y + 2 * direct->dy] != 1&& maze[p[i].x + 2 * direct->dx][p[i].y + 2 * direct->dy] != 5)
					{
						t++;
						p[i].dx = direct->dx;//记录走过的方向
						p[i].dy = direct->dy;
						maze[p[i].x][p[i].y] = 0;
						p[i].x += 2*direct->dx;
						p[i].y += 2*direct->dy;
						maze[p[i].x][p[i].y] = 5;
								p[i].step++;
						flag = 1;
						continue;
					}
				}
			
			}
			
			//换头领
			//changehead(p, end);
			/*else
			{
				int m；
				for (m = 0; m < 10; m++)//计算哪个起点近，且能冲破封锁，然后交换
				{
					//计算哪个近，且能运动，然后break；
			}
				Plane temp;
				temp = p[0];
				p[0] = p[m];
				p[m] = temp;
			}
			*/
		}

		//等待其他飞机到位
		else if(path.empty())
		{
			int xi, yi;
			for (int i = 1; i < 10; i++)//巧用continue，更新一次十架无人机的位置
			{
				//特殊：到位
				if (p[i].x == p[0].x + cal_x(i) && p[i].y == p[0].y + cal_y(i))
				{
					continue;
				}



				//1.趋向正常位置
				if (p[i].x != p[0].x + cal_x(i) || p[i].y != p[0].y + cal_y(i))
				{
					xi = comp(p[0].x + cal_x(i) - p[i].x);
					yi = comp(p[0].y + cal_y(i) - p[i].y);
					int check = maze[p[i].x + xi][p[i].y + yi];        //不应只是斜着走！（需改！！
					if (check != 1 && check != 5)
					{
						maze[p[i].x][p[i].y] = 0;
						p[i].x += xi;
						p[i].y += yi;
						maze[p[i].x][p[i].y] = 5;
								p[i].step++;
						p[i].dx = xi;
						p[i].dy = yi;
						flag = 1;
						continue;
					}
				}
				//不能用else，容易漏判
				//2.同方向移动，方法2
				if (maze[p[i].x + direct->dx][p[i].y + direct->dy] != 1 && maze[p[i].x + direct->dx][p[i].y + direct->dy] != 5)
				{
					t++;
					maze[p[i].x][p[i].y] = 0;
					p[i].dx = direct->dx;//记录走过的方向
					p[i].dy = direct->dy;
					p[i].x += p[i].dx;
					p[i].y += p[i].dy;
					maze[p[i].x][p[i].y] = 5;
								p[i].step++;
					flag = 1;
					continue;
				}

				//3.一条龙移动，方法3，又分为优先斜着走和直着走的三个方案
				xi = comp(p[i - 1].x - p[i - 1].dx - p[i].x);
				yi = comp(p[i - 1].y - p[i - 1].dy - p[i].y);
				if (maze[p[i].x + xi][p[i].y + yi] != 5 && maze[p[i].x + xi][p[i].y + yi] != 1)
				{
					t++;
					p[i].dx = xi;//记录走过的方向
					p[i].dy = yi;
					maze[p[i].x][p[i].y] = 0;
					p[i].x += p[i].dx;
					p[i].y += p[i].dy;
					maze[p[i].x][p[i].y] = 5;
								p[i].step++;
					flag = 1;
					continue;
				}

			}

		}
	}
	delete direct;//good habit
	//exit(0);
}


void changehead(Plane p[], Point &endpoint)
{
	int dis[10],m=400,n,i=0;
	for (i = 1; i < 10; i++)
	{
		dis[i] = abs(endpoint.x - p[i].x) + abs(endpoint.y - p[i].y);
		if (dis[i] < m)
		{
			m = dis[i];
			n = i;
		}
		
	}
	Plane temp;
	temp = p[i];
	p[i] = p[0];
	p[0] = p[i];
}

