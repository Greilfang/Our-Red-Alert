#include "Building.h"
#include "Const.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Size Base::size = Size(141, 89);
Size MilitaryCamp::size = Size(70, 127);
Size TankFactary::size = Size(140, 110);
Size PowerPlant::size = Size(135, 120);
Size Mine::size = Size(134, 96);

void Base::setMenuEnable(bool able)
{
	baselayer->setEnable(able);
}

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
	current_life = 800;
	max_life = 800;
	speed = 0.0f;
	attack_range = GridSize(5, 5);
	ATK = 0;
	z_index = 5;
	mobile = false;
	//militaryCamp
	period_map[11] = MILITARY_PERIOD;
	//mine
	period_map[12] = MINE_PERIOD;
	//PowerPlant
	period_map[13] = POWERPLANT_PERIOD;
	//TankFactary
	period_map[14] = TANKFACTARY_PERIOD;
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
			baselayer->unit_manager = unit_manager;
			baselayer->center = this;
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
	current_life = max_life = 200;
	//Soldier
	period_map[3] = SOLDIER_PERIOD;
	//Dog
	period_map[4] = DOG_PERIOD;
}

void Base::update(float f)
{
	timer++;
	if (timer == 180)
		setListenerEnable(true);
	//状态为1表示正在生产，状态为2表示生产结束，状态为0表示没有生产的单位
	if (state == 1)
	{
		if (++prod_process >= prod_period)
		{
			state = 2;
			setMenuEnable(false);
			prod_process = 0;
			unit_manager->getTiledMap()->removeChild(building);
			unit_manager->setUnitCreateCenter(this->getPosition());
			unit_manager->genCreateMessage(cur_prod, camp, createPosition.x, createPosition.y);
			prod_bar->setVisible(false);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio\\unitready.wav");
		}
		else
		{
			float rate = static_cast<float>(float(prod_process) / float(prod_period));
			prod_bar->updateBarDisplay(rate);
		}
	}

	if (state == 2)
		if (prod_list.size())
		{
			state = 1;
			setMenuEnable(false);
			prod_process = 0;
			cur_prod = prod_list.back();
			prod_list.pop_back();
			prod_period = period_map.at(cur_prod);

			std::string file, File;
			Animation * built;
			switch (cur_prod)
			{
			case 11: 
				file = "MilitaryCamp_"; 
				built = AnimationCache::getInstance()->getAnimation("MilitaryCampCreate"); 
				break;
			case 12: 
				file = "Mine_";		
				built = AnimationCache::getInstance()->getAnimation("MineCreate"); 
				break;
			case 13: 
				file = "PowerPlant_"; 
				built = AnimationCache::getInstance()->getAnimation("PowerPlantCreate"); 
				break;
			case 14: 
				file = "TankFactory_"; 
				built = AnimationCache::getInstance()->getAnimation("TankFactoryCreate"); 
				break;
			}

			auto onbuild = Animate::create(built);
			File = "Picture/units/" + file + std::to_string(camp) + ".png";
			building = Sprite::create(File);
			unit_manager->getTiledMap()->addChild(building, 1000);
			building->setPosition(createPosition);
			building->runAction(onbuild);

		}
		else
		{
			state = 0;
			setMenuEnable(true);
		}

}

void Building::update(float f)
{
	//状态为1表示正在生产，状态为2表示生产结束，状态为0表示没有生产的单位
	if (state == 1)
	{
		if (++prod_process >= prod_period)
		{
			state = 2;
			prod_process = 0;
			Point point = findFreePosition();
			unit_manager->setUnitCreateCenter(this->getPosition());
			unit_manager->genCreateMessage(cur_prod, camp, point.x, point.y);
			prod_bar->setVisible(false);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio\\unitready.wav");
		}
		else
		{
			float rate = static_cast<float>(float(prod_process) / float(prod_period));
			prod_bar->updateBarDisplay(rate);
		}
	}

	if (state == 2)
		if (prod_list.size())
		{
			state = 1;
			prod_process = 0;
			cur_prod = prod_list.back();
			prod_list.pop_back();
			prod_period = period_map.at(cur_prod);
		}
		else
			state = 0;

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
			militaryCampLayer->center = this;
			militaryCampLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			militaryCampLayer->unit_manager = unit_manager;
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
	//Fighter
	period_map[1] = FIGHTER_PERIOD;

	period_map[2] = TANK_PERIOD;
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
			tankFactaryLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			tankFactaryLayer->center = this;
			tankFactaryLayer->unit_manager = unit_manager;
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
	if(camp == unit_manager->player_id)
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
	if (camp == unit_manager->player_id)
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
	rec = unit_manager->getGridRect(p, this->getContentSize());
	unit_manager->grid_map->occupyPosition(id, rec);
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

void Building::initBar()
{
	//hp_bar
	if (current_life) {
		hp_bar = Bar::create();
		float rate = static_cast<float>(current_life) / static_cast<float>(max_life);
		addChild(hp_bar, 10);
		hp_bar->updateBarDisplay(rate);
		hp_bar->stopKeepingVisible();
	}
	//prod_bar
	prod_bar = Bar::create();
	prod_bar->setColor(Color4F(0, 0, 0.8, 0.8));
	prod_bar->setHeight(12);

	addChild(prod_bar, 20);
	hp_bar->stopKeepingVisible();
}

void Building::startProduce(int _unit_type)
{
	prod_list.push_back(_unit_type);
	if (state != 1)
		state = 2;
}

void Building::startProduce(int _unit_type, Point _point)
{
	startProduce(_unit_type);
	createPosition = _point;
}

Point Building::findFreePosition()
{
	GridPoint a = unit_manager->grid_map->findFreePositionNear(_cur_pos);
	Point p = unit_manager->getPoint(a);
	return p;
}

