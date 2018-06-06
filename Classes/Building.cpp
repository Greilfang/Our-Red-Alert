#include "Building.h"


USING_NS_CC;


Base* Base::create(const std::string & filename)
{
	Base *ret = new Base();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		if (ret->current_life) {
			ret->setMobile(false);
			ret->hp_bar = Bar::create();
			float rate = static_cast<float>(ret->current_life) / static_cast<float>(ret->max_life);
			ret->addChild(ret->hp_bar, 10);
			ret->hp_bar->updateBarDisplay(rate);
			ret->hp_bar->stopKeepingVisible();
		}
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Base::set(TMXTiledMap * AM, Layer * AL, EventListenerTouchOneByOne * ALis)
{
	tiled_map = AM;
	combat_scene = AL;
	spriteTouchListener = ALis;
}

void Base::setProperties()
{
	type = 0;
	z_index = 5;
	mobile = false;
}
void Base::setListener()
{
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
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
		log("onTouchBegan... x = %f, y = %f", locationInNode.x, locationInNode.y);
		// 设置透明度		
		target->setOpacity(180);
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
			baselayer->unit_manager = unit_manager;
			combat_scene->addChild(baselayer, 10);
			layer_is_created = true;
		}
		else
		{
			baselayer->setVisible(true);
		}
	}
}

MilitaryCamp* MilitaryCamp::create(const std::string & filename)
{
	MilitaryCamp *ret = new MilitaryCamp();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		if (ret->current_life) {
			ret->setMobile(false);
			ret->hp_bar = Bar::create();
			float rate = static_cast<float>(ret->current_life) / static_cast<float>(ret->max_life);
			ret->addChild(ret->hp_bar, 10);
			ret->hp_bar->updateBarDisplay(rate);
			ret->hp_bar->stopKeepingVisible();
		}
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void MilitaryCamp::set(TMXTiledMap * AM, Layer * AL, EventListenerTouchOneByOne * ALis)
{
	tiled_map = AM;
	combat_scene = AL;
	spriteTouchListener = ALis;
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
		log("onTouchBegan... x = %f, y = %f", locationInNode.x, locationInNode.y);
		// 设置透明度
		target->setOpacity(180);
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
			militaryCampLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			militaryCampLayer->unit_manager = unit_manager;
			combat_scene->addChild(militaryCampLayer, 10);
			layer_is_created = true;
		}
		else
		{
			militaryCampLayer->setVisible(true);
		}
	}
}
