#include "SettingScene.h"
#include "ui/CocosGUI.h"
#include "StartMenuScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace ui;
USING_NS_CC;

cocos2d::Scene * SettingScene::createScene() {
	auto scene = Scene::create();
	auto layer = SettingScene::create();

	scene->addChild(layer);
	return scene;
}

bool SettingScene::init() {
	if (!Layer::create()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* BackGround */
	auto back_ground = Sprite::create("background2.png");
	back_ground->setPosition(origin + visibleSize / 2);
	back_ground->setScaleX(visibleSize.width / back_ground->getContentSize().width);
	back_ground->setScaleY(visibleSize.height / back_ground->getContentSize().height);
	addChild(back_ground, -5);

	/* BgmVolume */
	auto volume_label = Label::createWithTTF("BGM VOLUME", "fonts/Marker Felt.ttf", 32);
	volume_label->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7));
	this->addChild(volume_label);

	auto volume_slider = Slider::create();
	volume_slider->loadBarTexture("slider1.png");
	volume_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	volume_slider->loadProgressBarTexture("slider2.png");
	volume_slider->setPercent(SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() * 100);
	volume_slider->setPosition(Vec2(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height * 0.7));

	volume_slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
			auto volume_percent = volume_slider->getPercent();
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((float)volume_percent / 100);
		}
	});
	this->addChild(volume_slider);

	/* EffectVolume */
	auto effects_volume_label = Label::createWithTTF("EFFECTS VOLUME", "fonts/Marker Felt.ttf", 32);
	effects_volume_label->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5));
	this->addChild(effects_volume_label);

	auto effects_volume_slider = Slider::create();
	effects_volume_slider->loadBarTexture("slider1.png");
	effects_volume_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	effects_volume_slider->loadProgressBarTexture("slider2.png");
	effects_volume_slider->setPercent(SimpleAudioEngine::getInstance()->getEffectsVolume() * 100);
	effects_volume_slider->setPosition(Vec2(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height * 0.5));

	effects_volume_slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
			auto effects_volume_percent = volume_slider->getPercent();
			SimpleAudioEngine::getInstance()->setEffectsVolume((float)effects_volume_percent / 100);
		}
	});
	this->addChild(effects_volume_slider);

	/* ReturnButton */
	auto return_button = Button::create("button.png");
	return_button->setScale(2);
	return_button->setTitleText("Return");
	return_button->setTitleFontName("fonts/Marker Felt.ttf");
	return_button->setTitleFontSize(16);
	return_button->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.x + visibleSize.height*0.3));

	return_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto transition = TransitionFlipY::create(0.5, GameMenu::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(return_button);

	return true;
}