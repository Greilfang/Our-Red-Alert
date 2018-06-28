#define ASIO_STANDALONE
#pragma warning(disable:4996)
#pragma once
#ifndef COMBAT_SCENE_H_
#define COMBAT_SCENE_H_
#define DEBUG
#include "asio.hpp"
#include"Unit.h"
#include <cocos2d.h>
#include<vector>
#include "GameMessage.pb.h"
#include<chat_client.h>
#include<chat_server.h>
#include"Const.h"
USING_NS_CC;

/**
	* @brief MouseRect类继承自DrawNode类
	* @用于加载游戏中的选框的绘制和操作
*/
class MouseRect : public cocos2d::DrawNode
{
public:
	cocos2d::Point touch_start, touch_end;				//触摸事件的起始点和终点
	cocos2d::Point touch_start_map, touch_end_map;		//对应于游戏地图的起始点与终点

/**
	* @brief 用于刷新选框的显示
	*/
	void update(float f) override;
};

/**
	* @brief PowerDisplay类继承自Label类
	* @用于显示并刷新游戏中的电力数据
*/
class PowerDisplay : public Label	
{
public:
	bool init() override;
	void updateDisplay(Power * power);
	CREATE_FUNC(PowerDisplay);
};

/**
	* @brief Power类继承自DrawNode类
	* @用于显示并刷新游戏中的电力条
*/
class Power : public DrawNode
{
private:
	int max_power = INITIAL_POWER;				//电量上限
	int used_power = 0;							//当前使用的电量

	int cur_length = 0;							//当前电力条的长度
	int length = 130;							//电力条的长度
	int width = 35;								//电力条的宽度

	cocos2d::Color4F red{ 1, 0, 0, 1 };			//红色显示
	cocos2d::Color4F yellow{ 1, 1, 0, 1 };		//黄色
	cocos2d::Color4F green{ 0, 1, 0.5, 1 };		//绿色
	cocos2d::Color4F blue{ 0.1, 0.5, 1, 1 };	//蓝色
public:
	PowerDisplay * powerDisplay = nullptr;		//用于同PowerDisplay类通信的指针

/**
	* @brief 用于增加当前电量的上线
	* @param delta 电量的增量
	*/
	void addMax_power(int delta);

/**
	* @brief 新建建筑消耗当前所产生的电量
	* @param delta 该建筑消耗的电量
	*/
	void spendPower(int power);

/**
	* @brief 设置当前显示电量条的长度
	*/
	void setCur_length();

/**
	* @brief 用于检查当前电量是否能为选择的建筑提供电力
	* @param delta 如果电量充足则返回true，反之返回false
	*/
	bool checkPower(int delta);

/**
	* @brief 根据当前电量的充裕程度设置显示颜色
	* @如果电量充足则展示绿色，接下来依据电量充沛程度依次显示绿、黄、红色
	*/
	Color4F getColor();

/**
	* @brief 更新电力条的显示状态
	*/
	void updatePowerDisplay();

/**
	* @brief 友元函数，更新电力条显示状态的同时更新电力数据的显示
	* @param power 电力数据
	*/
	friend void PowerDisplay::updateDisplay(Power * power);

	CREATE_FUNC(Power);
};

/**
	* @brief Money类继承自Label类
	* @用于显示并刷新游戏中的金钱总数
*/
class Money : public Label
{
public:
/**
	* @brief 初始化金钱显示的数据
	* @return 初始化成功则返回true，反之false
	*/
	bool init() override;

/**
	* @brief 更新金钱数目
	*/
	void update(float f) override;

/**
	* @brief 更新金钱数量的显示(由update函数进行调用)
	*/
	void updateMoneyDisplay();

/**
	* @brief 检查金钱数目，判断剩余金钱能否支付目标建筑
	* @param price 目标建筑的价格
	* @return 若足够支付则返回ture，反之false
	*/
	bool checkMoney(int price) const;

/**
	* @brief 建造建筑后扣除金钱
	* @param cost 目标建筑的建造代价
	*/
	void spendMoney(int cost);

/**
	* @brief 得到当下金钱增长的速度的值
	* @return 金币增长速率
	*/
	int getIncreasingAmount() const;

/**
	* @brief 设置当下金钱增长的速度的值
	* @param amount 增长幅度(矿场的特性)
	*/
	void setIncreasingAmount(int amount);
	CREATE_FUNC(Money);
private:
	int money = 0;				//金钱总数
	int timer = 0;				//游戏所经历的帧数
	int update_period = 20;		//刷新周期
	int increase_amount = 0;	//金钱随帧数增加的幅度(与矿场数目成正比)
};

