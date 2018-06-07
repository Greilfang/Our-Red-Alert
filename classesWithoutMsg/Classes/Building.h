#pragma once
#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "cocos2d.h"
#include "Unit.h"

USING_NS_CC;
//建筑物类,继承于Unit
class Building : public Unit
{
private:
	int id;
	int type;
public:
	Layer * getLayer();
	TMXTiledMap* getMap();
	static Building* create(const std::string & filename);
	//type表示建筑物的类型，0为基地，1为兵营，不同类的监听器调用不同的菜单
	void setType(int);
	int getType();
	void  addListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};





#endif // !__BUILDING_H__

