#include "StartMenuScene.h"
#include "ui/CocosGUI.h"
#include "PreLoadScene.h"
#include "AudioControlScene.h"
#include "CombatScene.h"
#include "StartScene.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

Scene* GameMenu::createScene() {
	auto scene = Scene::create();
	auto layer = GameMenu::create();

	scene->addChild(layer);
	return scene;
}

bool GameMenu::init() {
	if (!Layer::init()) {
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//BackGroundMusic
	if (!PreLoad::hasPlayedBgm)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("SovietMarch.mp3", true);
		PreLoad::hasPlayedBgm = true;
	}
	/* BackGroundPicture */
	auto back_ground = Sprite::create("background.png");
	back_ground->setPosition(origin + visibleSize / 2);
	back_ground->setScaleX(visibleSize.width / back_ground->getContentSize().width);
	back_ground->setScaleY(visibleSize.height / back_ground->getContentSize().height);
	addChild(back_ground, -5);

	/* StartGameButton */
	auto start_button = Button::create("button.png");
	start_button->setScale(2);
	start_button->setTitleText("Start Game");
	start_button->setTitleFontName("fonts/Marker Felt.ttf");
	start_button->setTitleFontSize(16);
	start_button->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height*0.7));
	start_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto transition = TransitionSlideInR::create(1.0, StartScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(start_button);

	/* SettingButton */
	auto setting_button = Button::create("button.png");
	setting_button->setScale(2);
	setting_button->setTitleText("Settings");
	setting_button->setTitleFontName("fonts/Marker Felt.ttf");
	setting_button->setTitleFontSize(16);
	setting_button->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height*0.5));
	setting_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto transition = TransitionFlipY::create(1.0, AudioControl::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(setting_button);

	/* QuitGameButton */
	auto close_button = Button::create("button.png");
	close_button->setScale(2);
	close_button->setTitleText("Quit Game");
	close_button->setTitleFontName("fonts/Marker Felt.ttf");
	close_button->setTitleFontSize(16);
	close_button->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height*0.3));
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

void buttonStartCallback(cocos2d::Ref* pSender) {

}