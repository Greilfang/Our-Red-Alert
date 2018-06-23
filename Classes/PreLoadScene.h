#pragma once
#ifndef PRELOAD_SCENE_H_
#define PRELOAD_SCENE_H_

#include <cocos2d.h>
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace CocosDenshion;

class PreLoad :public cocos2d::Layer {
protected:
	// 游戏需要加载的资源总数
	int _sourceCount;
	// 进度条
	ProgressTimer* _progress;
	// 进度条更新频率
	float _progressInterval;
public:
	static bool hasPlayedBgm;
	static cocos2d::Scene* createScene();
	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void loadMusic(ValueVector musicFiles);
	void loadEffect(ValueVector effectFiles);
	void loadSpriteSheets(ValueVector spriteSheets);
	void loadAnimations(ValueVector animations);
	void progressUpdate();

	CREATE_FUNC(PreLoad);
};

#endif // !PRELOAD_SCENE_H_
