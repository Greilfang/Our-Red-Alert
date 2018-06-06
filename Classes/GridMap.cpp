#include "GridMap.h"

USING_NS_CC;

GridMap * GridMap::create(const cocos2d::TMXTiledMap * tiled_map) {
	//ÈôÉêÇëÊ§°ÜÔò·µ»Ønullptr
	GridMap *ret = new(std::nothrow)GridMap();
	if (ret->initWithTiledMap(tiled_map)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool GridMap::initWithTiledMap(const cocos2d::TMXTiledMap * tiled_map) {
	if (tiled_map == nullptr)
		return false;

	_map_height = static_cast<int>(tiled_map->getMapSize().height);
	_map_width = static_cast<int>(tiled_map->getMapSize().width);
	_tile_height = static_cast<int>(tiled_map->getTileSize().height);
	_tile_width = static_cast<int>(tiled_map->getTileSize().width);
	_gmap = dyadic_array(_map_width, std::vector<int>(_map_height, 0));

	return true;
}

Point GridMap::getPoint(const GridPoint & great_point){
	return Point(great_point._x * _tile_width, great_point._y * _tile_height);
}

GridPoint GridMap::getGridPoint(const cocos2d::Point & point){
	return GridPoint(static_cast<int>(point.x) / _tile_width, static_cast<int>(point.y) / _tile_height);
}

void GridMap::occupyPosition(const GridPoint & pos){
	_gmap[pos._x][pos._y] = 1;
}

void GridMap::occupyPosition(const cocos2d::Point & pos){
	occupyPosition(getGridPoint(pos));
}

const dyadic_array & GridMap::getLogicalGridMap(){
	return _gmap;
}

