#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

#pragma once
class GameMenu : public cocos2d::Layer
{
	/*��������*/
	static cocos2d::Scene * createScene();
	/*��ʼ��*/
	virtual bool init();
	/*ʹ�ú괴����ǰ��Ķ����ͷ��ڴ�*/
	CREATE_FUNC(GameMenu);
};
#endif