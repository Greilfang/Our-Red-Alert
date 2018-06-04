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
	/*计算玩家移动时间函数*/
	float getPlayerMoveTime(Vec2 start_pos, Vec2 end_pos);
	/*控制目标移动*/
	void playMover(Point position, Unit * _sprite);
	/*构建选框*/
	void DrawRectArea(Point p1, Point p2);
	/*加入被选与取消被选*/
	void getClickedUnit();
	void cancellClickedUnit();
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
	
	
	
	CREATE_FUNC(CombatScene);
private:
	Point delta = { 0,0 };
	std::vector<Unit *> selected_box;//存放选中栏的所有单位
	bool is_clicked = false;
	cocos2d::Point last_touch{ 0, 0 };//鼠标最后一次碰到的点
	MouseRect* mouse_rect = nullptr;//选框
	TMXTiledMap* _combat_map = nullptr;//加载瓦片地图所用的指针
	Point _cursor_position{ 0,0 };
	UnitManager * unit_manager;
};

inline float Tri_Dsitance(Point t1, Point t2) {
#ifdef DEBUG
	log("dt:t1.x:%f t1.y:%f t2.x:%f t2.y:%f", t1.x, t1.y, t2.x, t2.y);
#endif
	return fabs(t1.x - t2.x) + fabs(t1.y - t2.y);
}
inline void CombatScene::getClickedUnit() {
	for (int i = 0; i != selected_box.size(); i++) {
		selected_box[i]->setOpacity(180);
		selected_box[i]->displayHP();
	}
}
inline void CombatScene::cancellClickedUnit() {
	for (int i = 0; i != selected_box.size(); i++) {
		selected_box[i]->setOpacity(255);
		selected_box[i]->hideHP();
	}
	selected_box.clear();
	selected_box.shrink_to_fit();
}
#endif