#pragma once
#ifndef GRIDMAP_H_
#define GRIDMAP_H_

#include "cocos2d.h"
#include <vector>
typedef std::vector<std::vector<int>> dyadic_array;


struct GridPoint {
	int _x, _y;
	GridPoint(int x, int y) :_x(x), _y(y) { }
};

typedef GridPoint GridVec;

typedef std::vector<GridPoint> GridPath;


class GridMap:public cocos2d::Ref{
public:
	static GridMap* create(const cocos2d::TMXTiledMap * tiled_map);
	bool initWithTiledMap(const cocos2d::TMXTiledMap * tiled_map);

	cocos2d::Point getPoint(const GridPoint& great_point);
	GridPoint getGridPoint(const cocos2d::Point& point);
	void occupyPosition(const GridPoint& pos);
	void occupyPosition(const cocos2d::Point& pos);

	const dyadic_array& getLogicalGridMap();
private:
	dyadic_array _gmap;
	int _map_width, _map_height;
	int _tile_width, _tile_height;
};

#endif