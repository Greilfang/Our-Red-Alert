#include "CreateUnitLayer.h"
#include "Building.h"
#include "CombatScene.h"
#include "SimpleAudioEngine.h"
#include "Const.h"
USING_NS_CC;
using namespace ui;

BaseLayer* BaseLayer::create()
{
	BaseLayer *ret = new BaseLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

void BaseLayer::setEnable(bool able)
{
	militaryCamp->setEnabled(able);
	powerPlant->setEnabled(able);
	mine->setEnabled(able);
	tankFactary->setEnabled(able);
}

bool BaseLayer::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bc = Sprite::create("bc.png");
	//初始化layout
	layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);
	//添加创建建筑的按钮
	// 创建一个创建militaryCamp的Button对象，设置在Layout的左上角
	militaryCamp = Button::create("/Picture/menu/MilitaryCampMenu.png",
		"/Picture/menu/MilitaryCampMenu.png");
	militaryCamp->setScale(0.8);
	layout->addChild(militaryCamp);
	RelativeLayoutParameter* rp_LeftCenter = RelativeLayoutParameter::create();
	rp_LeftCenter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
	militaryCamp->setLayoutParameter(rp_LeftCenter);
	//给militaryCamp添加监听器
	militaryCamp->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (checkBuilding(MILITARY_CAMP_MONEY, BUILDING_CONSUME_POWER))
			{
				//禁用层监听器
				unit_manager->getCombatScene()->destListener->setEnabled(false);
				//隐藏菜单
				this->setVisible(false);
				onBuilding(11, MilitaryCamp::size);
			}
		}
	});
	//添加一个创建mine的Button对象，设置在layout的右上角
	mine = Button::create("/Picture/menu/MineMenu.png",
		"/Picture/menu/MineMenu.png");
	mine->setScale(0.8);
	layout->addChild(mine);
	RelativeLayoutParameter* rp_top_right = RelativeLayoutParameter::create();
	rp_top_right->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
	mine->setLayoutParameter(rp_top_right);
	//给mine添加监听器
	mine->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (checkBuilding(MINE_MONEY, BUILDING_CONSUME_POWER))
			{
				//禁用层监听器
				unit_manager->getCombatScene()->destListener->setEnabled(false);
				//隐藏菜单
				this->setVisible(false);
				onBuilding(12, Mine::size);
			}
		}
	});

	//添加一个创建powerPlant的Button对象，设置在layout的左中部
	powerPlant = Button::create("/Picture/menu/PowerPlantMenu.png",
		"/Picture/menu/PowerPlantMenu.png");
	powerPlant->setScale(0.8);
	layout->addChild(powerPlant);
	RelativeLayoutParameter* rp_left_center = RelativeLayoutParameter::create();
	rp_left_center->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
	powerPlant->setLayoutParameter(rp_left_center);
	//给powerPlant添加监听器
	powerPlant->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (checkBuilding(POWER_PLANT_MONEY, 0))
			{
				//禁用层监听器
				unit_manager->getCombatScene()->destListener->setEnabled(false);
				//隐藏菜单
				this->setVisible(false);
				onBuilding(13, PowerPlant::size);
			}
		}
	});

	//添加一个创建TankFactary的Button对象，设置在layout的右部
	tankFactary = Button::create("/Picture/menu/TankFactaryMenu.png",
		"/Picture/menu/TankFactaryMenu.png");
	tankFactary->setScale(0.8);
	layout->addChild(tankFactary);
	RelativeLayoutParameter* rp_right_center = RelativeLayoutParameter::create();
	rp_right_center->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
	tankFactary->setLayoutParameter(rp_right_center);
	//给tankFacktary添加监听器
	tankFactary->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (checkBuilding(TANK_FACTARY_MONEY, BUILDING_CONSUME_POWER))
			{
				//禁用层监听器
				unit_manager->getCombatScene()->destListener->setEnabled(false);
				//隐藏菜单
				this->setVisible(false);
				onBuilding(14, TankFactary::size);
			}
		}
	});

	// 创建一个退出该layer的Button对象
	exit = Button::create("backNormal.png",
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
			this->setVisible(false);
		}
	});
	return true;
}

MilitaryCampLayer* MilitaryCampLayer::create()
{
	MilitaryCampLayer *ret = new MilitaryCampLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool MilitaryCampLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bc = Sprite::create("bc.png");
	//初始化layout
	layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);

	// 创建一个创建soldier的Button对象，设置在Layout的左上部
	soldier = Button::create("/Picture/menu/SoldierMenu.png",
		"/Picture/menu/SoldierMenu.png");
	layout->addChild(soldier);
	RelativeLayoutParameter* rp_TopLeft = RelativeLayoutParameter::create();
	rp_TopLeft->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
	soldier->setLayoutParameter(rp_TopLeft);
	soldier->setScale(0.8);
	//给soldier添加监听器
	soldier->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (unit_manager->money->checkMoney(2000))
			{
				center->startProduce(3);
				unit_manager->money->spendMoney(2000);
			}
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");
		}
	});

	// 创建一个创建dog的Button对象，设置在Layout的右上部
	dog = Button::create("/Picture/menu/DogMenu.png",
		"/Picture/menu/DogMenu.png");
	layout->addChild(dog);
	RelativeLayoutParameter* rp_TopRight = RelativeLayoutParameter::create();
	rp_TopRight->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
	dog->setLayoutParameter(rp_TopRight);
	dog->setScale(0.8);
	//给dog添加监听器
	dog->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (unit_manager->money->checkMoney(2000))
			{
				center->startProduce(4);
				unit_manager->money->spendMoney(2000);
			}
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");
		}
	});

	// 创建一个退出该layer的Button对象
	exit = Button::create("backNormal.png",
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
			this->setVisible(false);
		}
	});
	return true;
}


