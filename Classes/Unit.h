#pragma once
#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"
#include "GridMap.h"
#include "GameMessage.pb.h"
#include "fudancraft.h"
#include "PathFinder/PathFinder.h"
#include "SocketClient.h"

class Unit;
class UnitManager;
class Base;
class BattleScene;
class Notice;

class Bar : public cocos2d::DrawNode
{
public:
	/**
	 * \brief update the bar
	 */
	void update(float f) override;
	/**
	 * \brief update the bar in a rate
	 * \param rate update rate
	 * \param _disp_time display time
	 */
	void updateBarDisplay(float rate, int _disp_time = 0);
	CREATE_FUNC(Bar);
	/**
	 * \brief set lenght of bar
	 * \param _length length
	 */
	void setLength(float _length);
	/**
	 * \brief set color of bar
	 * \param _color color
	 */
	void setColor(const cocos2d::Color4F& _color);
	void keepVisible();
	/**
	 * \brief stop keeping visible
	 */
	void stopKeepingVisible();
private:
	int timer = 0;
	int disp_time = 0;
	float length = 32;
	float height = 4;
	bool kept_visible = false;
	cocos2d::Color4F color{ 0.8, 0, 0, 0.8 };
};

class UnitManager : public cocos2d::Ref
{
public:
	int player_id = 0;
	GameMessageSet* msgs = nullptr;

	CREATE_FUNC(UnitManager);
	bool init();
	void initRandomGenerator();
	void setMessageSet(GameMessageSet* _msgs);
	void setTiledMap(cocos2d::TMXTiledMap* _tiledMap);
	void setGridMap(GridMap* _grid_map);
	void setPlayerID(int _player_id);
	void setSocketClient(SocketClient* _socket_client);
	void setBattleScene(BattleScene* _battle_scene);
	void setNotice(Notice* _notice);
	/**
	 * \brief update units state through socket
	 */
	void updateUnitsState();
	/**
	 * \brief delete unit by id
	 * \param id unit id
	 */
	void deleteUnit(int id);
	/**
	 * \brief check if lose or win
	 * \param destroyed_base_id the base id that destroyed
	 */
	void checkWinOrLose(int destroyed_base_id);
	/**
	 * \brief generate attack effect 
	 * \param unit_id0 attacker
	 * \param unit_id1 target unit
	 */
	void genAttackEffect(int unit_id0, int unit_id1);

	/**
	 * \brief get the grid point
	 * \param unit_id unit id
	 * \return gridpoint of the unit
	 */
	GridPoint getUnitPosition(int unit_id);
	/**
	 * \return the unit camp
	 */
	int getUnitCamp(int unit_id);
	/**
	 * \return the location of user's base
	 */
	GridPoint getBasePosition();
	/**
	 * \brief geneate message of create a new unit
	 * \param _unit_type unit type
	 * \param _crt_gp create gridpoint
	 */
	void genCreateMessage(int _unit_type, const GridPoint& _crt_gp);
	void produceInBase(int _unit_type);
	int genRandom(int start, int end);

	/**
	 * \brief initially create unit 
	 */
	void initiallyCreateUnits();
	/**
	 * \brief select unit by click
	 * \param select_point click point
	 */
	void selectUnits(cocos2d::Point select_point);
	/**
	 * \brief select units by rectangle
	 * \param select_rect select rectangle
	 */
	void selectUnits(cocos2d::Rect select_rect);
private:
	cocos2d::Map<int, Unit*> id_map;
	std::vector<int> selected_ids;
	std::map<int, int> base_map;

	cocos2d::TMXTiledMap* tiled_map = nullptr;
	GridMap* grid_map = nullptr;
	SocketClient* socket_client = nullptr;
	BattleScene* battle_scene = nullptr;
	int next_id = 1;
	int base_id = 1;

	Base* base = nullptr;
	GridPoint base_pos{0, 0};

	/**
	 * \brief create a new unit
	 * \param id id
	 * \param camp camp
	 * \param uint_type unit type
	 * \param crt_gp create grid point
	 * \return 
	 */
	Unit* createNewUnit(int id, int camp, int uint_type, GridPoint crt_gp);
	void deselectAllUnits();
	void setBase(int _base_id, Base* _base, GridPoint _base_pos);

	std::default_random_engine gen;		

