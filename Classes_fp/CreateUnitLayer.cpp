#include "CreateUnitLayer.h"
#include "Building.h"
USING_NS_CC;
using namespace ui;

Layer * CreateUnitLayer::getLayer()
{
	return combatScene;
}

TMXTiledMap * CreateUnitLayer::getMap()
{
	return tiledMap;
}

bool BaseLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
}

void BaseLayer::set(TMXTiledMap * AM, Layer * AL, EventListenerTouchOneByOne * ALis)
{
	tiledMap = AM;
	combatScene = AL;
	spriteTouchListener = ALis;
	realInit();
}
//type为0的基地弹出的菜单
void BaseLayer::realInit()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bc = Sprite::create("bc.png");
	//初始化layout
	Layout* layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width*0.9, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);

	// 创建一个创建militaryCamp的Button对象，设置在Layout的左上角
	Button* militaryCamp = Button::create("/Picture/units/base_3.png",
		"/Picture/units/base_3.png");
	layout->addChild(militaryCamp);
	RelativeLayoutParameter* rp_LeftCenter = RelativeLayoutParameter::create();
	rp_LeftCenter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
	militaryCamp->setLayoutParameter(rp_LeftCenter);
	//给militaryCamp添加监听器
	militaryCamp->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			unit_manager->genCreateMessage(11);
			/*
			log("new militartCamp");
			auto militaryCamp1 = Building::create("/Picture/units/base_3.png");
			militaryCamp1->set(addMap, addLayer, addTouchListener);
			militaryCamp1->setType(1);
			militaryCamp1->setPosition(150, 250);
			Director::getInstance()->getEventDispatcher()
			->addEventListenerWithSceneGraphPriority(addTouchListener->clone(), militaryCamp1);
			addMap->addChild(militaryCamp1);
			*/
		}
	});

	// 创建一个退出该layer的Button对象
	Button* exit = Button::create("backNormal.png",
		"backNormal.png");
	layout->addChild(exit);
	RelativeLayoutParameter* rp_exit = RelativeLayoutParameter::create();
	rp_exit->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
	exit->setLayoutParameter(rp_exit);
	//给exit添加监听器
	exit->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			log("exit CreateUnitLayer layer");
			combatScene->removeChild(this);
		}
	});
}

bool MilitaryCampLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
}

void MilitaryCampLayer::set(TMXTiledMap * AM,Layer * AL, EventListenerTouchOneByOne * ALis)
{
	tiledMap = AM;
	combatScene = AL;
	spriteTouchListener = ALis;
	realInit();
}
//type为1的兵营弹出的菜单
void MilitaryCampLayer::realInit()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bc = Sprite::create("bc.png");
	//初始化layout
	Layout* layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width*0.9, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);

	// 创建一个创建ariplain的Button对象，设置在Layout的左上角
	Button* airplane = Button::create("/Picture/menu/airplane-menu-up.png",
		"/Picture/menu/airplane-menu-down.png");
	layout->addChild(airplane);
	RelativeLayoutParameter* rp_TopLeft = RelativeLayoutParameter::create();
	rp_TopLeft->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
	airplane->setLayoutParameter(rp_TopLeft);
	airplane->setScale(0.65);
	//给airplane添加监听器
	airplane->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			unit_manager->genCreateMessage(1);
			/*
			log("new airplane");
			auto airplane1 = Unit::create("Picture/units/airplane_0.png");
			airplane1->setPosition(100, 100);
			Director::getInstance()->getEventDispatcher()
				->addEventListenerWithSceneGraphPriority(addTouchListener->clone(), airplane1);
			addMap->addChild(airplane1);	
			*/
		}
	});

	// 创建一个创建tank的Button对象，设置在Layout的右上角
	Button* tank = Button::create("/Picture/menu/tank-menu-up.png",
		"/Picture/menu/tank-menu-down.png");
	layout->addChild(tank);
	RelativeLayoutParameter* rp_TopRight = RelativeLayoutParameter::create();
	rp_TopRight->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
	tank->setLayoutParameter(rp_TopRight);
	tank->setScale(0.65);
	//给tank添加监听器
	tank->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			unit_manager->genCreateMessage(2);
			/*
			log("new tank");
			auto tank1 = Unit::create("Picture/units/tank.png");
			tank1->setPosition(100, 300);
			addMap->addChild(tank1);
			Director::getInstance()->getEventDispatcher()
				->addEventListenerWithSceneGraphPriority(addTouchListener->clone(), tank1);
			*/
		}
	});

	// 创建一个创建soldier的Button对象，设置在Layout的中左部
	Button* soldier = Button::create("/Picture/menu/soldier-menu-up.png",
		"/Picture/menu/soldier-menu-down.png");
	layout->addChild(soldier);
	RelativeLayoutParameter* rp_LeftCenter = RelativeLayoutParameter::create();
	rp_LeftCenter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
	soldier->setLayoutParameter(rp_LeftCenter);
	soldier->setScale(0.65);
	//给soldier添加监听器
	soldier->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			unit_manager->genCreateMessage(3);
			/*
			log("new soldier");
			auto soldier1 = Unit::create("Picture/units/soldier.png");
			soldier1->setPosition(100, 200);
			addMap->addChild(soldier1);
			Director::getInstance()->getEventDispatcher()
				->addEventListenerWithSceneGraphPriority(addTouchListener->clone(), soldier1);
			*/
		}
	});

	// 创建一个退出该layer的Button对象
	Button* exit = Button::create("backNormal.png",
		"backNormal.png");
	layout->addChild(exit);
	RelativeLayoutParameter* rp_exit = RelativeLayoutParameter::create();
	rp_exit->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
	exit->setLayoutParameter(rp_exit);
	//给exit添加监听器
	exit->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			log("exit CreateUnitLayer layer");
			combatScene->removeChild(this);
		}
	});
}

