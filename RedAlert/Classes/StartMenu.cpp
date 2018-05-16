#include"StartMenu.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
bool GameMenu::init() {
	if (!Layer::init()) {
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto start_button = Button::create("button.png");
	start_button->setScale(2);
	start_button->setTitleText("开始游戏");
	start_button->setTitleFontSize(16);
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.7));
	start_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			//切换到预加载场景
			//Preload是下一个要加载的场景
			auto transition = TransitionSlideInL::create(2.0, PreLoad::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(start_button);
	/*



	游戏设置按钮||网络连接按钮||开发人员	
	
	
	
	
	*/
	//退出游戏按钮
	auto close_button = Button::create("button.png");
	close_button->setScale(2);
	close_button->setTitleText("退出游戏");
	close_button->setTitleFontName("微软雅黑");
	close_button->setTitleFontSize(16);
	close_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.4));
	close_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WP8)||(CC_TARGET_PLATFORM==CC_PLATFORM_WINRT)
			MessageBox("You pressed the close button.Windows Store Apps do now implement a close button!alert!");
			return;
#endif
			Director::getInstance()->end();
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
			exit(0);
#endif
		}
	});
	this->addChild(close_button);
	return true;
}