	Notice* notice = nullptr;
};

class Unit : public cocos2d::Sprite
{
public:
	int id;
	int camp = 0;
	int z_index;
	UnitManager* unit_manager = nullptr;

	static Unit* create(const std::string& filename);

	/**
	 * \brief (virtual)set the properties of the unit
	 */
	virtual void setProperties();
	/**
	 * \brief update function for unit
	 */
	virtual void update(float f) override;

	/**
	 * \brief (vitrual)initialize the hp bar or others 
	 */
	virtual void initBars();
	void initFlag();
	/**
	 * \brief display hp bar
	 * \param _disp_time display fram numbers
	 */
	void displayHPBar(int _disp_time = 0);
	/**
	 * \brief hide hp bar
	 */
	void hideHPBar();
	/**
	 * \brief add to the tilemap and grid map
	 * \param crt_gp current grid map
	 * \param _tiled_map tiled_map
	 * \param _grid_map grid_map
	 */
	virtual void addToMaps(const GridPoint & crt_gp, cocos2d::TMXTiledMap* _tiled_map, GridMap* _grid_map);
	void removeFromMaps();
	/**
	 * \brief setting the location path with socket message
	 */
	GridPoint getGridPosition() const;
	void setGridPath(const MsgGridPath& _grid_path);
	/**
	 * \brief let the unit move
	 */
	virtual void motivate();
	virtual void setState(int _state);
	void setDestination(const GridPoint& grid_dest);
	void setTarget(int _target_id);
	/**
	 * \brief abandon tracing the target
	 */
	void abandonTracing();
	int getState() const;
	int getType() const;
	/**
	 * \return if the unit has arrived the destination
	 */
	bool hasArrivedAtDest();
	/**
	 * \brief set damage on unit
	 * \return true if died 
	 */
	bool underAttack(int damage);
	bool isMobile();
	cocos2d::Color4F getCampColor();

	/**
	 * \brief try to find path
	 */
	void tryToFindPath();
protected:
	int timer = 0;
	int state = 0;
	bool moving = false;
	bool tracing = false;
	bool stalling = false;
	bool auto_atking = false;
	int target_id;
	bool selected = false;
	GridPath grid_path;
	GridPoint final_dest;
	GridPoint cur_pos;
	GridPoint cur_dest;
	GridPoint target_lastpos;

	GridSize size;
	GridRect cur_grec;

	int rfp_cnt = 0;
	int roc_cnt = 0;
	int stl_cnt = -1;

	int type;
	int cd;
	int hp;

	bool mobile;

	int atk;
	int atk_range;
	int hp_max;
	int cd_max;
	float move_speed;
	int auto_atk_freq;
	GridSize auto_atk_range;
	GridSize vision_range;

	cocos2d::TMXTiledMap* tiled_map = nullptr;
	GridMap* grid_map = nullptr;

	Bar* hpbar = nullptr;
	cocos2d::DrawNode* flag = nullptr;

	/**
	 * \brief move
	 */
	virtual void move();
	/**
	 * \brief attack
	 */
	virtual void attack();
	/**
	 * \brief try to find path stall for a while
	 */
	void stall();
	/**
	 * \brief trace the enemy
	 */
	void trace();
	/**
	 * \brief auto attack
	 */
	void auto_atk();
	/**
	 * \brief auto serach for nearby enemy
	 */
	void searchForNearbyEnemy();

	/**
	 * \param dest gridpint of destnation 
	 * \return try to find path to destination
	 */
	virtual GridPath findPath(const GridPoint& dest) const;
	/**
	 * \param orig_path the original path  
	 * \return an optimized path to destination
	 */
	GridPath optimizePath(const GridPath& orig_path) const;

	friend void UnitManager::updateUnitsState();

	//friend class UnitManager;
};


class TrajectoryEffect : public cocos2d::ParticleFire
{
public:
	virtual bool init() override;
	void setPath(cocos2d::Vec2, cocos2d::Vec2);

	CREATE_FUNC(TrajectoryEffect);
private:
	void updatefire(float);
	cocos2d::Vec2 from_, to_,move_;
	int speed_ = 3;

};

class ExplosionEffect : public cocos2d::ParticleFire
{
public:
	virtual bool init() override;

	CREATE_FUNC(ExplosionEffect);
private:
	void remove(float f);
};


#endif