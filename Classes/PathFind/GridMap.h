#pragma once
#ifndef GRIDMAP_H_
#define GRIDMAP_H_

#include "cocos2d.h"
#include <vector>

typedef std::vector<std::vector<int>> dyadic_array;		//vector形式的二维数组

struct GridSize;

/**
	* @brief GridPoint是一个记录每个地图格点的结构体
	* @地图中每个格点占用的像素大小为 32 * 32 像素
	* @我们将整个地图划分为 128 * 128 个格点
	* @以此为基础来避免单位建筑的碰撞、重叠，同时配合A*寻路算法
*/
struct GridPoint {
	int _x, _y;
	GridPoint(int x = 0, int y = 0) :_x(x), _y(y) { }

/**
	* @brief 运算符的重载，判断格点是否重叠
	* @ 接下来的几个友元函数执行格点的加减操作，此处不做赘述
	*/
	bool operator==(const GridPoint& gp2) const;
	friend GridPoint operator+(const GridPoint& gp1, const GridPoint& gp2);
	friend GridPoint operator-(const GridPoint& gp1, const GridPoint& gp2);
	friend GridPoint operator-(const GridPoint& gp, const GridSize& gz);

/**
	* @brief 将所得的向量根据 sgn 函数处理为 ({-1, 0, 1}, {-1, 0, 1})
	* @return 返回处理后的向量
	*/
	GridPoint getDirectionVec();
};

typedef GridPoint GridVec;

typedef std::vector<GridPoint> GridPath;

struct GridSize
{
	int width;
	int height;
	GridSize(int _width = 0, int _height = 0) : width(_width), height(_height) {};
	friend GridSize operator/(const GridSize& gz, int s);
};

/**
	* @brief 该结构体记录选择的格点区域(范围)
*/
struct GridRect
{
	GridPoint center;			//格点区域中心坐标
	GridPoint lower_left;		//格点区域的左下角坐标
	GridSize size;				//格点区域的尺寸

	GridRect(GridPoint _center, GridSize _size);
};

