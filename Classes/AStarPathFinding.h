#pragma once
#ifndef A_STAR_PATH_FINDING_H_
#define A_STAR_PATH_FINDING_H_

#include <GridMap.h>
#include "cocos2d.h"
#include <vector>

class Grid;

typedef std::vector<std::vector<Grid>> grid_matrix;
typedef std::vector<Grid *> grid_vec;
enum { VACANT, OCCUPIED, START, DESTINATION, OPEN, CLOSE };

class Grid {
public:
	Grid();
	int getFlag()const { return _flag; }
	void setFlag(int flag) { _flag = flag; }
	int getX()const { return _x; }
	int getY()const { return _y; }
	void setPosition(int x, int y) { _x = x; _y = y; }
	int getG()const { return _g; }
	void setG(int g) { _g = g; }
	int getH()const { return _h; }
	void setH(int h) { _h = h; }
	int getF()const { return _f; }
	void setF(int f) { _f = f; }
	Grid * getParent()const { return _parent; }
	void setParent(Grid* parent) { _parent = parent; }
private:
	int _x;
	int _y;
	int _flag;//网格的标志
	int _g;//G值，欧几里得距离(从起点到指定方块的移动代价)
	int _h;//H值，曼哈顿距离(从该点到终点的估算代价，启发函数)
	int _f;//行走代价 f = g + h
	Grid* _parent;//父节点
};

class PathFinding {
public:
	PathFinding(const dyadic_array& map, GridPoint start, GridPoint destination);
	//寻找路径
	void searchForPath();
	//生成路径
	void generatePath();
	GridPath getPath()const { return _path; }
private:
	int _width;									//地图宽度
	int _height;								//地图高度
	std::vector<std::vector<Grid>>  _map;		//地图矩阵
	Grid * _starting_point;						//起始点
	Grid * _terminal_point;						//终点

	grid_vec _open_list;						//开放列表
	grid_vec _close_list;						//封闭列表
	GridPath _path;								//最终路径

	//从当前开放列表中选取行走代价F最小的的格点作为下一步运算的格点
	Grid * getNextGrid();

	//检查当前格点周围八个格点的状态
	void checkSurroundedGrid(Grid & grid);

	//判断当前格点是否在地图范围内
	bool isInMapRange(cocos2d::Point & grid);

	//判断是否为转角，不可斜着穿过障碍物(类似象棋中马的移动不能蹩马腿)
	bool isCorner(Grid & g1, Grid & g2);

	//判断该格点是否可用
	bool isAvailable(Grid & grid);

	//计算欧几里得距离(即G值)
	int getEuclideanDistance(Grid & g1, Grid & g2);

	//计算曼哈顿距离(即H值)
	int getManhattanDistance(Grid & g1, Grid & g2);

	//从开放列表中移除已加入封闭列表的格点
	void removeFromOpenList(Grid * grid);
};


#endif