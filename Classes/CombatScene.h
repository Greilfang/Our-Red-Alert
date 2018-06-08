#pragma once
#ifndef COMBAT_SCENE_H_
#define COMBAT_SCENE_H_
#define DEBUG

#include"Unit.h"
#include <cocos2d.h>
#include<vector>
#include "GameMessage.pb.h"
USING_NS_CC;

class MouseRect : public cocos2d::DrawNode
{
public:
	
	CREATE_FUNC(MouseRect);
	cocos2d::Point touch_start, touch_end;
	void update(float f) override;
};


class CombatScene :public Layer{
public:
	GameMessageSet * msgs;
	/*构建选框*/
	void DrawRectArea(Point p1, Point p2);
	/*向selected_box中收入被选中的节点*/
	void getLayerUnit(Point p1, Point p2);
	/*滚动地图*/
	void scrollMap();
	/*用于滚动地图后的刷新*/
	virtual void update(float f);
	/*用于返回selected_box*/
	//std::vector<Unit *> & getSelected_box();
	static Scene* createScene();
	virtual bool init();
	
	/* 视野中心为基地 */
	void focusOnBase();
	
	
	CREATE_FUNC(CombatScene);
private:
	Point delta = { 0,0 };
	bool is_clicked = false;
	cocos2d::Point last_touch{ 0, 0 };//鼠标最后一次碰到的点
	MouseRect* mouse_rect = nullptr;//选框
	TMXTiledMap* _combat_map = nullptr;//加载瓦片地图所用的指针
	GridMap * _grid_map = nullptr;
	Point _cursor_position{ 0,0 };
	UnitManager * unit_manager;
};

inline float Tri_Dsitance(Point t1, Point t2) {
#ifdef DEBUG
	log("dt:t1.x:%f t1.y:%f t2.x:%f t2.y:%f", t1.x, t1.y, t2.x, t2.y);
#endif
	return fabs(t1.x - t2.x) + fabs(t1.y - t2.y);
}

#endif