#pragma once
#ifndef GRIDMAP_H_
#define GRIDMAP_H_

#include "cocos2d.h"
#include <vector>
typedef std::vector<std::vector<int>> dyadic_array;
struct GridSize;
struct GridPoint {
	int _x, _y;
	GridPoint(int x = 0, int y = 0) :_x(x), _y(y) { }

	bool operator==(const GridPoint& gp2) const;
	friend GridPoint operator+(const GridPoint& gp1, const GridPoint& gp2);
	friend GridPoint operator-(const GridPoint& gp1, const GridPoint& gp2);
	friend GridPoint operator-(const GridPoint& gp, const GridSize& gz);
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

struct GridRect
{
	GridPoint center;
	GridPoint lower_left;
	GridSize size;

	GridRect(GridPoint _center, GridSize _size);
};

class GridMap:public cocos2d::Ref{
public:
	static GridMap* create(const cocos2d::TMXTiledMap * tiled_map);
	bool initWithTiledMap(const cocos2d::TMXTiledMap * tiled_map);

	cocos2d::Point getPoint(const GridPoint& great_point);
	GridPoint getGridPoint(const cocos2d::Point& point);

	GridPoint getGridPointWithOffset(const cocos2d::Point& p);
	cocos2d::Point getPointWithOffset(const GridPoint& gp);
	bool hasApproached(const cocos2d::Point& cur_fp, const GridPoint& dest_gp);
	bool occupyPosition(int id, const GridPoint& pos, bool occupy_grid = true);
	void occupyPosition(int id, const GridRect& grec, bool occupy_grid = true);
	bool occupyPosition(int id, const cocos2d::Point& pos, bool occupy_grid = true);

	bool checkPointInMap(const GridPoint& gp) const;
	bool checkPointInMap(int x, int y) const;

	bool checkPosition(const GridPoint & gp);
	bool checkPosition(const GridRect & grec);

	void leavePosition(const GridPoint & pos, bool occupy_grid = true);
	void leavePosition(const GridRect & grec, bool occupy_grid = true);
	GridPoint findFreePositionNear(const GridPoint& origin_gp);

	const dyadic_array& getLogicalGridMap();
	const dyadic_array& getUnitMap();

	std::vector<int> getUnitIDAt(const GridRect& range) const;

private:
	dyadic_array _gmap;
	dyadic_array _umap;
	int _map_width, _map_height;
	int _tile_width, _tile_height;

	//Æ«ÒÆÏòÁ¿
	cocos2d::Vec2 _offset_vec;
};

#endif