#include "GridMap.h"
#define DEBUG
#include "iostream"
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
	_umap = dyadic_array(_map_width, std::vector<int>(_map_height, 0));
	_offset_vec = Vec2(_tile_width / 2, _tile_height / 2);

	auto barrier_layer = tiled_map->getLayer("InfoLayer");
	for (int x = 0; x < _map_width; x++)
		for (int y = 0; y < _map_height; y++) {
			int tile_gid = barrier_layer->getTileGIDAt(Vec2(x, _map_height - 1 - y));
			if (tile_gid > 0)
				_gmap[x][y] = 1;
		}
	return true;
}

Point GridMap::getPoint(const GridPoint & great_point) {
	return Point(great_point._x * _tile_width, great_point._y * _tile_height);
}

GridPoint GridMap::getGridPoint(const Point & point) {
	return GridPoint(static_cast<int>(point.x) / _tile_width, static_cast<int>(point.y) / _tile_height);
}

GridPoint GridMap::getGridPointWithOffset(const Point & p)
{
	return getGridPoint(p + _offset_vec);
}

cocos2d::Point GridMap::getPointWithOffset(const GridPoint & gp)
{
	return getPoint(gp) + _offset_vec;
}

bool GridMap::hasApproached(const cocos2d::Point & cur_fp, const GridPoint & dest_gp)
{
	cocos2d::Point dest_fp = getPointWithOffset(dest_gp);
	if ((dest_fp - cur_fp).length() < 5)
		return true;
	return false;
}

bool GridMap::occupyPosition(int id, const GridPoint & pos, bool occupy_grid) {
	if (occupy_grid)
	{
		if (checkPosition(pos)) {
			_gmap[pos._x][pos._y] = 1;
			_umap[pos._x][pos._y] = id;
#ifdef DEBUG
			std::cout << "GridPoint:" << pos._x << "," << pos._y << std::endl;
#endif // DEBUG
			return true;
		}
		return false;
	}
	else
	{
		_umap[pos._x][pos._y] = id;
		return true;
	}
	return false;
}

void GridMap::occupyPosition(int id, const GridRect& grec, bool occupy_grid)
{
	
	for (int x = grec.lower_left._x; x <= grec.lower_left._x + grec.size.width; x++)
	{
		for (int y = grec.lower_left._y; y <= grec.lower_left._y + grec.size.height; y++)
		{
			if(occupy_grid)
				_gmap[x][y] = 1;
			_umap[x][y] = id;
#ifdef DEBUG
			std::cout << "GridPoint:" << x << "," << y << std::endl;
#endif // DEBUG
		}
	}
}

bool GridMap::occupyPosition(int id, const cocos2d::Point & pos, bool occupy_grid) {
	return occupyPosition(id,getGridPoint(pos),occupy_grid);
}

bool GridMap::checkPointInMap(const GridPoint & gp) const
{
	return checkPointInMap(gp._x, gp._y);
}

bool GridMap::checkPointInMap(int x, int y) const
{
	return (x >= 0 && x < _map_width && y >= 0 && y < _map_height);
}

bool GridMap::checkPosition(const GridPoint & gp)
{
	if (gp._x >= 0 && gp._x < _map_width && gp._y >= 0 && gp._y < _map_height && _gmap[gp._x][gp._y] == 0)
		return true;
	return false;
}

bool GridMap::checkPosition(const GridRect& grec)
{
	for (int x = grec.lower_left._x; x <= grec.lower_left._x + grec.size.width; x++)
	{
		for (int y = grec.lower_left._y; y <= grec.lower_left._y + grec.size.height; y++)
		{
			if (x < 0 || x >= _map_width || y < 0 || y >= _map_height || _gmap[x][y] == 1)
				return(false);
		}
	}
	return(true);
}

void GridMap::leavePosition(const GridPoint & pos, bool occupy_grid)
{
	if(occupy_grid)
		_gmap[pos._x][pos._y] = 0;
	_umap[pos._x][pos._y] = 0;
}

void GridMap::leavePosition(const GridRect& grec, bool occupy_grid)
{
	for (int x = grec.lower_left._x; x <= grec.lower_left._x + grec.size.width; x++)
	{
		for (int y = grec.lower_left._y; y <= grec.lower_left._y + grec.size.height; y++)
		{
			if (occupy_grid)
				_gmap[x][y] = 0;
			_umap[x][y] = 0;
		}
	}
}

GridPoint GridMap::findFreePositionNear(const GridPoint & origin_gp)
{
	if (checkPosition(origin_gp))
		return(origin_gp);

	for (int i = 1; i < _map_height + _map_width; i++)
	{
		GridPath dir_vec = { GridVec(1, 1), GridVec(1, -1), GridVec(-1, -1), GridVec(-1, 1) };
		GridPoint gp = origin_gp + GridVec(-i, 0);
		for (int dir = 0; dir < 4; dir++)
			for (int j = 1; j <= i; j++)
			{
				gp = gp + dir_vec[dir];
				if (checkPosition(gp))
					return(gp);
			}

	}
	return GridPoint(-1, -1);
}

const dyadic_array & GridMap::getLogicalGridMap() {
	return _gmap;
}

const dyadic_array& GridMap::getUnitMap()
{
	return(_umap);
}
std::vector<int> GridMap::getUnitIDAt(const GridRect & grec) const
{
	std::vector<int> unit_ids;
	int i = 0;
	for (int x = grec.lower_left._x; x <= grec.lower_left._x + grec.size.width; x++)
		for (int y = grec.lower_left._y; y <= grec.lower_left._y + grec.size.height; y++)
			if (checkPointInMap(x, y) && _umap[x][y])
			{
				unit_ids.push_back(_umap[x][y]);
				i++;
			}
	return unit_ids;
}



bool GridPoint::operator==(const GridPoint & gp2) const
{
	return(_x == gp2._x && _y == gp2._y);
}

GridPoint GridPoint::getDirectionVec()
{
	auto sgn = [](int x)->int { return x ? (x > 0 ? (1) : (-1)) : 0; };
	return GridPoint(sgn(_x), sgn(_y));
}

GridPoint operator+(const GridPoint & gp1, const GridPoint & gp2)
{
	return GridPoint(gp1._x + gp2._x, gp1._y + gp2._y);
}

GridPoint operator-(const GridPoint & gp1, const GridPoint & gp2)
{
	return GridPoint(gp1._x - gp2._x, gp1._y - gp2._y);
}

GridPoint operator-(const GridPoint & gp, const GridSize & gz)
{
	return GridPoint(gp._x - gz.width, gp._y - gz.height);
}

GridSize operator/(const GridSize & gz, int s)
{
	return GridSize(gz.width / s, gz.height / s);
}

GridRect::GridRect(GridPoint _center, GridSize _size)
{
	center = _center;
	size = _size;
	lower_left = GridPoint(_center._x - size.width / 2, _center._y - size.height / 2);
}

