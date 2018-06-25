#pragma once
#ifndef _UNIT_H_
#define _UNIT_H_

#include <iostream>
#include "cocos2d.h"
#include "GameMessage.pb.h"
#include "ui/CocosGUI.h"
#include <vector>
#include "GridMap.h"
#include "AStarPathFinding.h"
#include"chat_client.h"

USING_NS_CC;
using namespace ui;

class Unit;
class UnitManager;
class Bar;
class Building;
class Money;
class Power;
class Base;
class CombatScene;
class MilitaryCamp;
class TankFactary;

class Bar :public DrawNode {
private:
	int timer = 0;//展示血条的计时器
	int display_time = 0;//设置展示时间
	float length = 36;//血条长
	float width = 4;//血条宽
	float height = 5;
	bool is_visible = false;//是否显示
	cocos2d::Color4F color{ 0.8, 0, 0, 0.8 };
public:
	//void updateHide(float f);
	void updateBarDisplay(float rate);
	void setLength(float _length) { length = _length; };
	void setHeight(float _height) { height = _height; };
	void setColor(const cocos2d::Color4F& _color) { color = _color; };
	void keepVisible();
	void stopKeepingVisible();
	CREATE_FUNC(Bar);

};

class ConstructRange : public DrawNode
{
private:
	Color4F color{ 0.5,0.8,0.95,0.5 };
public:
	void drawRange(Point p,float r);
	CREATE_FUNC(ConstructRange);
};

class UnitManager : public cocos2d::Ref
{
public:
	Vector<Layer*> createLayer;
	int player_id = 0;
	int player_num;
	Base * base = nullptr;
	Money * money = nullptr;
	Power * power = nullptr;
	ConstructRange * constructRange = nullptr;
	chat_client * socket_client = nullptr;
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
	void setSocketClient(chat_client* _socket_client);
	//获取运动的时间
	float getPlayerMoveTime(Vec2 start_pos, Vec2 end_pos, int _speed);
	//执行运动的操作
	void playMover(Point position, Unit * _sprite);
	//获取随机数
	int genRandom(int start, int end);

	CombatScene * getCombatScene();
	TMXTiledMap * getTiledMap();
	EventListenerTouchOneByOne * getSpriteTouchListener();
	int  getUnitCamp(int unit_id);

	//生成新单位的信息
	void genCreateMessage(int _unit_type, int camp, float x, float y);
	void updateUnitsState();

	void initializeUnitGroup();
	void playBaseCreateAnimation(Base * s);
	void setMax_power(int delta);
	void setIncreasingAmount(int amount);
	void setUnitCreateCenter(Point center);
	void setBasePosition(Point base_pos);
	GridPoint getAttackPoint(Unit * attacker,Unit * target);
	Point getBasePosition()const;
	Point getUnitCreateCenter();

	void deleteUnit(int id);
	GridSize getGridSize(Size);
	GridRect getGridRect(Point, Size);
	GridPoint getGridPoint(Point);
	Point getPoint(GridPoint);
	//当点击空地时的操作
	void selectEmpty(Point position);
	//当点击单位时的操作（我方，他方)
	void selectPointUnits(Unit * _unit);
	//对selected_ids里单位的操作
	void getClickedUnit();
	//清空selected_ids
	void cancellClickedUnit();
	void checkWinOrLose(int base_id);
	void setPlayerNum(chat_client* _socket_client);
private:
	cocos2d::Map<int, Unit*> id_map;
	std::map<int, int> base_map;

	cocos2d::TMXTiledMap* tiled_map = nullptr;
	CombatScene* combat_scene = nullptr;

	int next_id;

	Point _base_pos{ 0,0 };
	Point unit_create_center{ 0,0 };
	Unit* createNewUnit(int id, int camp, int uint_type, float x, float y);

	void genAttackEffect(int unit_id0, int unit_id1);
	std::default_random_engine gen;
};

class Unit :public cocos2d::Sprite {
protected:
	GameMessageSet * msgs = nullptr;
	TMXTiledMap * tiled_map = nullptr;
	GridMap* grid_map = nullptr;
	Layer* combat_scene = nullptr;
	EventListenerTouchOneByOne * spriteTouchListener = nullptr;

	GridPoint _cur_pos;
	GridPoint _cur_dest;
	Point _cur_dest_point;
	GridPoint _final_dest;
	GridPath _grid_path;
	
	//count of delay 延迟寻路计数器
	int del_cnt = -1;

	int type;
	bool mobile;


	bool is_moving = false;
	bool is_delaying = false; //延迟寻路标志

	bool selected = false;//是否被选中当前位置和当前目标
	int current_life=100;
	int max_life=100;
	int ATK = 0;
	int attack_frequency ;

	int speed ;
	Bar* hp_bar = nullptr;//用来给单位创建血条

	//移动单位
	virtual void move();
	//单位延迟寻路
	void delay();
	bool hasArrivedFinalDest();
	//寻路算法
	virtual GridPath findPath(const GridPoint& dest)const;

	//优化寻路(若连续几个格点在同一条路径上则把原先的几条GridPath合成为一条)
	GridPath optimizePath(const GridPath & origin_path) const;

	
public:
	void setGridMap(GridMap *);
	//将单位绑定监听器，并加到地图上
	void setCurPos(const GridPoint _cur);
	void set(TMXTiledMap *, GridMap * _gridMap,Layer * _combat_scene, EventListenerTouchOneByOne *);
	virtual void addToGmap(Point p);
	virtual void setListener();
	void setUnitManager(UnitManager*);
	EventListenerTouchOneByOne * getListener();
	int id;
	int camp = 0;
	int z_index;
	int attack_id;
	int attack_freq = 50;
	int timer = 0;
	UnitManager* unit_manager = nullptr;
	bool is_attack = false;
	GridSize attack_range;
	//中心位置（用于占据格点和设定攻击特效路径）
	Point _center_position;
	GridRect rec;
	bool is_in_attack = false;
	static Unit* create(const std::string & filename);

	//设置延迟寻路
	void setDelayPathFinding(int cnt);

	virtual void initBar();
	virtual void setProperties();

	void removeFromMaps();
	void tryToSearchForPath();
	GridPoint getGridPosition() const;
	void setGridPath(const MsgGridPath & msg_grid_path);
	void setDestination(const GridPoint& grid_dest);
	void setCurDestPoint(const GridPoint& grid_dest);
	void setListenerEnable(bool enable);
	virtual void setCamp(int _camp);
	void setMobile(bool can);
	int getCamp() const;
	int getType() const;
	int getSpeed() const;
	bool isMobile();
	//构造函数
	Unit(int _max_life, int _atk_freq, double _atk_range, int _speed) 
		:max_life(_max_life), attack_frequency(_atk_freq), attack_range(_atk_range), speed(_speed), rec(GridRect{ 0,0 })
	{ ; };
	Unit() :max_life(100), attack_frequency(1), speed(10), rec(GridRect{ 0,0 }) { ; };
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
	int speed_ = 7;
};

class ExplosionEffect :public cocos2d::ParticleFire
{
public:
	virtual bool init() override;

	CREATE_FUNC(ExplosionEffect);
private:
	void remove(float f);
};

#endif
