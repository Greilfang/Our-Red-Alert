#pragma once
#ifndef __ADVANCEDUNIT_H__
#define __ADVANCEDUNIT_H__


#include "Unit.h"

class Dog : public Unit
{
public:
	static Dog* create(const std::string& filename);
	void playMoveAnimation();
	bool checkDirectionChange(Vec2 direction);
private:
	int _direction = 0;
	int _preDirection = 0;
	void setProperties() override;
	void move() override;
};
class Fighter : public Unit
{
public:
	static Fighter* create(const std::string& filename);

	float getRotation(Vec2 direction);

	GridPath findPath(const GridPoint& dest)const override;

private:

	void setProperties() override;
	//重载实现“飞行(移动不占用格点)”功能
	void move() override;
};


class Tank : public Unit
{
public:
	static Tank* create(const std::string& filename);

	float getRotation(Vec2 direction);

private:

	void setProperties() override;
	//重载实现旋转功能
	void move() override;
};

class Soldier : public Unit
{
public:
	static Soldier* create(const std::string& filename);
private:

	void setProperties() override;
};

#endif // !__ADVANCEDUNIT_H__