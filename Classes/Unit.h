#pragma once
#ifndef _UNIT_H_
#define _UNIT_H_

#include <iostream>
#include <cocos2d.h>
#include "GameMessage.pb.h"
#include "ui/CocosGUI.h"
#include "GridMap.h"

USING_NS_CC;
using namespace ui;

class Unit;
class UnitManager;
class Bar;
class Building;
class CombatScene;

class Bar :public DrawNode {
private:
	int timer = 0;//展示血条的计时器
	int display_time = 0;//设置展示时间
	float length = 36;//血条长
	float width = 4;//血条宽
	bool is_visible = false;//是否显示
	cocos2d::Color4F color{ 0.8, 0, 0, 0.8 };
public:
	//void updateHide(float f);
	void updateBarDisplay(float rate);
	void setLength(float _length) { length = _length; };
	void setColor(const cocos2d::Color4F& _color) { color = _color; };
	void keepVisible();
	void stopKeepingVisible();
	CREATE_FUNC(Bar);

};

class UnitManager : public cocos2d::Ref
{
public:
	int player_id = 0;
	GameMessageSet* msgs = nullptr;
	GridMap* grid_map = nullptr;
	EventListenerTouchOneByOne * spriteTouchListener;
	std::vector<int> selected_ids;

	CREATE_FUNC(UnitManager);
	bool init();
	void setMessageSet(GameMessageSet* _msgs);
	void setSpriteTouchListener(EventListenerTouchOneByOne * _spriteTouchListener);
	void setTiledMap(cocos2d::TMXTiledMap* _tiledMap);
	void setGridMap(GridMap *);
	void setPlayerID(int _player_id);
	void setCombatScene(CombatScene* _combat_scene);
	void setBuilding(Building * _building);
	//获取运动的时间
	float getPlayerMoveTime(Vec2 start_pos, Vec2 end_pos, int _speed);
	//执行运动的操作
	void playMover(Point position, Unit * _sprite);

	CombatScene * getCombatScene();
	EventListenerTouchOneByOne * getSpriteTouchListener();
	int  getUnitCamp(int unit_id);

	//生成新单位的信息
	void genCreateMessage(int _unit_type, int camp, float x, float y);
	void updateUnitsState();

	void initializeUnitGroup();

	void setBasePosition(Point base_pos);
	Point getBasePosition()const;

	//当点击空地时的操作
	void selectEmpty(Point position);
	//当点击单位时的操作（我方，他方)
	void selectPointUnits(Unit * _unit);
	//对selected_ids里单位的操作
	void getClickedUnit();
	//清空selected_ids
	void cancellClickedUnit();
private:
	cocos2d::Map<int, Unit*> id_map;
	std::map<int, int> base_map;

	cocos2d::TMXTiledMap* tiled_map = nullptr;
	CombatScene* combat_scene = nullptr;

	int next_id = 1;
	int base_id = 1;

	Point _base_pos{ 0,0 };

	Building * building = nullptr;
	Unit* createNewUnit(int id, int camp, int uint_type, float x, float y);
	void genAttackEffect(int unit_id0, int unit_id1);
};

class Unit :public cocos2d::Sprite {
protected:
	GameMessageSet * msgs = nullptr;
	TMXTiledMap * tiled_map = nullptr;
	GridMap* grid_map = nullptr;
	Layer* combat_scene;
	EventListenerTouchOneByOne * spriteTouchListener;
	int type;
	bool mobile;
	bool is_attack;
	bool selected = false;//是否被选中当前位置和当前目标
	int current_life=100;
	int max_life=100;
	int attack_frequency ;
	double attack_range ;
	int speed ;
	Bar* hp_bar = nullptr;//用来给单位创建血条
public:
	//设置单位所在的层，地图指针和Sprite监听器
	void setGridMap(GridMap *);
	void set(TMXTiledMap *, Layer *, EventListenerTouchOneByOne *);
	virtual void setListener();
	void setUnitManager(UnitManager*);
	
	int id;
	int camp = 0;
	int z_index;
	int attack_id;
	int attack_freq = 50;
	int timer = 0;
	UnitManager* unit_manager = nullptr;
	
	static Unit* create(const std::string & filename);

	virtual void setProperties();
	//virtual void addToMaps(cocos2d::TMXTiledMap* _tiled_map);
	//void removeFromMaps();

	virtual void setCamp(int _camp);
	void setMobile(bool can);
	int getCamp() const;
	int getType() const;
	int getSpeed() const;
	bool isMobile();
	//构造函数
	Unit(int _max_life, int _atk_freq, double _atk_range, int _speed) 
		:max_life(_max_life), attack_frequency(_atk_freq), attack_range(_atk_range), speed(_speed) 
	{ ; };
	Unit() :max_life(100), attack_frequency(1), attack_range(30), speed(10) { ; };
	//判断单位是不是在某个范围内
	bool is_in(Point p1, Point p2);
	//对血条的显示和隐藏操作
	void displayHP();
	void hideHP();
	void attack();
	void searchEnemy();
	bool underAttack(int damage);
	friend void UnitManager::updateUnitsState();
	virtual void update(float f);
};
class TrajectoryEffect : public cocos2d::ParticleFire
{
public:
	virtual bool init() override;
	void setPath(cocos2d::Vec2, cocos2d::Vec2);

	CREATE_FUNC(TrajectoryEffect);
private:
	void updatefire(float);
	cocos2d::Vec2 from_, to_, move_;
	int speed_ = 3;

};


#endif
