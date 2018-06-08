#include "Unit.h"
#include "AdvancedUnit.h"
#include "Building.h"
#include "BattleScene.h"
#include <string>
#include <random>
#include "SimpleAudioEngine.h"


USING_NS_CC;

void Bar::update(float dt)
{
	if (++timer == disp_time)
	{
		timer = 0;
		disp_time = 0;
		unschedule(schedule_selector(Bar::update));
		setVisible(false);
	}
}

void Bar::updateBarDisplay(float rate, int _disp_time)
{
	setVisible(true);
	clear();
	drawRect(Point(0, 0), Point(length, height), color);
	Point endpoint{ length * rate, height };
	drawSolidRect(Point(0, 0), endpoint, color);
	if (!kept_visible)
		if (_disp_time)
		{
			schedule(schedule_selector(Bar::update));
			disp_time = _disp_time;
			timer = 0;
		}
		else
			kept_visible = true;
}

void Bar::setLength(float _length)
{
	length = _length;
}

void Bar::setColor(const cocos2d::Color4F & _color)
{
	color = _color;
}

void Bar::keepVisible()
{
	setVisible(true);
	kept_visible = true;
}

void Bar::stopKeepingVisible()
{
	setVisible(false);
	kept_visible = false;
}

Unit* Unit::create(const std::string& filename)
{
	Unit *ret = new (std::nothrow) Unit();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}

void Unit::setProperties()
{
	type = 0;
	selected = 0;
	state = 0;
	target_id = -1;

	cd = cd_max;
	hp = hp_max;

}

void Unit::initBars()
{
	hpbar = Bar::create();
	float unit_width = size.width * tiled_map->getTileSize().width;
	float unit_height = size.height * tiled_map->getTileSize().height;
	hpbar->setLength(unit_width);
	hpbar->setVisible(false);
	addChild(hpbar, 20);
	hpbar->setPosition(Point(0, unit_height + 5));
}

void Unit::initFlag()
{
	flag = DrawNode::create();
	flag->drawSolidRect(Point(0, 0), Point(5, 5), getCampColor());
	addChild(flag);
}

cocos2d::Color4F Unit::getCampColor()
{
	std::vector<Color4F> camp_color_list = { { 0, 0, 0, 0 },{ 1, 0, 0, 1 },{ 0, 1, 0, 1 },{ 0, 0, 1, 1 } };
	return camp_color_list[camp % 4];
}

void Unit::displayHPBar(int _disp_time)
{
	if (hpbar)
	{
		hpbar->updateBarDisplay(float(hp) / float(hp_max), _disp_time);
		hpbar->setVisible(true);
	}
}

void Unit::hideHPBar()
{
	if (hpbar)
	{
		hpbar->stopKeepingVisible();
	}
}

GridPoint Unit::getGridPosition() const
{
	if (grid_map)
		return(grid_map->getGridPoint(getPosition()));
	else
		return(GridPoint(0, 0));
}

void Unit::setGridPath(const MsgGridPath& _msg_grid_path)
{
	int grid_point_size = _msg_grid_path.grid_point_size();
	grid_path = GridPath(grid_point_size);
	for (int i = 0; i < grid_point_size; i++)
		grid_path[i] = GridPoint{ _msg_grid_path.grid_point(i).x(), _msg_grid_path.grid_point(i).y() };
	final_dest = grid_path[0];
	cur_dest = grid_path.back();
	grid_path.pop_back();
}

void Unit::motivate()
{
	moving = true;
}

void Unit::setState(int _state)
{
	state = _state;
}

void Unit::setDestination(const GridPoint& grid_dest)
{
	final_dest = grid_dest;
}

void Unit::setTarget(int _target_id)
{
	target_lastpos = unit_manager->getUnitPosition(_target_id);
	target_id = _target_id;
	tracing = true;
}

void Unit::abandonTracing()
{
	tracing = false;
	target_id = 0;
}

int Unit::getState() const
{
	return(state);
}

int Unit::getType() const
{
	return type;
}

