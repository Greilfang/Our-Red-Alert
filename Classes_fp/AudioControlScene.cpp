#include "AudioControlScene.h"
#include "ui/CocosGUI.h"
#include "GameMenu.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace ui;
USING_NS_CC;

cocos2d::Scene * AudioControl::createScene() {
	auto scene = Scene::create();
	auto layer = AudioControl::create();
	scene->addChild(layer);
	return scene;
}

bool AudioControl::init() {
	if (!Scene::create()) {
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
	auto music_label = Label::createWithTTF("BGM VOLUME", "fonts/Marker Felt.ttf", 32);
	music_label->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7));
	this->addChild(music_label);

	auto music_slider = Slider::create();
	music_slider->loadBarTexture("sliderTrack.png");
	music_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	music_slider->loadProgressBarTexture("sliderProgress.png");
	float musicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");
	if (musicPercent == 0.0f)
	{
		musicPercent = 100.0f;
	}

	music_slider->setPercent(musicPercent);
	music_slider->setPosition(Vec2(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height * 0.7));

	music_slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
			int percent = music_slider->getPercent();
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((float)(percent) / 100);
			UserDefault::getInstance()->setFloatForKey("musicPercent", percent);
		}
	});
	this->addChild(music_slider);

	/* EffectVolume */
	
	auto effect_label = Label::createWithTTF("EFFECT VOLUME", "fonts/Marker Felt.ttf", 32);
	effect_label->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5));
	this->addChild(effect_label);

	auto effect_slider = Slider::create();
	effect_slider->loadBarTexture("sliderTrack.png");
	effect_slider->loadSlidBallTextures("sliderThumb.png", "sliderThumb.png", "");
	effect_slider->loadProgressBarTexture("sliderProgress.png");
	float effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
	if (effectPercent == 0.0f)
	{
		effectPercent = 100.0f;
	}

	effect_slider->setPercent(effectPercent);
	effect_slider->setPosition(Vec2(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height * 0.5));

	effect_slider->addEventListener([=](Ref* pSender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
			int percent = effect_slider->getPercent();
			SimpleAudioEngine::getInstance()->setEffectsVolume((float)(percent) / 100);
			UserDefault::getInstance()->setFloatForKey("effectPercent", percent);
		}
	});
	this->addChild(effect_slider);
	/*music_toggle_menu: ON/OFF*/
	MenuItemFont::setFontSize(32);
	auto music_toggle_menu = MenuItemToggle::createWithTarget(this, menu_selector(AudioControl::musicTogglemenuCallBack), MenuItemFont::create("ON"), MenuItemFont::create("OFF"), NULL);
	music_toggle_menu->setPosition(Vec2(origin.x + visibleSize.width * 0.92, origin.y + visibleSize.height * 0.7));
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	{
		music_toggle_menu->setSelectedIndex(1);
		UserDefault::getInstance()->setFloatForKey("index", 1);
	}
	else
	{
		music_toggle_menu->setSelectedIndex(0);
		UserDefault::getInstance()->setFloatForKey("index", 0);
	}
	
	/*effect_toggle_menu: ON/OFF*/
	
	auto effect_toggle_menu = MenuItemToggle::createWithTarget(this, menu_selector(AudioControl::effectTogglemenuCallBack), MenuItemFont::create("ON"), MenuItemFont::create("OFF"), NULL);
	effect_toggle_menu->setPosition(Vec2(origin.x + visibleSize.width * 0.92, origin.y + visibleSize.height * 0.5));

	if (SimpleAudioEngine::sharedEngine()->getEffectsVolume())
	{
		effect_toggle_menu->setSelectedIndex(1);
	}
	else
	{
		effect_toggle_menu->setSelectedIndex(0);
	}
	
	auto menu = Menu::create(music_toggle_menu,effect_toggle_menu, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	
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

void AudioControl::musicTogglemenuCallBack(Ref * pSender)
{
	
	//创建临时MenuItemToggle  
	auto temp = (MenuItemToggle*)pSender;
	//根据MenuItemToggle的选项来决定音乐的开关  
	if (temp->getSelectedIndex() == 0)
	{
		//暂停播放音乐  
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	if (temp->getSelectedIndex() == 1)
	{
		//继续播放音乐  
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}

	auto musicToggleMenuItem = (MenuItemToggle*)pSender;
	log("musicToggleMenuItem %d", musicToggleMenuItem->getSelectedIndex());
	
}
void AudioControl::effectTogglemenuCallBack(Ref * pSender)
{
	//创建临时MenuItemToggle  
	MenuItemToggle* temp = (MenuItemToggle*)pSender;
	//根据CCMenuItemToggle的选项来决定音乐的开关  
	if (temp->getSelectedIndex() == 0)
	{
		//暂停播放音乐  
		SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	}
	if (temp->getSelectedIndex() == 1)
	{
		//继续播放音乐  
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	}
}