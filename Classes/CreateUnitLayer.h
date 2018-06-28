#ifndef __CREATEUNITLAYER_H__
#define __CREATEUNITLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Unit.h"
USING_NS_CC;

class Building;

/**
*创建单位菜单的基类,继承自Layer
*/
class CreateUnitLayer :public cocos2d::Layer
{
private:
	Point center_position;
public:	
	/**
	*创建建筑时的提示矩形
	*/
	Sprite * rec = nullptr;
	/**
	*提示矩形的大小,根据要创建建筑的大小变化
	*/
	Size rec_size;
	/**
	*提示矩形的中心位置(相对于屏幕,与鼠标位置对应)
	*/
	Point rec_center;
	/**
	*提示矩形的中心位置(相对于瓦片地图,与创建位置对应)
	*/
	Point rec_abs_center;
	/**
	*菜单所对应的建筑
	*/
	Building* center = nullptr;
	/**
	*表示该单位可以是否创建(电力、金钱满足条件)
	*/
	bool building = false;
	/**
	*表示该单位可以是否创建(位置满足条件)
	*/
	bool can_build = true;
	/**
	*表示该提示矩形是否被创建
	*/
	bool is_rec_created = false;
	/**
	*菜单的容器,用于添加各种按钮
	*/
	Layout * layout;
	/**
	*给提示矩形添加监听事件,如果被点击则发送创建信息
	*/
	void addListenerToRect(int type);
	/**
	*检查金钱、电力是否足够建造
	*/
	bool checkBuilding(int money, int power);
	/**
	*如果金钱够建造，则执行该函数：创建rec，判断是否可以创建，点击则创建type的建筑
	*/
	void onBuilding(int type,Size size);
	/**
	*判断提示矩形是否在建造范围内
	*/
	bool checkInRange(Point p);

	UnitManager * unit_manager;
	virtual bool init() = 0;
};

class BaseLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *militaryCamp;
	cocos2d::ui::Button *powerPlant;
	cocos2d::ui::Button *mine;
	cocos2d::ui::Button *tankFactary;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static BaseLayer * create();

	/**
	*设置创建菜单各按钮的可用性
	*/
	void setEnable(bool able);
};

class MilitaryCampLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *dog;
	cocos2d::ui::Button *soldier;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static MilitaryCampLayer * create();

};

class TankFactaryLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *airplane;
	cocos2d::ui::Button *tank;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static TankFactaryLayer * create();
};
#endif
