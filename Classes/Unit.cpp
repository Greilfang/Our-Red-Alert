#include"cocos2d.h"
#include"Unit.h"
#include "AdvancedUnit.h"
#include "Building.h"
#include "CreateUnitLayer.h"
#include<string>
#include<random>
//#define DEBUG

const int MAX_PLAYER_NUM = 4;
USING_NS_CC;
/*血条更新*/
void Bar::updateBarDisplay(float rate) {
#ifdef DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif // DEBUG
	
	setVisible(true);
	clear();
	auto tg = static_cast<Unit*>(this->getParent());
#ifdef DEBUG
	log("bar father:address %d", this->getParent());
#endif // DEBUG

	auto s = tg->getContentSize();
	
	drawRect(Point((s.width - length) / 2, s.height + 5), Point((s.width + length) / 2, s.height + 5 + width), color);
	Point endpoint{ s.width / 2 - length / 2 + length * rate,s.height + 5 + width };
	drawSolidRect(Point((s.width - length) / 2, s.height + 5), endpoint, color);
}

void Bar::stopKeepingVisible() {
	setVisible(false);
	is_visible = false;
}
void Bar::keepVisible() {
	setVisible(true);
	is_visible = true;
}

Unit * Unit::create(const std::string & filename)
{
	Unit *ret = new Unit();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		if (ret->current_life) {
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

void Unit::setProperties()
{
	type = -1;
	team = 0;
	selected = 0;

}

void Unit::displayHP() 
{
	if (hp_bar) {
		hp_bar->keepVisible();
	}
}
void Unit::hideHP() 
{
	if (hp_bar) {
		hp_bar->stopKeepingVisible();
	}
}

void Unit::set(TMXTiledMap * _tiledMap, Layer * _combatScene, EventListenerTouchOneByOne * _listener)
{
	tiled_map = _tiledMap;
	combat_scene = _combatScene;
	spriteTouchListener = _listener;
}

void Unit::setListener()
{
}

void Unit::setUnitManager(UnitManager * _unit_manager)
{
	unit_manager = _unit_manager;
}

void Unit::setMobile(bool can)
{
	mobile = can;
}

bool Unit::isMobile()
{
	return mobile;
}

void Unit::setTeam(int _team)
{
	team = _team;
}

int Unit::getTeam() const
{
	return team;
}

int Unit::getType() const
{
	return type;
}

bool Unit::is_in(Point p1, Point p2) {
	Point unitPoint = this->getPosition();
	if (abs(unitPoint.x - p1.x) + abs(unitPoint.x - p2.x) != abs(p1.x - p2.x)) {
		return false;
	}
	else if(abs(unitPoint.y - p1.y) + abs(unitPoint.y - p2.y) != abs(p1.y - p2.y)) {
		return false;
	}
	return true;
}

bool UnitManager::init()
{
	return true;
}

void UnitManager::setMessageSet(GameMessageSet * _msgs)
{
	msgs = _msgs;
}

void UnitManager::setSpriteTouchListener(EventListenerTouchOneByOne * _spriteTouchListener)
{
	spriteTouchListener = _spriteTouchListener;
}

void UnitManager::setTiledMap(cocos2d::TMXTiledMap * _tiledMap)
{
	tiled_map = _tiledMap;;
}

void UnitManager::setPlayerID(int _player_id)
{
	player_id = _player_id;
}

void UnitManager::setCombatScene(CombatScene * _combat_scene)
{
	combat_scene = _combat_scene;
}

void UnitManager::setBuilding(Building * _building)
{
	building = _building;
}
/*
GridPoint UnitManager::getUnitPosition(int _unit_id)
{
	Unit* unit = id_map.at(_unit_id);
	if (unit)
		return(unit->getGridPosition());
	else
		return{ -1, -1 };
}
*/
int UnitManager::getUnitTeam(int unit_id)
{
	Unit* unit = id_map.at(unit_id);
	if (unit)
		return(unit->team);
	else
		return 0;
}
/*
GridPoint UnitManager::getBasePosition()
{
	return base_pos;
}
*/
CombatScene * UnitManager::getCombatScene()
{
	return combat_scene;
}

EventListenerTouchOneByOne * UnitManager::getSpriteTouchListener()
{
	return spriteTouchListener;
}

void UnitManager::updateUnitsState()
{
	for (int i = 0; i < msgs->game_message_size(); i++)
	{
		const GameMessage&  msg = msgs->game_message(i);
		//log("UnitManager: Read Message %d Success", i);
		if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_CRT)
		{
			int id = msg.unit_0();
			int team = msg.camp();
			int unit_type = msg.unit_type();
			Unit* new_unit = createNewUnit(id, team, unit_type);
			id_map.insert(id, new_unit);
		}
	}
	msgs->clear_game_message();
}

Unit* UnitManager::createNewUnit(int id, int team, int unit_type)
{
	Unit* nu;
	Base* tmp_base;

	switch (unit_type)
	{
	case 1:
		nu = Fighter::create("Picture/units/fighter.png");
		break;
	case 2:
		nu = Tank::create("Picture/units/tank.png");
		break;
	case 3:
		nu = Soldier::create("Picture/units/soldier.png");
		break;
	case 0:
		tmp_base = Base::create("Picture/units/base_0.png");
		base_map[id] = team;
		nu = tmp_base;
		break;
	case 11:
		nu = MilitaryCamp::create("Picture/units/base_2.png");
		break;
	default:
		break;
	}

	nu->unit_manager = this;
	nu->id = id;
	nu->team = team;
	nu->setProperties();
	nu->setListener();
	nu->setAnchorPoint(Vec2(0.5, 0.5));
	nu->set(tiled_map, (Layer *)combat_scene, spriteTouchListener);
	nu->setPosition(100, 100 + id);
	tiled_map->addChild(nu);
	//nu->initBars();
	//nu->initFlag();
	nu->schedule(schedule_selector(Unit::update));

	return(nu);
}

//生成新单位测试程序
void UnitManager::genCreateMessage(int _unit_type)
{
	auto new_msg = msgs->add_game_message();
	new_msg->set_cmd_code(GameMessage::CmdCode::GameMessage_CmdCode_CRT);
	new_msg->set_unit_type(_unit_type);
	GridPath *gridpath = new GridPath;
	auto newgridpoint = gridpath->add_grid_point();
	newgridpoint->set_x(100);
	newgridpoint->set_y(100);
	new_msg->set_allocated_grid_path(gridpath);
	next_id += MAX_PLAYER_NUM;
}

void UnitManager::deselectAllUnits()
{
	for (auto& id : selected_ids)
		id_map.at(id)->hideHP();
	selected_ids.clear();
}
/*
void UnitManager::selectUnits(Point select_point)
{
	
	if (selected_ids.size())
	{
		
		for (auto & id_unit : id_map)
			if (id_unit.second->team != player_id && id_unit.second->getBoundingBox().containsPoint(select_point))
			{

				for (auto & id : selected_ids)
				{
					//log("Unit ID: %d, tracing enemy id: %d", id, id_unit.second->id);
					Unit* unit = id_map.at(id);
					if (!unit || !unit->isMobile())
						continue;
					GridPoint target_pos = getUnitPosition(id_unit.first);
					unit->setTarget(id_unit.second->id);
					unit->setDestination(target_pos);
					log("Unit %d, start tracing FP", id);
					unit->tryToFindPath();
				}
				return;
			}
		for (auto & id_unit : id_map)
			if (id_unit.second->camp == player_id && id_unit.second->getBoundingBox().containsPoint(select_point))
			{
				deselectAllUnits();
				selected_ids.push_back(id_unit.first);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/selecttarget.wav");
				id_unit.second->displayHPBar();
				return;
			}
		for (auto & id : selected_ids)
		{
			Unit* unit = id_map.at(id);

			if (!unit || !unit->isMobile())
				continue;

			GridPoint grid_dest = grid_map->getGridPoint(select_point);
			log("Unit ID: %d, plan to move to:(%d, %d)", id, grid_dest.x, grid_dest.y);

			unit->setDestination(grid_dest);
			log("Unit %d, start moving FP", id);
			unit->abandonTracing();
			unit->tryToFindPath();

		}
		return;
	}
	else
		for (auto & id_unit : id_map)
			if (id_unit.second->camp == player_id && id_unit.second->getBoundingBox().containsPoint(select_point))
			{
				deselectAllUnits();
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/selecttarget.wav");
				selected_ids.push_back(id_unit.first);
				id_unit.second->displayHPBar();
				return;
			}
	
	return;
}
/*
/*
void UnitManager::selectUnits(Rect select_rect)
{
	deselectAllUnits();
	for (auto & id_unit : id_map)
		if (id_unit.second->team == player_id && select_rect.containsPoint(id_unit.second->getPosition()))
		{
			selected_ids.push_back(id_unit.first);
			id_unit.second->displayHP();
		}
	return;
}
*/