#include "Building.h"
#include "Const.h"
#define DEBUG
#include "iostream"
USING_NS_CC;

Size Base::size = Size(20, 20);
Size MilitaryCamp::size = Size(90, 90);
Size TankFactary::size = Size(64, 64);
Size PowerPlant::size = Size(85, 90);
Size Mine::size = Size(75, 91);

Base* Base::create(const std::string & filename)
{
	Base *ret = new Base();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();		
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Base::setProperties()
{
	type = 0;
	max_life = 800;
	speed = 0.0f;
	attack_range = 0;
	ATK = 0;
	z_index = 5;
	mobile = false;
}
void Base::setListener()
{
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	spriteTouchListener = listener;
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Base::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Base::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Base::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


bool Base::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	// 获得当前事件触摸的目标对象
	auto target = static_cast<Unit*>(event->getCurrentTarget());
	// 获得当前的触摸点
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	// 获得触摸对象的ContentSize
	Size s = target->getContentSize();
	// 获得位置矩形
	Rect rect = Rect(0, 0, s.width, s.height);
	// 如果位置矩形包含触摸点
	if (rect.containsPoint(locationInNode))
	{
		unit_manager->selectPointUnits(target);
		// 表明用户触摸事件已经被处理，后续的onTouchMoved、onTouchEnded和onTouchCancelled会接着响应，其他事件监听器则不会再去进行监听本次触摸事件。
		return true;
	}
	return false;
}

void Base::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{

}

void Base::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 设置透明度
	target->setOpacity(255);
	std::string name;
	if (target == this)
	{	
		if (layer_is_created == false)
		{
			baselayer = BaseLayer::create();
			baselayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			baselayer->setCenterPosition(this->getPosition());
			baselayer->unit_manager = unit_manager;
			baselayer->setSwallowsTouches(true);
			combat_scene->addChild(baselayer, 15);
			unit_manager->createLayer.pushBack(baselayer);
			layer_is_created = true;
		}
		setLayerVisible(baselayer);

	}
}

MilitaryCamp* MilitaryCamp::create(const std::string & filename)
{
	MilitaryCamp *ret = new MilitaryCamp();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();	
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void MilitaryCamp::setProperties()
{
	type = 11;
	z_index = 5;
	mobile = false;
}
void MilitaryCamp::setListener()
{
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(MilitaryCamp::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MilitaryCamp::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MilitaryCamp::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


bool MilitaryCamp::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	// 获得当前事件触摸的目标对象
	auto target = static_cast<Unit*>(event->getCurrentTarget());
	// 获得当前的触摸点
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	// 获得触摸对象的ContentSize
	Size s = target->getContentSize();
	// 获得位置矩形
	Rect rect = Rect(0, 0, s.width, s.height);
	// 如果位置矩形包含触摸点
	if (rect.containsPoint(locationInNode))
	{
		unit_manager->selectPointUnits(target);
		// 表明用户触摸事件已经被处理，后续的onTouchMoved、onTouchEnded和onTouchCancelled会接着响应，其他事件监听器则不会再去进行监听本次触摸事件。
		return true;
	}
	return false;
}

void MilitaryCamp::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{

}

void MilitaryCamp::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 设置透明度
	target->setOpacity(255);
	std::string name;
	if (target == this)
	{
		if (layer_is_created == false)
		{
			militaryCampLayer = MilitaryCampLayer::create();
			militaryCampLayer->setCenterPosition(this->getPosition());
			militaryCampLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			militaryCampLayer->unit_manager = unit_manager;
			militaryCampLayer->setSwallowsTouches(true);
			combat_scene->addChild(militaryCampLayer, 15);
			unit_manager->createLayer.pushBack(militaryCampLayer);
			layer_is_created = true;
		}
		setLayerVisible(militaryCampLayer);
	}
}


void TankFactary::setProperties()
{
	type = 14;
	z_index = 5;
	mobile = false;
}

TankFactary * TankFactary::create(const std::string & filename)
{
	TankFactary *ret = new TankFactary();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();		
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void TankFactary::setListener()
{
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(TankFactary::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TankFactary::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TankFactary::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool TankFactary::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	// 获得当前事件触摸的目标对象
	auto target = static_cast<Unit*>(event->getCurrentTarget());
	// 获得当前的触摸点
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	// 获得触摸对象的ContentSize
	Size s = target->getContentSize();
	// 获得位置矩形
	Rect rect = Rect(0, 0, s.width, s.height);
	// 如果位置矩形包含触摸点
	if (rect.containsPoint(locationInNode))
	{
		unit_manager->selectPointUnits(target);
		// 表明用户触摸事件已经被处理，后续的onTouchMoved、onTouchEnded和onTouchCancelled会接着响应，其他事件监听器则不会再去进行监听本次触摸事件。
		return true;
	}
	return false;
}

void TankFactary::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
}

void TankFactary::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 设置透明度
	target->setOpacity(255);
	std::string name;
	if (target == this)
	{
		if (layer_is_created == false)
		{
			tankFactaryLayer = TankFactaryLayer::create();
			tankFactaryLayer->setCenterPosition(this->getPosition());
			tankFactaryLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			tankFactaryLayer->unit_manager = unit_manager;
			tankFactaryLayer->setSwallowsTouches(true);
			combat_scene->addChild(tankFactaryLayer, 15);
			unit_manager->createLayer.pushBack(tankFactaryLayer);
			layer_is_created = true;
		}
		setLayerVisible(tankFactaryLayer);

	}
}


void Mine::setProperties()
{
	type = 12;
	z_index = 5;
	mobile = false;
	setIncreasingAmount(MONEY_INCREASE);
}

void Mine::setIncreasingAmount(int amount)
{
	unit_manager->setIncreasingAmount(amount);
}

Mine * Mine::create(const std::string & filename)
{
	Mine *ret = new Mine();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void PowerPlant::setProperties()
{
	type = 13;
	z_index = 5;
	mobile = false;
	addMax_Power(POWER_INCREASE);
}

void PowerPlant::addMax_Power(int delta)
{
	unit_manager->setMax_power(delta);
}

PowerPlant * PowerPlant::create(const std::string & filename)
{
	PowerPlant *ret = new PowerPlant();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Building::addToGmap(Point p)
{
#ifdef DEBUG
	std::cout << "Unit type: " << this->type<<std::endl;
#endif // DEBUG
	rec = unit_manager->getGridRect(p, this->getContentSize());
	unit_manager->grid_map->occupyPosition(rec);
}

void Building::setListener()
{
	Unit::setListener();
}

void Building::setLayerVisible(Layer * myLayer)
{
	for (auto cl : unit_manager->createLayer)
	{
		cl->setVisible(false);
	}
	myLayer->setVisible(true);
}


