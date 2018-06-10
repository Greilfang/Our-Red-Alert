#pragma once
#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "cocos2d.h"
#include "Unit.h"
#include "CreateUnitLayer.h"

USING_NS_CC;
class CombatScene;
//建筑物类,继承于Building

class Building : public Unit
{
private:
	GridRect rec;
	void setProperties() = 0;
public:
	Building() : rec(GridRect{ 0,0 }) {	Unit();	};
	void addToGmap(Point p) override;
	bool layer_is_created = false;
	void setListener() = 0;
	
};
class Base : public Building
{
private:
	void setProperties() override;
public:
	BaseLayer * baselayer = nullptr;
	static Base* create(const std::string & filename);
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

class MilitaryCamp : public Building
{
private:
	void setProperties() override;
public:
	MilitaryCampLayer * militaryCampLayer = nullptr;
	static MilitaryCamp* create(const std::string & filename);
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};



#endif // !__BUILDING_H__