/**
	* @brief GridMap相当于一个将TMX地图的信息投影到一个透明的地图层的类
	* @用于将地图所有要处理的信息进行汇总、处理
*/
class GridMap:public cocos2d::Ref{
public:
/**
	* @brief 将TMXTiledMap中的图层信息加载到格点地图上
	* @return 指向GridMap的指针
	*/
	static GridMap* create(const cocos2d::TMXTiledMap * tiled_map);

/**
	* @brief 用TMXTiledMap对格点地图执行初始化操作
	* @将地图上的可用点在_gmap上标注为0，被占据点标注为1
	* @return 若初始化成功则返回true,反之false
	*/
	bool initWithTiledMap(const cocos2d::TMXTiledMap * tiled_map);

/**
	* @brief 将GridPoint的坐标转换成Point坐标
	* @param grid_point 要转换的GridPoint坐标
	* @return 相应的Point坐标
	*/
	cocos2d::Point getPoint(const GridPoint& grid_point);

/**
	* @brief 将Point的坐标转换成GridPoint坐标
	* @param point 要转换的Point坐标
	* @return 相应的GridPoint坐标
	*/
	GridPoint getGridPoint(const cocos2d::Point& point);

/**
	* @brief 将带有坐标偏移量Offset的Point的坐标转换成GridPoint坐标
	* @param point 要转换的带偏移量的Point坐标
	* @return 相应的GridPoint坐标
	*/
	GridPoint getGridPointWithOffset(const cocos2d::Point& p);

/**
	* @brief 将GridPoint的坐标转换成反应坐标偏移量Point坐标
	* @param point 要转换的GridPoint坐标
	* @return 相应的带偏移量的Point坐标
	*/
	cocos2d::Point getPointWithOffset(const GridPoint& gp);

/**
	* @brief 判断单位是否到达了目标格点
	* @param cur_fp 单位目前所占据的Point坐标
	* @param dest_gp 单位当前所要前往的目标格点的GridPoint坐标
	* @return 如果抵达则返回true,反之false
	*/
	bool hasApproached(const cocos2d::Point& cur_fp, const GridPoint& dest_gp);

/**
	* @brief 单位到达某格点后调用该函数占据该格点，即改变_gmap与_umap的值
	* @param id 标注单位的id
	* @param pos 单位想要占用的格点位置
	* @param occupy_grid 单位是否在行进中占用该格点(除了飞机以外的所有单位均占据格点，即不能重叠)
	* @return 如果占用成功(说明格点当前可用)则返回true，反之返回false
	*/
	bool occupyPosition(int id, const GridPoint& pos, bool occupy_grid = true);

/**
	* @brief 单位到达某格点区域后调用该函数占据该格点区域，即改变_gmap与_umap的值
	* @param id 标注单位的id
	* @param grec 单位想要占用的格点位置
	* @param occupy_grid 单位是否在行进中占用该格点(除了飞机以外的所有单位均占据格点，即不能重叠)
	* @return 如果占用成功(说明格点当前可用)则返回true，反之返回false
	*/
	void occupyPosition(int id, const GridRect& grec, bool occupy_grid = true);

/**
	* @brief 单位到达某格点后调用该函数占据该格点，即改变_gmap与_umap的值
	* @param id 标注单位的id
	* @param pos 单位想要占用的格点位置
	* @param occupy_grid 单位是否在行进中占用该格点(除了飞机以外的所有单位均占据格点，即不能重叠)
	* @return 如果占用成功(说明格点当前可用)则返回true，反之返回false
	*/
	bool occupyPosition(int id, const cocos2d::Point& pos, bool occupy_grid = true);

/**
	* @brief 判断格点是否在地图区域内
	* @param gp 待检查格点的位置信息
	* @return 如果当前格点在地图范围内返回true，反之返回false
	*/
	bool checkPointInMap(const GridPoint& gp) const;

/**
	* @brief 判断格点是否在地图区域内
	* @param x 待检查格点的水平位置坐标
	* @param y 待检查格点的竖直位置坐标
	* @return 如果当前格点在地图范围内返回true，反之返回false
	*/
	bool checkPointInMap(int x, int y) const;

/**
	* @brief 在调用occupyPosition的时候检查该格点是否可用
	* @param gp 待检查格点的位置信息
	* @return 如果格点当前可用返回true，反之返回false
	*/
	bool checkPosition(const GridPoint & gp);

/**
	* @brief 在调用occupyPosition的时候检查该格点区域是否可用
	* @param gp 待检查格点区域的位置信息
	* @return 如果格点区域当前可用返回true，反之返回false
	*/
	bool checkPosition(const GridRect & grec);

/**
	* @brief 单位离开某格点后调用该函数离开该格点，改变_gmap与_umap的值
	* @param pos 单位想要离开的格点位置
	* @param occupy_grid 单位是否在行进中占用该格点
	*/
	void leavePosition(const GridPoint & pos, bool occupy_grid = true);

/**
	* @brief 单位离开某格点区域后调用该函数离开该格点区域，改变_gmap与_umap的值
	* @param grec 单位想要离开的格点区域
	* @param occupy_grid 单位是否在行进中占用该格点
	*/
	void leavePosition(const GridRect & grec, bool occupy_grid = true);

/**
	* @brief 寻找在目标格点附近的可用格点(当单位行进终点被占据时调用此函数)
	* @param origin_gp 目标格点
	* @return 符合条件的可用格点
	*/
	GridPoint findFreePositionNear(const GridPoint& origin_gp);

/**
	* @brief 获取_gmap，即可用格点和被占据格点的位置信息
	* @return _gmap
	*/
	const dyadic_array& getLogicalGridMap();

/**
	* @brief 获取_umap，即占据地图每个格点的单位id信息
	* @return _umap
	*/
	const dyadic_array& getUnitMap();

/**
	* @brief 获取_umap在某个格点范围内的数值，即在该格点范围内每个格点的单位id信息
	* @return 存储id信息的vector
	*/
	std::vector<int> getUnitIDAt(const GridRect& range) const;

private:
	dyadic_array _gmap;					//存储格点是否可以占用信息的二维数组
	dyadic_array _umap;					//存储占用格点的单位ID信息的二维数组
	int _map_width, _map_height;		//格点地图的宽度和高度
	int _tile_width, _tile_height;		//每块格点的宽度和高度

	cocos2d::Vec2 _offset_vec;			//偏移向量，其大小为(_tile_width / 2, _tile_height / 2)
};

#endif