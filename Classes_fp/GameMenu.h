#pragma once
#ifndef  START_MENU_SCENE_H_
#define  START_MENU_SCENE_H_

#include "cocos2d.h"

#pragma once
class GameMenu : public cocos2d::Layer{
public:
	static cocos2d::Scene * createScene();
	virtual bool init();

	CREATE_FUNC(GameMenu);
};
#endif