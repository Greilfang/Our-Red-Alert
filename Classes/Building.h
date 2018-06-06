#pragma once
#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "cocos2d.h"
#include "Unit.h"
#include "CreateUnitLayer.h"

USING_NS_CC;
class CombatScene;
//建筑物类,继承于Unit
class Base : public Unit
{
private:
	void setProperties() override;
public:
	BaseLayer * baselayer = nullptr;
	bool layer_is_created = false;
	static Base* create(const std::string & filename);
	void set(TMXTiledMap *, Layer *, EventListenerTouchOneByOne *);
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

class MilitaryCamp : public Unit
{
private:
	void setProperties() override;
public:
	MilitaryCampLayer * militaryCampLayer = nullptr;
	bool layer_is_created = false;
	static MilitaryCamp* create(const std::string & filename);
	void set(TMXTiledMap *, Layer *, EventListenerTouchOneByOne *);
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};



#endif // !__BUILDING_H__