bool TankFactaryLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bc = Sprite::create("bc.png");
	//初始化layout
	layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);

	// 创建一个创建天启坦克的Button对象，设置在Layout的左上角
	tank = Button::create("/Picture/menu/TankMenu.png",
		"/Picture/menu/TankMenu.png");
	layout->addChild(tank);
	RelativeLayoutParameter* rp_TopLeft = RelativeLayoutParameter::create();
	rp_TopLeft->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
	tank->setLayoutParameter(rp_TopLeft);
	tank->setScale(0.8);
	//给天启坦克添加监听器
	tank->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (unit_manager->money->checkMoney(2000))
			{
				center->startProduce(2);
				unit_manager->money->spendMoney(2000);
			}
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");
		}
	});

	// 创建一个创建ariplain的Button对象，设置在Layout的右上角
	airplane = Button::create("/Picture/menu/AirplaneMenu.png",
		"/Picture/menu/AirplaneMenu.png");
	layout->addChild(airplane);
	RelativeLayoutParameter* rp_TopRight = RelativeLayoutParameter::create();
	rp_TopRight->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
	airplane->setLayoutParameter(rp_TopRight);
	airplane->setScale(0.8);
	//给airplane添加监听器
	airplane->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (unit_manager->money->checkMoney(2000))
			{
				center->startProduce(1);
				unit_manager->money->spendMoney(2000);
			}
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");
		}
	});

	// 创建一个退出该layer的Button对象
	exit = Button::create("backNormal.png",
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
			this->setVisible(false);
		}
	});
	return true;
}

TankFactaryLayer * TankFactaryLayer::create()
{
	TankFactaryLayer *ret = new TankFactaryLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

Point CreateUnitLayer::findFirstPosition()
{
	return Point();
}

void CreateUnitLayer::addListenerToRect(int type)
{
	auto spriteListener = EventListenerTouchOneByOne::create();
	spriteListener->setSwallowTouches(false);
	spriteListener->onTouchBegan = [=](Touch* touch, Event* event) {
		auto target = static_cast<Unit*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode)) {
			if (can_build)
			{
				rec->setVisible(false);
				center->startProduce(type, rec_abs_center);
				//unit_manager->genCreateMessage(type,1,rec_abs_center.x,rec_abs_center.y);
				building = false;
				//显示菜单
				this->setVisible(true);
				//重新启用层监听器
				unit_manager->getCombatScene()->destListener->setEnabled(true);
				unit_manager->getCombatScene()->removeChild(rec);
				//关闭基地建造范围
				unit_manager->constructRange->setVisible(false);
				return true;
			}			
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(spriteListener, rec);
}

bool CreateUnitLayer::checkBuilding(int money, int power)
{
	if (unit_manager->money->checkMoney(money))
	{
		if (unit_manager->power->checkPower(power))
		{
			building = true;		
			unit_manager->money->spendMoney(money);
			unit_manager->power->spendPower(power);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");
		return false;
	}
}

void CreateUnitLayer::onBuilding(int type,Size size)
{
	unit_manager->constructRange->setVisible(true);
	rec = Sprite::create("MagentaSquare.png");
	rec->setAnchorPoint(Vec2(0.5, 0.5));
	rec->setContentSize(size);
	rec_size = size;
	unit_manager->getCombatScene()->addChild(rec);
	rec->setVisible(false);
	addListenerToRect(type);
	auto mouse_event = EventListenerMouse::create();
	mouse_event->onMouseMove = [&](Event *event) {
		if (building)
		{
			EventMouse* pem = static_cast<EventMouse*>(event);
			Point delta = unit_manager->getCombatScene()->cdelta;
			rec_center = Vec2(pem->getCursorX(), pem->getCursorY());
			rec->setPosition(rec_center);
			rec_abs_center = rec_center - delta;
			if (unit_manager->grid_map->checkPosition(unit_manager->getGridRect(rec_abs_center, rec_size)) && checkInRange(rec_abs_center))
			{
				can_build = true;
				rec->setOpacity(255);
			}
			else
			{
				can_build = false;
				rec->setOpacity(120);
			}
			rec->setVisible(true);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mouse_event, 1);
}

bool CreateUnitLayer::checkInRange(Point p)
{
	Point baseP = unit_manager->getBasePosition();
	float range = unit_manager->base->construct_range;
	float distance = (p.x - baseP.x)*(p.x - baseP.x) + (p.y - baseP.y)*(p.y - baseP.y);
	return distance <= range * range;
}

