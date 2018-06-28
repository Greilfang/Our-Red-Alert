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
	/**
	*展示血条的计时器
	*/
	int timer = 0;
	int display_time = 0;
	float length = 36;
	float width = 4;
	float height = 5;
	bool is_visible = false;
	cocos2d::Color4F color{ 0.8, 0, 0, 0.8 };
public:
	/**
	*更新显示血条
	*/
	void updateBarDisplay(float rate);
	void setLength(float _length) { length = _length; };
	void setHeight(float _height) { height = _height; };
	void setColor(const cocos2d::Color4F& _color) { color = _color; };
	void keepVisible();
	void stopKeepingVisible();
	CREATE_FUNC(Bar);

};


class ConstructRange : public DrawNode{
private:
	Color4F color{ 0.5,0.8,0.95,0.5 };
public:
	/**
	*显示可建造范围(圆)
	*参数 center:圆心,radius:半径
	*/
	void drawRange(Point center,float radius);
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

	//获取随机数
	int genRandom(int start, int end);

	CombatScene * getCombatScene();
	TMXTiledMap * getTiledMap();
	EventListenerTouchOneByOne * getSpriteTouchListener();
	int  getUnitCamp(int unit_id);

	/**
	*发送生成新单位的信息
	*参数 _unit_type:新单位的类型,camp:新单位的阵营,x、y: 新单位出生位置
	*/
	void genCreateMessage(int _unit_type, int camp, float x, float y);
	/**
	*处理接受到的各类信息: 创建:CRT,攻击:ATK,移动:MOV,聊天:CHT
	*/
	void updateUnitsState();
	/**
	*根据地图信息创建初始单位
	*/
	void initializeUnitGroup();
	/**
	*播放基地创建的动画
	*参数 base:需要播放动画的Base*对象
	*/
	void playBaseCreateAnimation(Base * base);
	/**
	*将电力上限提高 delta
	*/
	void setMax_power(int delta);
	/**
	*每秒增加金钱提高 amount
	*/
	void setIncreasingAmount(int amount);
	/**
	*保存本阵营基地位置
	*/
	void setBasePosition(Point base_pos);
	/**
	*根据敌我位置确定攻击敌方时停在那个位置
	*参数 attacker:发出攻击的单位,target:被攻击的单位
	*返回值 GridPoint 攻击方目的地格点坐标
	*/
	GridPoint getAttackPoint(Unit * attacker,Unit * target);
	Point getBasePosition()const;
	/**
	*根据id删除单位
	*/
	void deleteUnit(int id);
	/**
	*将Size坐标转换为GridSize坐标
	*/
	GridSize getGridSize(Size size);
	/**
	*将Point坐标转换为GridPoint坐标
	*/
	GridPoint getGridPoint(Point point);
	/**
	*将GridPoint坐标转换为Point坐标
	*/
	Point getPoint(GridPoint grid_point);
	/**
	*根据中心坐标center和范围size返回对应的GridRect
	*/
	GridRect getGridRect(Point center, Size size);
	/**
	*点击空地时将己方合适的单位移动至position的位置
	*/
	void selectEmpty(Point position);
	/**
	*当点击精灵单位时,根据敌我进行不同的处理
	*/
	void selectPointUnits(Unit * _unit);
	/**
	*对选中的己方单位进行处理
	*/
	void getClickedUnit();
	/**
	*清空selected_ids
	*/
	void cancellClickedUnit();
	/**
	*判断胜负
	*/
	void checkWinOrLose(int base_id);
	/**
	*为每个新加入的player分配阵营标识
	*/
	void setPlayerNum(chat_client* _socket_client);
private:
	cocos2d::Map<int, Unit*> id_map;
	std::map<int, int> base_map;
	std::default_random_engine gen;
	cocos2d::TMXTiledMap* tiled_map = nullptr;
	CombatScene* combat_scene = nullptr;

	int next_id;

