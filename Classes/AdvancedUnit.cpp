#include "AdvancedUnit.h"
#include "Const.h"
USING_NS_CC;

Fighter* Fighter::create(const std::string& filename)
{
	Fighter *ret = new (std::nothrow) Fighter();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}

float Fighter::getRotation(Vec2 direction)
{
	float x = direction.x;
	float y = direction.y;
	float radian = atan2f(y, x);
	float degrees = CC_RADIANS_TO_DEGREES(radian);
	return 90 - degrees;

}

void Fighter::setProperties()
{
	type = 1;

	ATK = 10;
	speed = 6.5f;

	mobile = true;
	attack_range = GridSize(7, 7);
	z_index = 15;

}

void Fighter::move()
{
	auto esp = (grid_map->getPointWithOffset(_cur_dest) - getPosition()).getNormalized();
	Point next_pos = getPosition() + esp * speed;
	GridPoint next_gp = grid_map->getGridPoint(next_pos);
	setRotation(getRotation(esp));
	if (_cur_pos == next_gp)
		setPosition(next_pos);
	else {
		grid_map->occupyPosition(id, next_gp, false);
		setPosition(next_pos);
		grid_map->leavePosition(_cur_pos, false);
		_cur_pos = next_gp;
	}

	if (hasArrivedFinalDest()) {
		if (_grid_path.size()) {
			_cur_dest = _grid_path.back();
			_grid_path.pop_back();
		}
		else {
			if (grid_map->checkPosition(_cur_dest)) {
				grid_map->occupyPosition(id, next_gp, true);
				is_moving = false;
			}
			else {
				_cur_dest = grid_map->findFreePositionNear(_cur_dest);
				_final_dest = _cur_dest;
			}
		}
	}
}

GridPath Fighter::findPath(const GridPoint & dest) const
{
	return GridPath{ dest };
}

Tank* Tank::create(const std::string& filename)
{
	Tank *ret = new (std::nothrow) Tank();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}

float Tank::getRotation(Vec2 direction)
{
	float x = direction.x;
	float y = direction.y;
	float radian = atan2f(y, x);
	float degrees = CC_RADIANS_TO_DEGREES(radian);
	return 90 - degrees;
}

void Tank::setProperties()
{
	type = 2;

	speed = 5.5f;
	ATK = 7;

	z_index = 10;
	attack_range = GridSize(5, 5);
	mobile = true;
}

void Tank::move()
{
	//esp为当前格点指向当前终点的单位向量
	auto esp = (_cur_dest_point - getPosition()).getNormalized();
	setRotation(getRotation(esp));
	Point next_position = esp * speed + getPosition();
	GridPoint next_gpos = grid_map->getGridPoint(next_position);

	if (_cur_pos == next_gpos)
	{
		setPosition(next_position);
	}
	else if (grid_map->occupyPosition(id, next_gpos))
	{
		setPosition(next_position);
		grid_map->leavePosition(_cur_pos);
		_cur_pos = next_gpos;
	}
	else
	{
		_cur_dest = _cur_pos;
		setCurDestPoint(_cur_dest);
		Point final_dest = grid_map->getPointWithOffset(_final_dest);
		if (camp == unit_manager->player_id && (final_dest - getPosition()).length() > REFIND_PATH_MAX_RANGE) {
			if (!is_delaying)
				tryToSearchForPath();
		}
	}
	if (hasArrivedFinalDest()) {
		if (_grid_path.size()) {
			_cur_dest = _grid_path.back();
			setCurDestPoint(_cur_dest);
			_grid_path.pop_back();
		}
		else
			is_moving = false;
	}
}


Soldier* Soldier::create(const std::string& filename)
{
	Soldier *ret = new (std::nothrow) Soldier();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}

void Soldier::setProperties()
{
	type = 3;

	speed = 3.5f;
	ATK = 5;
	z_index = 10;
	attack_range = GridSize(5, 5);
	mobile = true;
}

Dog * Dog::create(const std::string & filename)
{
	Dog *ret = new (std::nothrow) Dog();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}

void Dog::playMoveAnimation()
{
	Animation * move;
	std::string dir;
	switch (_direction)
	{
	case 1:dir = "Right"; break;
	case 2:dir = "TopRight"; break;
	case 3:dir = "Up"; break;
	case 4:dir = "TopLeft"; break;
	case 5:dir = "Left"; break;
	case 6:dir = "LowerRight"; break;
	case 7:dir = "Down"; break;
	case 8:dir = "LowerLeft"; break;
	}
	_preDirection = _direction;
	std::string actionName = "DogMove" + dir;
	move = AnimationCache::getInstance()->getAnimation(actionName);
	auto animate = Animate::create(move);
	auto run = RepeatForever::create(animate);
	this->stopAllActions();
	this->runAction(run);

}

bool Dog::checkDirectionChange(Vec2 direction)
{
	float x = direction.x;
	float y = direction.y;
	Animation * move;
	if (y >= 0)
	{
		if (x > 0.92)
			_direction = 1;//Right
		else if (x > 0.38)
			_direction = 2;//TopRight
		else if (x > -0.38)
			_direction = 3;//Up
		else if (x > -0.92)
			_direction = 4;// TopLeft
		else
			_direction = 5;// Left
	}
	else
	{
		if (x > 0.92)
			_direction = 1;// Right
		else if (x > 0.38)
			_direction = 6;// LowerRight
		else if (x > -0.38)
			_direction = 7;// Down
		else if (x > -0.92)
			_direction = 8;// LowerLeft
		else
			_direction = 5;// Left
	}
	return _preDirection != _direction;
}

void Dog::setProperties()
{
	type = 4;

	speed = 3.0f;
	ATK = 10;
	z_index = 10;
	attack_range = GridSize(1, 1);
	mobile = true;
}

void Dog::move()
{
	auto esp = (_cur_dest_point - getPosition()).getNormalized();
	if (checkDirectionChange(esp))
	{
		playMoveAnimation();
	}
	Point next_position = esp * speed + getPosition();
	GridPoint next_gpos = grid_map->getGridPoint(next_position);

	if (_cur_pos == next_gpos)
	{
		setPosition(next_position);
	}
	else if (grid_map->occupyPosition(id, next_gpos))
	{
		setPosition(next_position);
		grid_map->leavePosition(_cur_pos);
		_cur_pos = next_gpos;
	}
	else
	{
		_cur_dest = _cur_pos;
		setCurDestPoint(_cur_dest);
		Point final_dest = grid_map->getPointWithOffset(_final_dest);
		if (camp == unit_manager->player_id && (final_dest - getPosition()).length() > REFIND_PATH_MAX_RANGE) {
			if (!is_delaying)
				tryToSearchForPath();
		}
	}
	if (hasArrivedFinalDest()) {
		
		if (_grid_path.size()) {
			_cur_dest = _grid_path.back();
			setCurDestPoint(_cur_dest);
			_grid_path.pop_back();
		}
		else
		{
			is_moving = false;
			this->stopAllActions();
			_preDirection = 0;
		}
	}
}