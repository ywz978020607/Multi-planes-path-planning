#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<list>
const int kCost1 = 10;//邻格
const int kCost2 = 14;//斜格消耗
const int safe_distance = 0; //安全距离

struct Point {
	int x, y;
	int F, G, H;
	Point *parent;
	Point(int _x, int _y) :x(_x), y(_y), F(0), G(0), H(0), parent(NULL)
	{
	}
};

class Astar {
public:
	void InitAstar(std::vector<std::vector<int>>&_maze);
	std::list<Point *> GetfPath(Point &startPoint, Point &endPoint, bool isIgnoreCorner);
	std::list<Point *> GetPath(Point &startPoint, Point &endPoint, bool isIgnoreCorner); //起终点，是否能斜着走
private:
	Point *findPath(Point &startPoint, Point &endPoint, bool isIgnoreCorner);
	Point *refindPath(Point &startPoint, Point &endPoint, bool isIgnoreCorner);
	std::vector<Point *>getSurroundPoints(const Point *point, bool isIgnoreCorner) ;
	bool isCanreach(const Point *point, const Point *target, bool isIgnoreCorner, int safe_d) const;//judge if there is a solution
	Point *isInList(Point * curPoint, const Point *point)const;
	//Point *getLeastFpoint(Point * point, bool isIgnoreCorner);   //return Fmin point from openlist,F=G+H
	int calcG(Point *temp_start, Point *point);
	int calcH(Point *point, Point *end);
	int calcF(Point *point);

private:
	std::vector<std::vector<int>> maze;
	std::list<Point *> openList;
	std::list<Point *> closeList;
};

class point_queue {
public:
	struct que_point {
		Point *data;
		struct que_point *next;
		struct que_point *last;
	};
	point_queue() 
	{
		pfront = NULL;
		prear = NULL;
	}
	bool empty()
	{
		if (pfront == NULL || prear == NULL)
		{
			pfront = NULL;
			prear = NULL;
			return 1;
		}
		else
			return 0;
	}
	void pop() {
		if (!empty())
		{
			pfront = pfront->next;
			if(pfront!=NULL)
			pfront->last = NULL;
		}
		if (pfront == NULL)
			prear = NULL;
	}
	Point *front()
	{
		if (!empty())
		{
			return pfront->data;
		}
		else
			return NULL;
	}
	int find_point(Point *input)
	{
		if (!empty())
		{
			struct que_point *temp = prear;
			while (temp != NULL && (temp->data->x != input->x || temp->data->y != input->y))
				temp = temp->last;
			if (temp != NULL)
				return temp->data->F;
			else
				return 0;
		}
		else
			return 0;
	}
	void push(Point *input)
	{
		if (empty())
		{
			struct que_point * temp = new struct que_point;
			temp->data = input;
			temp->next = NULL;
			temp->last = NULL;
			prear = temp;
			pfront = temp;
		}
		else
		{
			struct que_point * temp = new struct que_point;
			temp->data = input;
			temp->next = NULL;
			temp->last = prear;
			prear->next = temp;
			prear = prear->next;
		}
	}
	void del(Point *input)
	{
		struct que_point *temp = prear;
		while (temp != NULL &&temp!=pfront && (temp->data->x != input->x || temp->data->y != input->y))
			temp = temp->last;
		if (temp != NULL&& temp->data->x != input->x && temp->data->y != input->y)
		{
			//若非头指针也非尾指针
			if (temp != pfront&&temp != prear)
			{
				if (temp->last != NULL)
					temp->last->next = temp->next;
				if (temp->next != NULL)
					temp->next->last = temp->last;
			}


			
			//删除前处理好头尾指针问题
			else if (temp == prear&&temp!=pfront)
			{
				prear = prear->last;
				prear->next = NULL;

			}
			else if (temp == pfront&&temp!=prear)
			{
				pfront = pfront->next;
				pfront->last = NULL;

			}
			else if (temp == pfront&&temp == prear)
			{
				pfront = NULL;
				prear = NULL;
			}
			
			delete temp;

		}
		
		/*struct que_point *temp = pfront;
		while (!empty() && temp != NULL&&temp->data != cur)
			temp = temp->next;
		if (temp != NULL&&temp != pfront)
		{
			temp->last->next = temp->next;
			temp->next->last = temp->last;
			delete temp;
		}
		*/
	}
private:
	struct que_point *pfront;
	struct que_point *prear;
};