	Point _base_pos{ 0,0 };
	/**
	*创建新单位
	*参数 _unit_type:新单位的类型,camp:新单位的阵营,x、y: 新单位出生位置
	*/
	Unit* createNewUnit(int id, int camp, int uint_type, float x, float y);
	/**
	*创建从unit_id0到unit_id1的攻击特效
	*参数 unit_id0:攻击方id, unit_id1:被攻击方id
	*/
	void genAttackEffect(int unit_id0, int unit_id1);

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
	/**
	*count of delay 延迟寻路计数器
	*/
	int del_cnt = -1;

	int type;
	bool mobile;


	bool is_moving = false;
	/**
	*count of delay 延迟寻路计数器
	*/
	bool is_delaying = false; 
	/**
	*是否被选中当前目标
	*/
	bool selected = false;
	int current_life=100;
	int max_life=100;
	int ATK = 0;
	int attack_frequency ;

	int speed ;
	Bar* hp_bar = nullptr;

	/**
	*根据设定路径进行移动
	*/
	virtual void move();
	/**
	*单位延迟寻路
	*/
	void delay();
	bool hasArrivedFinalDest();	
	/**
	*寻路算法
	*/
	virtual GridPath findPath(const GridPoint& dest)const;
	/**
	*优化寻路(若连续几个格点在同一条路径上则把原先的几条GridPath合成为一条)
	*/
	GridPath optimizePath(const GridPath & origin_path) const;

	
public:
	void setGridMap(GridMap *);
	void setCurPos(const GridPoint _cur);
	/**
	*将单位加到瓦片地图并绑定监听器,将grid_map设为当前格点地图
	*/
	void set(TMXTiledMap *, GridMap * _gridMap,Layer * _combat_scene, EventListenerTouchOneByOne *);
	/**
	*将单位加到格点地图并占用对应格点
	*/
	virtual void addToGmap(Point p);
	/**
	*给单位绑定监听器
	*/
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
	/**
	*建筑占用的格点矩形
	*/
	GridRect rec;
	bool is_in_attack = false;
	static Unit* create(const std::string & filename);
	/**
	*设置延迟寻路
	*/
	void setDelayPathFinding(int cnt);
	/**
	*创建血条
	*/
	virtual void initBar();
	/**
	*设置单位属性
	*/
	virtual void setProperties();
	/**
	*将单位从瓦片地图去除，释放占用格点，播放爆炸特效
	*/
	void removeFromMaps();
	/**
	*调用find_path()寻找一条路径并且发送单位移动的信息
	*/
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
	/**
	*构造函数
	*/
	Unit(int _max_life, int _atk_freq, double _atk_range, int _speed) 
		:max_life(_max_life), attack_frequency(_atk_freq), attack_range(_atk_range), speed(_speed), rec(GridRect{ 0,0 })
	{ ; };
	Unit() :max_life(100), attack_frequency(1), speed(10), rec(GridRect{ 0,0 }) { ; };
	/**
	*判断单位是不是在某个范围内
	*/
	bool is_in(Point p1, Point p2);
	/**
	*对血条的显示和隐藏操作
	*/
	void displayHP();
	void hideHP();
	/**
	*发送单位攻击的信息
	*/
	void attack();
	/**
	*寻找攻击范围的敌人
	*/
	void searchEnemy();
	/**
	*被攻击时掉血
	*/
	bool underAttack(int damage);
	friend void UnitManager::updateUnitsState();
	/**
	*更新单位状态，并根据不同状态执行不同操作
	*/
	virtual void update(float f);
};

class TrajectoryEffect : public cocos2d::ParticleFire
{
public:
	virtual bool init() override;
	/**
	*设定攻击特效路径
	*/
	void setPath(cocos2d::Vec2, cocos2d::Vec2);

	CREATE_FUNC(TrajectoryEffect);
private:
	/**
	*更新攻击特效位置
	*/
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
