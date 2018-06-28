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

/**
	* @brief Grid类刻画并记录寻路过程中所需计算的格点的数值
*/
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

	int _flag;		//网格的标志(记录被占用、未被占用、起点、终点、开放、封闭六种状态)
	int _g;			//G值，欧几里得距离(从起点到指定方块的移动代价)
	int _h;			//H值，曼哈顿距离(从该点到终点的估算代价，启发函数)
	int _f;			//行走代价 f = g + h

	Grid* _parent;	//父节点
};


/**
	* @brief PathFinding类为寻路算法的核心内容
	* @提供了初始化路径、搜索路径、生成路径、返回路径四个主要接口
*/
class PathFinding {
public:
/**
	* @brief 构造函数，主要用于初始化格点信息与起止点
	* @param map 当_map值为1时表示可以通过，反之不能
	* @param start 记录起始点的位置
	* @param destination 记录终止点的位置
	*/
	PathFinding(const dyadic_array& map, GridPoint start, GridPoint destination);

/**
	* @brief 寻找可能的路径
	*/
	void searchForPath();

/**
	* @brief 生成一条路径
	*/
	void generatePath();

/**
	* @brief 将所得路径返回
	* @return 最优路径
	*/
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

/**
	* @brief 从当前开放列表中选取行走代价F最小的的格点作为下一步运算的格点
	* @return F值最小的格点
	*/
	Grid * getNextGrid();

/**
	* @brief 检查当前格点附近八个格点的状态
	* @param grid 当前所需检查的中心格点
	*/
	void checkSurroundedGrid(Grid & grid);

/**
	* @brief 判断当前格点是否在地图范围内
	* @param grid 当前所需检查的格点
	* @return 若在地图范围内则返回true，反之false
	*/
	bool isInMapRange(cocos2d::Point & grid);

/**
	* @brief 判断是否为转角，不可斜着穿过障碍物(类似象棋中马的移动不能蹩马腿)
	* @param g1 当前正在计算的格点
	* @param g2 可能的下一步的格点
	* @return 若该格点为转角则返回true，反之false
	*/
	bool isCorner(Grid & g1, Grid & g2);

/**
	* @brief 判断该格点是否可用(有无障碍物)
	* @param grid 当前正在计算的格点
	* @return 若该格点可用则返回true，反之false
	*/
	bool isAvailable(Grid & grid);

/**
	* @brief 计算欧几里得距离(G值)
	* @param g1 当前正在计算的格点
	* @param grid 邻格点
	* @return EuclideanDistance
	*/
	int getEuclideanDistance(Grid & g1, Grid & g2);

/**
	* @brief 计算曼哈顿距离(H值)
	* @param g1 当前正在计算的格点
	* @param g2 终点格点
	* @return ManhattanDistance
	*/	
	int getManhattanDistance(Grid & g1, Grid & g2);

/**
	* @brief 将已加入到封闭列表的格点从开放列表中移除
	* @param grid 想要从开放列表中移除的格点
	*/		
	void removeFromOpenList(Grid * grid);
};


#endif