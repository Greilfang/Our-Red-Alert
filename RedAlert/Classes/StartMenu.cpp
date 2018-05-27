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
	start_button->setTitleText("��ʼ��Ϸ");
	start_button->setTitleFontSize(16);
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.7));
	start_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			//�л���Ԥ���س���
			//Preload����һ��Ҫ���صĳ���
			auto transition = TransitionSlideInL::create(2.0, PreLoad::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(start_button);
	/*



	��Ϸ���ð�ť||�������Ӱ�ť||������Ա	
	
	
	
	
	*/
	//�˳���Ϸ��ť
	auto close_button = Button::create("button.png");
	close_button->setScale(2);
	close_button->setTitleText("�˳���Ϸ");
	close_button->setTitleFontName("΢���ź�");
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