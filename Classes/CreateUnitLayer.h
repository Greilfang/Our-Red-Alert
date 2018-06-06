#ifndef __CREATEUNITLAYER_H__
#define __CREATEUNITLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Unit.h"
USING_NS_CC;

class CreateUnitLayer :public cocos2d::Layer
{
public:	
	Layout * layout;
	UnitManager * unit_manager;
	virtual bool init() = 0;	
};

class BaseLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *Military;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static BaseLayer * create();
};

class MilitaryCampLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *airplane;
	cocos2d::ui::Button *tank;
	cocos2d::ui::Button *soldier;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static MilitaryCampLayer * create();

};
#endif
