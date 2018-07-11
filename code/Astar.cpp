#include<math.h>
#include<queue>
#include<stack>
#include "Astar.h"
int step = 0;
void Astar::InitAstar(std::vector<std::vector<int>>& _maze)   //二维容器
{
	maze = _maze;
}

std::list<Point*> Astar::GetfPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	Point *result = findPath(startPoint, endPoint, isIgnoreCorner);
	std::list<Point *> path;
	//返回路径,若未找到,则返回空链表
	while (result)
	{
		path.push_front(new Point(result->x, result->y));//从前面插入,倒叙.
		result = result->parent;
	}
	while (!openList.empty())
	{
		auto curPoint = openList.front();
		openList.pop_front();
		delete curPoint;
	}
	//cout << step<<endl;
	return path;
}


int Astar::calcG(Point * temp_start, Point * point)   //周围移动耗费
{
	int extraG = (abs(point->x - temp_start->x) + abs(point->y - temp_start->y) == 1) ? kCost1 : kCost2;  //判断是否为斜格
	int parentG = (point->parent == NULL) ? 0 : point->parent->G;
	return parentG + extraG;
}

int Astar::calcH(Point * point, Point * end)   //距离,计算算法仍有优化空间!
{
	return (abs(end->x - point->x) + abs(end->y - point->y))*kCost1;   //此处用简单算法:只可以平动不斜动
}

int Astar::calcF(Point * point)
{
	return point->G + point->H;
}
 
/*
Point * Astar::getLeastFpoint(const Point * point,bool isIgnoreCorner)     //更改为只在周围未走过的点搜索！！
{
	if (point != NULL)
	{
		Point *fPoints, *temp;
		for (int x = point->x - 1; x <= point->x + 1; x++)
			for (int y = point->y - 1; y <= point->y + 1; y++)
				if (isCanreach(point, new Point(x, y), isIgnoreCorner, safe_distance))
					fPoints.push_back(new Point(x, y));
		return fPoints;

	}
	




	if (!openList.empty())
	{
		auto resPoint = openList.front();
		for (auto &point : openList)     //从openlist中取点,  ***??***
			if (point->F < resPoint->F)
				resPoint = point;
		return resPoint;                   //返回开启区域中F最小的点
	}
	return nullptr; //c++的空指针
}
*/
Point * Astar::findPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)  //开辟新内存
{
	int flag=0;
	openList.push_back(new Point(startPoint.x, startPoint.y));   //深拷贝!!(只有一个
	Point * curPoint = openList.front();
	curPoint->parent = NULL;
	while (curPoint!=NULL)
	{
		step++;
		flag = 0;
		maze[curPoint->x][curPoint->y] = 3;//标识走过路径
		auto surroundPoints = getSurroundPoints(curPoint, isIgnoreCorner);   //找周围的格子
		for (auto &target : surroundPoints)
		{

			
			//target->parent = curPoint;
			//if(maze[target->x][target->y]==0)  //若target(周围点)不在开启列表中,则开启  
			//{
				target->parent = curPoint;   //当前点为其父点
				target->G = calcG(curPoint, target);
				target->H = calcH(target, &endPoint);
				target->F = calcF(target);
				maze[target->x][target->y] = 2;  //open but not went
				//openList.push_back(target);   //放入openList 中
			//}

			//如果已开启,则必已相邻,看G,如果G大,则不操作,否则换父节点,
			//目的:无论经不经过target,判断绕至curPoint与否才是最近.若是,更改父结点,使得若到target,一定会绕道curPoint(F值最小得出)

			/*

			else if (maze[target->x][target->y] == 2)  //遍历过但没走过
						{
							//重计算G
							int tempG = calcG(curPoint, target);
							//如果绕到curPoint更近
							if (tempG + curPoint->G < target->G)  //make a correction
							{
								target->parent = curPoint;  //换父节点
								target->G = tempG;
								target->F = calcF(target);   //F=G+H  H不变
							}
						}
			*/
		}
	

		Point *resPoint = isInList(curPoint, &endPoint); //判断endPoint是否在openList之中.
		if (resPoint)//endPoint is in openList
			return resPoint;   //开头处深拷贝,故返回resPoint节点,而非endPoint

		//curPoint = getLeastFpoint(curPoint,isIgnoreCorner);  //找到F值最小的点
		if (!surroundPoints.empty())
		{
			auto temPoint = curPoint;
			curPoint = surroundPoints.front();
			for (auto &target : surroundPoints)
			{
				openList.push_back(target);   //放入openList 中
				if (target->F< curPoint->F)
				{
					curPoint = target;
				}
			}
		}
		else
		{
			curPoint = curPoint->parent;
			
		}
	}
	return nullptr;
}




