#pragma once
#ifndef COMBAT_SCENE_H_
#define COMBAT_SCENE_H_

#include <cocos2d.h>

USING_NS_CC;

class MouseRect :public DrawNode {
public:
	Point _touch_start, _touch_end;
	Point _start, _end;

	virtual void update(float f);
	CREATE_FUNC(MouseRect);
};

class CombatScene :public Layer{
public:
	static Scene* createScene();
	virtual bool init();

	virtual void update(float f);
	void scrollMap();

	CREATE_FUNC(CombatScene);
private:
	TMXTiledMap* _combat_map = nullptr;
	
	MouseRect* _mouse_rect = nullptr;

	Point _cursor_position{ 0,0 };
};

#endif