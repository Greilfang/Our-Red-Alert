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
	int state = 0;
	int cur_prod;
	int prod_process;
	int prod_period;
	std::map<int, int> period_map;
	std::vector<int> prod_list;
	Bar* prod_bar = nullptr;

	Building() : rec(GridRect{ 0,0 }) {	Unit();	};
	void addToGmap(Point p) override;
	bool layer_is_created = false;
	virtual void setListener();
	void setLayerVisible(Layer *);
	virtual void initBar();
	virtual void update(float f);
	void startProduce(int _unit_type);
	void startProduce(int _unit_type, Point _point);
	Point findFreePosition();
	Point createPosition = Point(0,0);
};

class Base : public Building
{
private:
	void setProperties() override;	
public:
	float construct_range = 300;
	static Size size;	
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
	static Size size;
	MilitaryCampLayer * militaryCampLayer = nullptr;
	static MilitaryCamp* create(const std::string & filename);
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

class TankFactary : public Building
{
private:
	void setProperties() override;
public:
	static Size size;
	TankFactaryLayer * tankFactaryLayer = nullptr;
	static TankFactary* create(const std::string & filename);
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

class Mine : public Building
{
private:
	void setProperties() override;;
public:
	static Size size;
	void setIncreasingAmount(int amount);
	static Mine* create(const std::string & filename);
};

class PowerPlant : public Building
{
private:
	void setProperties() override;;
public:
	static Size size;
	void addMax_Power(int delta);
	static PowerPlant* create(const std::string & filename);
};

#endif // !__BUILDING_H__

