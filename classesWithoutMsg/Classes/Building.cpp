#include "Building.h"
#include "CreateUnitLayer.h"

USING_NS_CC;


Building * Building::create(const std::string & filename)
{
	Building *ret = new Building();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		if (ret->current_life) {
			ret->set_movable(false);
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

Layer * Building::getLayer()
{
	return addLayer;
}


TMXTiledMap* Building::getMap()
{
	return addMap;
}

void Building::setType(int ty = 1)
{
	type = ty;
	this->addListener();
}

int Building::getType()
{
	return type;
}

void Building::addListener()
{
	
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Building::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Building::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Building::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


bool Building::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
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
		/*
		getLayer()->cancellClickedUnit();
		getLayer()->getSelected_box().push_back(target);	
		getLayer()->getClickedUnit();
		*/
		target->setOpacity(180);
		// 表明用户触摸事件已经被处理，后续的onTouchMoved、onTouchEnded和onTouchCancelled会接着响应，其他事件监听器则不会再去进行监听本次触摸事件。
		return true;
	}
	return false;
}

void Building::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{

}

void Building::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 设置透明度
	target->setOpacity(255);
	std::string name;
	if (target == this)
	{
		if (type == 1)
		{
			auto createUnitLayer = MilitaryCampLayer::create();
			createUnitLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			createUnitLayer->set(addMap, addLayer, addTouchListener);
			getLayer()->addChild(createUnitLayer, 10);
		}
		else if (type == 0)
		{
			auto createUnitLayer = BaseLayer::create();
			createUnitLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			createUnitLayer->set(addMap, addLayer, addTouchListener);
			getLayer()->addChild(createUnitLayer, 10);
		}
	}
}