Point * Astar::refindPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	Point *fresult= findPath(startPoint, endPoint, isIgnoreCorner);
	while (fresult)
	{
		maze[fresult->x][fresult->y] = 4;  //第一次走过的点标记4
		fresult = fresult->parent;
	}
	
	
	//开始bfs
	point_queue q;
	Point *p = new Point(startPoint.x, startPoint.y);
	p->parent = NULL;
	q.push(p);
	while (!q.empty())
	{
		Point *temp = q.front();
		if (temp->x == endPoint.x&&temp->y == endPoint.y)
			return temp;
		maze[temp->x][temp->y] = 6;
		for (int i = 0; ; )
		{
			for (int j = 1; j>-2; j--)
			{
				if (maze[temp->x + i][temp->y + j] == 4)
				{
					Point *next = new Point(temp->x + i, temp->y + j);
					next->parent = temp;
					next->G = calcG(temp, next);
					next->H = calcH(next, &endPoint);
					next->F = calcF(next);
					q.push(next);
					maze[temp->x + i][temp->y + j] = 6;

				}
				//标签校正法
				else if (maze[temp->x + i][temp->y + j] == 6)               
				{
					Point *next = new Point(temp->x + i, temp->y + j);
					next->G = calcG(temp, next);
					next->H = calcH(next, &endPoint);
					next->F = calcF(next);
					int pre_F = q.find_point(next);
					if (pre_F <= next->F)
						delete next;
					else														//标签校正法
					{
						q.del(next);
						next->parent = temp;
						q.push(next);
						
					}
				}
			}
			//设定广搜中的旋转取点顺序
			if (i == 0)
				i = 1;
			else if (i == 1)
				i = -1;
			else if (i == -1)
				break;
		}

		q.pop();
	}
	return NULL;
}



std::list<Point*> Astar::GetPath(Point & startPoint, Point & endPoint, bool isIgnoreCorner)
{
	Point *result = refindPath(startPoint, endPoint, isIgnoreCorner);
	std::list<Point *> path;
	//返回路径,若未找到,则返回空链表
	while (result)
	{
		path.push_front(new Point(result->x, result->y));//从前面插入,倒叙.
		result = result->parent;
	}


	
	while (!openList.empty())
	{
		auto curPoint = openList.front();
		openList.pop_front();
		delete curPoint;
	}
	//cout << step<<endl;
	return path;
}


Point * Astar::isInList( Point * curPoint, const Point * point) const
{

	if (maze[point->x][point->y] == 2)
	{
		Point *p = new Point(point->x, point->y);
		p->parent = curPoint;
		return p;
	}
	return nullptr;
}


bool Astar::isCanreach(const Point * point, const Point * target, bool isIgnoreCorner, int safe_d) const  //加安全距离
{
	if (target->x<0 || target->x>maze.size() - 1
		|| target->y<0 || target->y>maze[0].size() - 1
		|| maze[target->x][target->y] == 1
		|| (target->x == point->x&&target->y == point->y)
		|| maze[target->x][target->y]==3)//|| maze[target->x][target->y]==2,  2点也算在每个新店的周围点集内，可能存在绕远，但影响不大。
	{
		delete target;
		return false;
	}
	for (int i = -1; abs(i)<safe_d; i = (i<0) ? (-i) : (-i - 1))
	{
		for (int j = -1; abs(j)<safe_d; j = (j<0) ? (-j) : (-j - 1))
		{
			if (maze[target->x + i][target->y + j] == 1)
			{
				delete target;
				return false;
			}
		}
	}
	if (abs(point->x - target->x) + abs(point->y - target->y) == 1)
	{
		delete target;
		return true;
	}
	else
	{
		delete target;
		return isIgnoreCorner;// 判断斜对角是否绊住
	}
}


std::vector<Point*> Astar::getSurroundPoints(const Point * point, bool isIgnoreCorner) 
{
	std::vector<Point *> surroundPoints;
	for (int x = point->x - 1; x <= point->x + 1; x++)
		for (int y = point->y - 1; y <= point->y + 1; y++)
			if (isCanreach(point, new Point(x, y), isIgnoreCorner, safe_distance))
				surroundPoints.push_back(new Point(x, y));
	return surroundPoints;
}