#include "AStarPathFinding.h"

USING_NS_CC;

const int DIR[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1, 1} };
const int DISTANCE = 10;

Grid::Grid()
{
	_x = _y = 0;
	_flag = VACANT;
	_g = _h = _f = 0;
	_parent = nullptr;
}

PathFinding::PathFinding(dyadic_array & map, cocos2d::Point start, cocos2d::Point destination)
{
	_width = map.size();
	_height = map[0].size();

	std::vector<Grid> col(_height, Grid());		//先初始化_map的一列
	_map.assign(_width, col);					//初始化_map

	for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _height; ++j) {
			_map[i][j].setPosition(i, j);
			int flag = map[i][j] == 0 ? VACANT : OCCUPIED;
			_map[i][j].setFlag(flag);
		}

	_map[start.x][start.y].setFlag(START);
	_map[destination.x][destination.y].setFlag(DESTINATION);

	_starting_point = &_map[start.x][start.y];
	_terminal_point = &_map[destination.x][destination.y];
}

void PathFinding::searchForPath()
{
	Grid * grid_now = nullptr;
	_open_list.push_back(_starting_point);

	while (_terminal_point->getFlag() != CLOSE && !_open_list.empty()) {
		grid_now = getNextGrid();
		checkSurroundedGrid(*grid_now);
		grid_now->setFlag(CLOSE);
		_close_list.push_back(grid_now);
		removeFromOpenList(*grid_now);
	}
}

void PathFinding::generatePath()
{
	Grid * g = _terminal_point;
	while (g->getParent() != nullptr) {
		_path.push_back(GridPoint(g->getX(),g->getY()));
		g = g->getParent();
	}
}

Grid * PathFinding::getNextGrid()
{
	Grid * next_grid = _open_list[0];
	for (auto &p : _open_list)
		if (p->getF() < next_grid->getF())
			next_grid = p;
	return next_grid;
}

void PathFinding::checkSurroundedGrid(Grid & grid)
{
	for (int i = 0; i < 8; ++i) {
		Point current_grid(grid.getX() + DIR[i][0], grid.getY() + DIR[i][1]);
		int cx = current_grid.x, cy = current_grid.y;

		if (isInMapRange(current_grid) && !isCorner(grid, _map[cx][cy]) && isAvailable(_map[cx][cy])) {
			int g = getEuclideanDistance(_map[cx][cy], grid);
			_map[cx][cy].setH(getManhattanDistance(_map[cx][cy], *_terminal_point));

			if (_map[cx][cy].getFlag() == OPEN) {
				if(g < _map[cx][cy].getG()){
					_map[cx][cy].setParent(&grid);
					_map[cx][cy].setG(g);
					_map[cx][cy].setF(g + _map[cx][cy].getH());
				}
			}
			else {
				_map[cx][cy].setFlag(OPEN);
				_map[cx][cy].setParent(&grid);
				_map[cx][cy].setG(g);
				_map[cx][cy].setF(g + _map[cx][cy].getH());
			}
		}
	}
}

bool PathFinding::isInMapRange(Point & grid)
{
	if (grid.x >= 0 && grid.x <= _width && grid.y >= 0 && grid.y <= _height)
		return true;
	return false;
}

bool PathFinding::isCorner(Grid & g1, Grid & g2)
{
	if (g1.getX() == g2.getX() || g1.getY() == g2.getY())
		return false;
	else if (_map[g1.getX()][g2.getY()].getFlag() == OCCUPIED || _map[g1.getY()][g2.getX()].getFlag() == OCCUPIED)
		return true;
	else return false;
}

bool PathFinding::isAvailable(Grid & grid)
{
	if (grid.getFlag() == START || grid.getFlag() == CLOSE || grid.getFlag() == OCCUPIED)
		return false;
	return true;
}

int PathFinding::getEuclideanDistance(Grid & g1, Grid & g2)
{
	return g1.getX() == g2.getX() || g1.getY() == g2.getY() ? g2.getG() + 10 : g2.getG() + 14;
}

int PathFinding::getManhattanDistance(Grid & g1, Grid & g2)
{
	return (abs(g1.getX() - g2.getX()) + abs(g1.getY() - g2.getY())) * DISTANCE;
}

void PathFinding::removeFromOpenList(Grid & grid)
{
	auto iter = find(_open_list.begin(), _open_list.end(), grid);
	if (iter != _open_list.end())
	_open_list.erase(iter);
}