bool Unit::underAttack(int damage)
{
	hp -= damage;
	displayHPBar(200);
	if (hp < 0)
		return(true);
	else
		return(false);
}


bool Unit::isMobile()
{
	return mobile;
}

bool TrajectoryEffect::init()
{
	if(!ParticleFire::init())
		return false;
	setScale(0.1);
//	setPosition(Vec2(400,20));
//	log("fire start position:%f,%f", getPosition().x, getPosition().y);
	setPositionType(PositionType::RELATIVE);
	return true;
}

/**
 * \brief set the path and start move
 * \param from sender's position
 * \param to target's position
 */
void TrajectoryEffect::setPath(cocos2d::Vec2 from, cocos2d::Vec2 to)
{
	from_ = from;
	to_ = to;
	setPosition(from_);
//	log("fire start position:%f,%f", getPosition().x, getPosition().y);
//	log("start position:%f,%f", from.x, from.y);
	move_ =(to_-from_).getNormalized()*speed_;
	schedule(schedule_selector(TrajectoryEffect::updatefire));
}

void TrajectoryEffect::updatefire(float)
{
//	log("fire position:%f,%f", getPosition().x, getPosition().y);
	if (((abs(getPosition().x-to_.x)<speed_ )&& (abs(getPosition().y - to_.y)<speed_)))
		removeFromParent();
	else
		setPosition(getPosition() + move_);
}

bool ExplosionEffect::init()
{
	if (!ParticleFire::init())
		return false;
	setScale(0.0000008);
	setDuration(1);
	auto action = ScaleBy::create(0.5, 500000);
	runAction(action);
	scheduleOnce(schedule_selector(ExplosionEffect::remove), 1.2);	
	setPositionType(PositionType::RELATIVE);
	return true;
}

void ExplosionEffect::remove(float f)
{
	removeFromParent();
}

void Unit::addToMaps(const GridPoint & crt_gp, TMXTiledMap* _tiled_map, GridMap* _grid_map)
{
	tiled_map = _tiled_map;
	grid_map = _grid_map;

	cur_pos = crt_gp;
	setPosition(grid_map->getPointWithOffset(crt_gp));

	_tiled_map->addChild(this, z_index);

	_grid_map->occupyPosition(id, cur_pos);
	if (camp == unit_manager->player_id)
		grid_map->clearFog(GridRect(cur_pos, vision_range, true));
}

void Unit::removeFromMaps()
{
	// add explosion effect
	auto explosion_effect = ExplosionEffect::create();
	explosion_effect->setPosition(this->getPosition());
	getParent()->addChild(explosion_effect,20);
	grid_map->leavePosition(cur_pos);
	tiled_map->removeChild(this,1);
}

bool Unit::hasArrivedAtDest()
{
	return(grid_map->hasApproached(getPosition(), cur_dest) && getGridPosition() == cur_dest);
}

void Unit::move()
{
	auto esp = (grid_map->getPointWithOffset(cur_dest) - getPosition()).getNormalized();
	Point next_pos = getPosition() + esp * move_speed;
	GridPoint next_gp = grid_map->getGridPoint(next_pos);

	if (cur_pos == next_gp)
	{
		setPosition(next_pos);
	}
	else
		if (grid_map->occupyPosition(id, next_gp))
		{
			roc_cnt = 0;
			setPosition(next_pos);
			grid_map->leavePosition(cur_pos);
			cur_pos = next_gp;
			if (camp == unit_manager->player_id)
				grid_map->clearFog(GridRect(cur_pos, vision_range, true));
		}
		else
		{
			cur_dest = cur_pos;

			Point final_fp = grid_map->getPointWithOffset(final_dest);

			if (camp == unit_manager->player_id && (final_fp - getPosition()).length() > DISREFINDPATH_RANGE)
			{
				if (!stalling)
				{
					log("Unit ID: %d, find path again due to cur_dest occupied", id);
					tryToFindPath();
				}
				else
					log("Unit ID: %d, Already stalling!", id);
			}
			else
				log("Unit ID: %d, abandon cur_dest again due to cur_dest occupied and in the range of final_dest", id);
		}

	if (hasArrivedAtDest())
		if (grid_path.size())
		{
			cur_dest = grid_path.back();
			grid_path.pop_back();
		}
		else
		{
			log("Unit ID: %d, stop move", id);
			moving = false;
		}
}

