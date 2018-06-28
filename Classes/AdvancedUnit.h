#pragma once
#ifndef __ADVANCEDUNIT_H__
#define __ADVANCEDUNIT_H__


#include "Unit.h"

class Dog : public Unit
{
public:
	static Dog* create(const std::string& filename);
	/**
	*根据_direction来播放对应的动画
	*/
	void playMoveAnimation();
	/**
	*判断方向是否发生变化
	*/
	bool checkDirectionChange(Vec2 direction);
private:
	/**
	*现在move的方向
	*/
	int _direction = 0;
	/**
	*上一步move的方向
	*/
	int _preDirection = 0;
	/**
	*重写move() 根据momve方向播放动画
	*/
	void move() override;

	void setProperties() override;
};
class Fighter : public Unit
{
public:
	static Fighter* create(const std::string& filename);
	/**
	*根据move的方向获得需要旋转的角度
	*/
	float getRotation(Vec2 direction);
	/**
	*重写findPath 寻找忽略格点占用的路径
	*/
	GridPath findPath(const GridPoint& dest)const override;

private:
	void setProperties() override;
	//重写move 实现“飞行(移动不占用格点)”功能,并根据方向旋转
	void move() override;
};


class Tank : public Unit
{
public:
	static Tank* create(const std::string& filename);
	/**
	*根据move的方向获得需要旋转的角度
	*/
	float getRotation(Vec2 direction);

private:

	void setProperties() override;
	//重写move 根据move方向旋转
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