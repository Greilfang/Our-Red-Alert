#pragma once
#ifndef START_SCENE_H_
#define START_SCENE_H_
#include <cocos2d.h>

USING_NS_CC;

class StartScene :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuServerCallback(cocos2d::Ref* pSender);
	void menuClientCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(StartScene);
};

class ServerMenu :public cocos2d::Layer{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuStartServerCallback(cocos2d::Ref* pSender);
	void menuStartGameCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(ServerMenu);
};

class ClientMenu :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuStartGameCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);
};

#endif