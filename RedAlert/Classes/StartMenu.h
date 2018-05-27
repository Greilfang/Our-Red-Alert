#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

#pragma once
class GameMenu : public cocos2d::Layer
{
	/*创建场景*/
	static cocos2d::Scene * createScene();
	/*初始化*/
	virtual bool init();
	/*使用宏创建当前类的对象并释放内存*/
	CREATE_FUNC(GameMenu);
};
#endif