void Unit::attack()
{
	if (!cd)
	{
		unit_manager->msgs->add_game_message()->genGameMessage(GameMessage::CmdCode::GameMessage_CmdCode_ATK, id, target_id, atk, camp, 0, {});

		cd = cd_max;
	}
	else
		cd--;
}

void Unit::stall()
{
	if (stl_cnt > 0)
		--stl_cnt;
	else
		if (camp == unit_manager->player_id)
			tryToFindPath();
}

void Unit::trace()
{
	GridPoint target_gp = unit_manager->getUnitPosition(target_id);
	Point target_fp = grid_map->getPointWithOffset(target_gp);
	Point last_fp = grid_map->getPointWithOffset(target_lastpos);
	Point cur_fp = grid_map->getPointWithOffset(cur_pos);//getPosition();
	Vec2 dist_vec = target_fp - cur_fp;
	Vec2 offset_vec = target_fp - last_fp;

	if (target_gp == GridPoint(-1, -1))
		tracing = false;
	else
		if (dist_vec.length() < atk_range)
		{
			/*cur_dest = cur_pos;
			final_dest = cur_pos;
			grid_path.clear();*/
			if (moving)
				unit_manager->msgs->add_game_message()->genGameMessage(GameMessage::CmdCode::GameMessage_CmdCode_UDP, id, 0, 0, camp, 0, {cur_pos});
			attack();
		}
		else
			if (offset_vec.length() > TRACING_SENSOR * dist_vec.length() && camp == unit_manager->player_id)
			{
				target_lastpos = target_gp;
				final_dest = target_gp;
				if (rfp_cnt)
					rfp_cnt--;
				log("Unit %d, Tracing FP", id);
				if (!stalling)
					tryToFindPath();
				else
					log("Unit ID: %d, Already stalling!", id);
			}
}

void Unit::auto_atk()
{
	GridPoint target_gp = unit_manager->getUnitPosition(target_id);
	Point target_fp = grid_map->getPointWithOffset(target_gp);
	Point cur_fp = getPosition();
	Vec2 dist_vec = target_fp - cur_fp;

	if (target_gp == GridPoint(-1, -1) || dist_vec.length() >= atk_range)
		auto_atking = false;
	else
		attack();
}

void Unit::searchForNearbyEnemy()
{
	const auto & auto_atk_rect = GridRect(cur_pos - auto_atk_range / 2, auto_atk_range);
	const auto & unit_ids = grid_map->getUnitIDAt(auto_atk_rect);
	for (auto its_id : unit_ids)
	{
		int its_camp = unit_manager->getUnitCamp(its_id);
		if (its_camp != 0 && its_camp != camp)
		{
			target_id = its_id;
			auto_atking = true;
			return;
		}
	}
	return;
}

void Unit::tryToFindPath()
{
	if (camp != unit_manager->player_id)
		return;

	if (!grid_map->checkPosition(final_dest))
	{
		final_dest = grid_map->findFreePositionNear(final_dest);

		log("Change Destination due to occupied: -> (%d, %d)", final_dest.x, final_dest.y);

	}
	GridPath grid_path = findPath(final_dest);
	if (grid_path.size())
	{
		log("Unit %d, Success FP, RFP: %d, Path Length: %d", id, rfp_cnt, grid_path.size());
		rfp_cnt = 0;
		stl_cnt = -1;
		stalling = false;
		unit_manager->msgs->add_game_message()->genGameMessage(GameMessage::CmdCode::GameMessage_CmdCode_UDP, id, 0, 0, camp, 0, grid_path);
	}
	else
	{
		//stl_cnt = unit_manager->genRandom(8 * (1 + rfp_cnt), 8 * (2 + rfp_cnt));
		stl_cnt = unit_manager->genRandom(16, 32);
		stalling = true;
		log("Unit %d, Failure FP, RFP: %d, Stall Time: %d", id, rfp_cnt, stl_cnt);
		rfp_cnt++;
		if (rfp_cnt >= MAX_PATH_FIND_TIMES)
			rfp_cnt = 1;
	}
}

