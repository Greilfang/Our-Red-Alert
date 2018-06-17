#ifndef __CREATEUNITLAYER_H__
#define __CREATEUNITLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameMessage.pb.h"
#include "Unit.h"
USING_NS_CC;
class CreateUnitLayer :public cocos2d::Layer
{
public:
	GameMessageSet * msgs = nullptr;
	UnitManager * unit_manager;
	virtual bool init() = 0;
	//设置单位所在的层，地图指针和Sprite监听器
	virtual void set(TMXTiledMap *, Layer*, EventListenerTouchOneByOne *) = 0;
	Layer * getLayer();
	TMXTiledMap * getMap();
	virtual void realInit() = 0;
protected:
	Layer * combatScene;
	TMXTiledMap * tiledMap;
	EventListenerTouchOneByOne * spriteTouchListener;
	
};
//兵营菜单，只有设置了单位所在的地图指针，层，监听器（精灵监听器）才会realInit
class MilitaryCampLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *airplane;
	cocos2d::ui::Button *tank;
	cocos2d::ui::Button *soldier;
	cocos2d::ui::Button *exit;
public:
	void set(TMXTiledMap *, Layer *, EventListenerTouchOneByOne *);
	virtual bool init();
	void realInit();
	CREATE_FUNC(MilitaryCampLayer);
};
//基地菜单，只有设置了单位所在的地图指针，层，监听器（精灵监听器）才会realInit
class BaseLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *Military;
	cocos2d::ui::Button *exit;
public:
	void set(TMXTiledMap *, Layer*, EventListenerTouchOneByOne *);
	virtual bool init();
	void realInit();
	CREATE_FUNC(BaseLayer);
};
#endif
