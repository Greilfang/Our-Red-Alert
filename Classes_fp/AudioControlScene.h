#pragma once
#ifndef AUDIO_CONTROL_SCENE_H_
#define AUDIO_CONTROL_SCENE_H_
#include <cocos2d.h>

USING_NS_CC;

class AudioControl :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void musicTogglemenuCallBack(Ref* pSender);
	void effectTogglemenuCallBack(Ref* pSender);
	CREATE_FUNC(AudioControl);
};

#endif // !SETTING_SCENE_H_