GridPath Unit::findPath(const GridPoint & dest) const
{
	std::vector<std::vector<int>>& gmap = grid_map->getLogicalGridMap();
	GridPoint start = getGridPosition();

	PathFinder path_finder(gmap, start.x, start.y, dest.x, dest.y);
	path_finder.searchPath();
	path_finder.generatePath();
	GridPath _grid_path = path_finder.getPath();
	GridPath opt_path = optimizePath(_grid_path);

	return(opt_path);
}

GridPath Unit::optimizePath(const GridPath & orig_path) const
{
	int path_len = orig_path.size();
	if (path_len < 3)
		return(orig_path);

	GridPath opt_path;
	GridPoint prev_p = orig_path[0];
	GridVec prev_dir = {2, 3};
	for (int i = 1; i < path_len - 1; i++)
	{
		const auto & p = orig_path[i];
		const auto & dir = (p - prev_p).getDirectionVector();
		if (!(dir == prev_dir))
		{
			opt_path.push_back(prev_p);
			prev_dir = dir;
		}
		prev_p = p;
	}
	opt_path.push_back(orig_path[path_len - 1]);
	return(opt_path);
}

void Unit::update(float dt)
{
	++timer;

	if (moving)
		move();

	if (tracing)
		trace();

	if (camp == unit_manager->player_id)
	{
		if (stalling)
			stall();

		if (auto_atking)
			auto_atk();
		else
			if (!tracing && timer % auto_atk_freq == 0)
				searchForNearbyEnemy();
	}
}

bool UnitManager::init()
{
	return true;
}

void UnitManager::initRandomGenerator()
{
	std::random_device rd;						//采用非确定性随机数发生器产生随机数种子
	gen = std::default_random_engine(rd());		//采用默认随机数引擎产生随机数
}

void UnitManager::setMessageSet(GameMessageSet* _msgs)
{
	msgs = _msgs;
}

void UnitManager::setTiledMap(cocos2d::TMXTiledMap* _tiledMap)
{
	tiled_map = _tiledMap;
}
void UnitManager::setGridMap(GridMap* _grid_map)
{
	grid_map = _grid_map;
}

void UnitManager::setSocketClient(SocketClient* _socket_client)
{
	socket_client = _socket_client;
}

void UnitManager::setBattleScene(BattleScene * _battle_scene)
{
	battle_scene = _battle_scene;
}

void UnitManager::setNotice(Notice * _notice)
{
	notice = _notice;
}

void UnitManager::setBase(int _base_id, Base * _base, GridPoint _base_pos)
{
	base_id = _base_id;
	base = _base;
	base_pos = _base_pos;
	if (battle_scene)
		battle_scene->focusOnBase();
}

void UnitManager::setPlayerID(int _player_id)
{
	player_id = _player_id;
	next_id = _player_id;
}

GridPoint UnitManager::getUnitPosition(int _unit_id)
{
	Unit* unit = id_map.at(_unit_id);
	if (unit)
		return(unit->getGridPosition());
	else
		return{-1, -1};
}

int UnitManager::getUnitCamp(int unit_id)
{
	Unit* unit = id_map.at(unit_id);
	if (unit)
		return(unit->camp);
	else 
		return 0;
}

GridPoint UnitManager::getBasePosition()
{
	return base_pos;
}



void UnitManager::produceInBase(int _unit_type)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/traning.wav");
	if (id_map.at(base_id))
		base->startProduce(_unit_type);
}

int UnitManager::genRandom(int start, int end)
{
	std::uniform_int_distribution<> u(start, end);
	return(u(gen));
}

