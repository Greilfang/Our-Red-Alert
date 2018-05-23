#pragma once
#ifndef PRELOAD_SCENE_H_
#define PRELOAD_SCENE_H_

#include <cocos2d.h>
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace CocosDenshion;

class PreLoad :public cocos2d::Layer {
protected:
	// ��Ϸ��Ҫ���ص���Դ����
	int _sourceCount;
	// ������
	ProgressTimer* _progress;
	// ����������Ƶ��
	float _progressInterval;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	void loadMusic(ValueVector musicFiles);
	void loadEffect(ValueVector effectFiles);
	void loadSpriteSheets(ValueVector spriteSheets);
	void progressUpdate();

	CREATE_FUNC(PreLoad);
};

#endif // !PRELOAD_SCENE_H_