/**
	* @brief mini类继承自Sprite类
	* @用于显示游戏中的小地图
*/
class Minimap :public Sprite {
public:
/**
	* @brief 创建小地图
	* @param filename 小地图图片的路径
	* @return 指向当前小地图的指针
	*/
	static Minimap *create(const std::string &filename);

/**
	* @brief 标志小地图上的各方单位
	* @param index 单位的id
	* @param target 单位的坐标
	*/
	void addpoint(int index, Point & target);

	DrawNode* drawNode = nullptr;	//小地图单位的标注
private:
	float mini_width;		//小地图的宽度
	float mini_height;		//小地图的高度
};

/**
	* @CombatScene类继承自Layer类
	* @是整个游戏的主场景
	* @游戏初期我们将主要的功能全部加载在这个类中，同时也是进行新功能测试的地方
	* @随着开发的进行将控制类、格点类、单位类全部转移了出去，构建了现在的Unit类、GridMap类、PathFinding类
	* @只保留了游戏的初始化、加载地图、加载选框、移动地图等基本游戏操作
*/
class CombatScene :public Layer{
public:
	EventListenerTouchOneByOne * destListener = nullptr;			//鼠标监听器
	EventListenerKeyboard * letterListener = nullptr;				//键盘监听器

	Point cdelta = { 0,0 };											//地图偏移量
	GameMessageSet * msgs;											//指向消息模块的指针
	Power * power = nullptr;										//指向电力信息的指针
	Money * money = nullptr;										//指向金钱信息的指针

/**
	* @brief 构建选框
	* @param p1 鼠标事件的起始点
	* @param target 鼠标事件的终点
	*/
	void DrawRectArea(Point p1, Point p2);

/**
	* @brief 向selected_box中收入被选中的节点
	* @param p1 选框的初始点
	* @param p2 选框的终点
	*/
	void getLayerUnit(Point p1, Point p2);

/**
	* @brief 拖动地图 
	* @当鼠标移到地图边界10~40像素时移动地图，0~10像素时速度翻倍
	* @当按下键盘上下左右键时也能移动地图显示位置
	*/
	void dragMap();

/**
	* @brief 刷新小地图的显示
	*/	
	void updateMircoLocation();

/**
	* @brief 用于拖动地图后的视点位置的刷新
	*/
	virtual void update(float f);

/**
	* @brief 用于返回selected_box
	*/
	static Scene* createScene(chat_server * server_context_, chat_client * client_context_);
	virtual bool init(chat_server * server_context_, chat_client * client_context_);
	
/**
	* @brief 将视野中心设置为基地 
	*/
	void focusOnBase();
	
	static CombatScene* create(chat_server * server_context_, chat_client * client_context_);
	
private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;	//记录键盘事件的类型

	chat_client * client_side = nullptr;					//服务端指针
	chat_server * server_side = nullptr;					//服务端指针
	TMXTiledMap* _combat_map = nullptr;						//加载瓦片地图所用的指针
	UnitManager * unit_manager;								//游戏控制器的指针
	GridMap * _grid_map = nullptr;							//格点地图
	MouseRect* mouse_rect = nullptr;						//选框
	Minimap * mini_map = nullptr;							//小地图

	bool is_clicked = false;								//flag，记录是否点击
	cocos2d::Point last_touch{ 0, 0 };						//鼠标最后一次碰到的点
	Point _cursor_position{ 0,0 };							//鼠标位置
	int message_update = 0;									//消息的更新
};

inline float Tri_Dsitance(Point t1, Point t2) {
#ifdef DEBUG
	log("dt:t1.x:%f t1.y:%f t2.x:%f t2.y:%f", t1.x, t1.y, t2.x, t2.y);
#endif
	return fabs(t1.x - t2.x) + fabs(t1.y - t2.y);
}

#endif