void UnitManager::updateUnitsState()
{
	msgs->add_game_message()->genGameMessage(GameMessage::CmdCode::GameMessage_CmdCode_EMP, 0, 0, 0, 0, 0, {});
	auto sent_msg_str = msgs->SerializeAsString();
	socket_client->send_string(sent_msg_str);
	int sent_msg_num = msgs->game_message_size();
	if (sent_msg_num)
		log("Sent Message Num: %d, Sent Message string length: %d", sent_msg_num, sent_msg_str.length());

	auto msg_str = socket_client->get_string();
	msgs->ParseFromString(msg_str);
	int recv_msg_num = msgs->game_message_size();
	if (recv_msg_num)
		log("Received Message Num: %d, Received Message String Length: %d", recv_msg_num, msg_str.length());

	for (int i = 0; i < msgs->game_message_size(); i++)
	{
		const GameMessage&  msg = msgs->game_message(i);
		//log("UnitManager: Read Message %d Success", i);
		if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_EMP)
		{
			log("Empty Message, there must be something wrong");
		}
		else
			if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_CRT)
			{

				int id = msg.unit_0();
				int camp = msg.camp();
				int unit_type = msg.unit_type();
				Unit* new_unit = createNewUnit(id, camp, unit_type, GridPoint(msg.grid_path().grid_point(0).x(), msg.grid_path().grid_point(0).y()));
				id_map.insert(id, new_unit);
			}
			else
				if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_UDP)
				{
					Unit* u0 = id_map.at(msg.unit_0());
					if (u0)
					{
						u0->rfp_cnt = 0;
						const MsgGridPath& msg_grid_path = msg.grid_path();
						if (msg_grid_path.grid_point_size())
						{
							log("Unit ID: %d, update path and start moving", msg.unit_0());
							u0->setGridPath(msg.grid_path());
							u0->motivate();
						}
					}
				}
				else
					if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_ATK)
					{
						int unitid_0 = msg.unit_0();
						int unitid_1 = msg.unit_1();
						int damage = msg.damage();
						//log("Attack! Unit %d -> Unit %d, Damage %d", unitid_0, unitid_1, damage);
						Unit* unit_1 = id_map.at(unitid_1);
						if (unit_1)
						{
							genAttackEffect(unitid_0, unitid_1);
							if (notice && unit_1->camp == player_id)
							{
								char ntc[50];
								if (unit_1->type == 5)
								{
									//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/baseunderatack.wav");
									CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/mp3/baseunderatack.mp3");
									sprintf(ntc, "Our base is under attack, damage %d", damage);
								}
								else
									sprintf(ntc, "Unit %d under attack, damage %d", unitid_1, damage);
								notice->displayNotice(ntc, 30);
							}
							if (unit_1->underAttack(damage))
							{
								if (unit_1->getType() == 5)
									checkWinOrLose(unitid_1);
								if (getUnitCamp(unitid_0) == player_id)
									battle_scene->destroyReward(unit_1->getType());
								deleteUnit(unitid_1);
							}
						}
					}
	}

	msgs->clear_game_message();
}

void UnitManager::deleteUnit(int id)
{
	Unit* unit = id_map.at(id);
	if (unit)
	{
		auto itor = std::find(selected_ids.begin(), selected_ids.end(), id);
		if (itor != selected_ids.end())
			selected_ids.erase(itor);

		unit->removeFromMaps();

		id_map.erase(id);
		//human
		if(unit->getType()== 3) 
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/die1.wav");
		else
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/bomb1.wav");
		
		if (notice && unit->camp == player_id)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/unitlost.wav");
			char ntc[50];
			sprintf(ntc, "Unit %d destroyed", id);
			notice->displayNotice(ntc, 80);
		}
	}
}

void UnitManager::checkWinOrLose(int destroyed_base_id)
{
	int base_camp = base_map.at(destroyed_base_id);
	base_map.erase(destroyed_base_id);
	if (!battle_scene)
		return;
	if (base_camp == player_id)
		battle_scene->lose();
	else
		if (base_map.size() == 1)
			battle_scene->win();

}

