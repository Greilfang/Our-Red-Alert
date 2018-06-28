#pragma once
#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "cocos2d.h"
#include "Unit.h"
#include "CreateUnitLayer.h"

USING_NS_CC;
class CombatScene;

/**
*建筑基类，继承自Unit
*/
class Building : public Unit{
private:
	void setProperties() = 0;
public:
	bool layer_is_created = false;
	Point createPosition = Point(0, 0);
	/**
	*用于播放动画的建筑载体
	*/
	Sprite * building;
	/**
	*建筑当前状态:0->不处于生产状态;1->正在生产;2->当前单位生产结束
	*/
	int state = 0;
	/**
	*当前生产单位的类型
	*/
	int cur_prod;
	/**
	*当前生产单位所需消耗时间
	*/
	int prod_period;
	/**
	*生产当前单位时已经消耗的时间
	*/
	int prod_process;
	/**
	*记录各类型单位生产所需消耗时间
	*/
	std::map<int, int> period_map;
	/**
	*生产序列
	*/
	std::vector<int> prod_list;
	/**
	*生产进度条
	*/
	Bar* prod_bar = nullptr;
	/**
	*设置生产菜单的可见性
	*/
	void setLayerVisible(Layer *);
	/**
	*建筑根据格点矩形占用格点
	*/
	void addToGmap(Point p) override;
	/**
	*为不同的建筑分配不同的监听器
	*/
	virtual void setListener();
	/**
	*创建血条和生产进度条
	*/
	virtual void initBar();
	/**
	*更新建筑状态
	*/
	virtual void update(float f);
	/**
	*将_unit_type加入生产序列(用于非建筑类生产)
	*/
	void startProduce(int _unit_type);
	/**
	*将_unit_type加入生产序列,并将createPosition设为_point(鼠标点击位置)(用于建筑类生产)
	*/
	void startProduce(int _unit_type, Point _point);
	/**
	*返回该建筑周围未被占用的位置
	*/
	Point findFreePosition();
};

class Base : public Building
{
private:
	void setProperties() override;
public:	
	/**
	*可创建范围半径
	*/
	float construct_range = 500;
	static Size size;	
	BaseLayer * baselayer = nullptr;

	/**
	*设置baselayer中各按钮的可用性
	*/
	void setMenuEnable(bool able);

	static Base* create(const std::string & filename);	
	void update(float f);

	/**
	*给Base类对象添加监听器(即点击事件+弹出菜单)
	*/
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

	/**
	*给MilitaryCamp类对象添加监听器(即点击事件+弹出菜单)
	*/
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

	/**
	*给TankFactory类对象添加监听器(即点击事件+弹出菜单)
	*/
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

	static Mine* create(const std::string & filename);

	/**
	*每秒金钱增加量增加amount
	*/
	void setIncreasingAmount(int amount);
};

class PowerPlant : public Building
{
private:
	void setProperties() override;;
public:
	static Size size;

	static PowerPlant* create(const std::string & filename);

	/**
	*电力最大负荷增加delta
	*/
	void addMax_Power(int delta);
};

#endif // !__BUILDING_H__

