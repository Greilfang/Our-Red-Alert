#pragma once
#ifndef SETTING_SCENE_H_
#define SETTING_SCENE_H_
#include <cocos2d.h>

USING_NS_CC;

class SettingScene :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(SettingScene);
};

#endif // !SETTING_SCENE_H_