void UnitManager::genAttackEffect(int unit_id0, int unit_id1)
{
	//log("position %f,%f,%f,%f", cur_fp.x, cur_fp.y, target_fp.x, target_fp.y);
	Unit* unit_0 = id_map.at(unit_id0);
	Unit* unit_1 = id_map.at(unit_id1);
	if (unit_0 && unit_1)
	{
		auto trajectory_effect = TrajectoryEffect::create();
		trajectory_effect->setPath(unit_0->getPosition(), unit_1->getPosition());
		tiled_map->addChild(trajectory_effect, 20);
	}
}

Unit* UnitManager::createNewUnit(int id, int camp, int unit_type, GridPoint crt_gp)
{
	Unit* nu;
	Base* tmp_base;
	std::vector<std::string > pic_paths = { 
		"", "Picture/units/airplane_", "Picture/units/robot_front_", "Picture/units/footman_front_",
		"", "Picture/units/base_", "Picture/units/tower_"
	};
	std::vector<std::string > suffixes = { ".png", ".jpg" };
	
	int pic_num = camp % 4;
	std::string pic_file = pic_paths[unit_type] + std::to_string(pic_num) + suffixes[0];
	switch (unit_type)
	{
	case 1:
		nu = Fighter::create(pic_file);
		break;
	case 2:
		nu = Tank::create(pic_file);
		break;
	case 3:
		nu = Soldier::create(pic_file);
		break;
	case 5:
		tmp_base = Base::create(pic_file);
		base_map[id] = camp;
		if (camp == player_id)
			setBase(id, tmp_base, crt_gp);
		nu = tmp_base;
		break;
	case 6:
		nu = Tower::create(pic_file);
		break;
	default:
		break;
	}

	nu->unit_manager = this;
	nu->id = id;
	nu->camp = camp;
	nu->setProperties();
	nu->setAnchorPoint(Vec2(0.5, 0.5));
	nu->addToMaps(crt_gp, tiled_map, grid_map);
	nu->initBars();
	//nu->initFlag();
	nu->schedule(schedule_selector(Unit::update));

	return(nu);
}


//生成新单位测试程序
void UnitManager::genCreateMessage(int _unit_type, const GridPoint & crt_gp)
{
	auto new_msg = msgs->add_game_message();
	new_msg->genGameMessage(GameMessage::CmdCode::GameMessage_CmdCode_CRT, next_id, 0, 0, player_id, _unit_type, GridPath{ crt_gp });
	next_id += MAX_PLAYER_NUM;
}

void UnitManager::initiallyCreateUnits()
{
	auto* obj_group = tiled_map->getObjectGroup("InitialUnits");
	auto& objs = obj_group->getObjects();

	for (auto& obj : objs)
	{
		auto& dict = obj.asValueMap();
		float cx = dict["x"].asFloat();
		float cy = dict["y"].asFloat();
		int camp = dict["camp"].asInt();
		int type = dict["type"].asInt();
		GridPoint crt_gp = grid_map->getGridPoint({ cx, cy });

		if (camp == player_id && type == BASE_TYPE_NO)
		{
			base_id = next_id;
		}
		if (camp == player_id)
		{
			genCreateMessage(type, crt_gp);
		}
	}
}

void UnitManager::deselectAllUnits()
{
	for (auto& id : selected_ids)
		id_map.at(id)->hideHPBar();
	selected_ids.clear();
}


void UnitManager::selectUnits(Point select_point)
{
	if (selected_ids.size())
	{
		for (auto & id_unit : id_map)
			if (id_unit.second->camp != player_id && id_unit.second->getBoundingBox().containsPoint(select_point))
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

void UnitManager::selectUnits(Rect select_rect)
{
	deselectAllUnits();
	for (auto & id_unit : id_map)
		if (id_unit.second->camp == player_id && select_rect.containsPoint(id_unit.second->getPosition()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/selecttarget.wav");
			selected_ids.push_back(id_unit.first);
			id_unit.second->displayHPBar();
		}
	return;
}