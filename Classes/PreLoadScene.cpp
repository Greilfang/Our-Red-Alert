#include "PreLoadScene.h"
#include "StartMenuScene.h"


USING_NS_CC;

Scene* PreLoad::createScene() {
	auto scene = Scene::create();
	auto layer = PreLoad::create();

	scene->addChild(layer);
	return scene;
}

bool PreLoad::hasPlayedBgm = false;

bool PreLoad::init() {
	if (!Layer::init()) {
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* 设置背景图片 */
	auto back_ground = Sprite::create("background3.png");
	back_ground->setPosition(origin + visibleSize / 2);
	back_ground->setScaleX(visibleSize.width / back_ground->getContentSize().width);
	back_ground->setScaleY(visibleSize.height / back_ground->getContentSize().height);
	addChild(back_ground, -5);

	auto bar_sprite = Sprite::create("slider2.png");
	auto bar_back_ground_sprite = Sprite::create("slider1.png");
	bar_back_ground_sprite->setScale(2);
	bar_back_ground_sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.15));
	this->addChild(bar_back_ground_sprite);

	/*设置_progress的各项参数*/
	_progress = ProgressTimer::create(bar_sprite);
	_progress->setPercentage(0.0f);
	_progress->setScale(2);
	_progress->setMidpoint(Point(0.0f, 0.5f));
	_progress->setBarChangeRate(Point(1.0f, 0.f));
	_progress->setType(ProgressTimer::Type::BAR);
	_progress->setPosition(visibleSize.width / 2, visibleSize.height * 0.15);
	this->addChild(_progress);

	/* 添加标签 */
	Label *label = Label::createWithTTF("Loading...", "fonts/Marker Felt.ttf", 45);
	Size size = Director::getInstance()->getVisibleSize();
	label->setPosition(Vec2(size.width / 2, size.height * 0.23));
	this->addChild(label);

	return true;
}

void PreLoad::onEnterTransitionDidFinish() {
	// 读取preloadResources文件中的游戏资源名称列表
	ValueMap map = FileUtils::getInstance()->getValueMapFromFile("preloadResources.plist");
	ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
	ValueVector effects = map.at("Sounds").asValueVector();
	ValueVector musics = map.at("Musics").asValueVector();

	_sourceCount = spriteSheets.size() + effects.size() + musics.size();
	// 设置进度条更新次数=100/需要加载的资源数量
	_progressInterval = 100 / _sourceCount;

	loadMusic(musics);
	loadEffect(effects);
	loadSpriteSheets(spriteSheets);
}

void PreLoad::loadMusic(ValueVector musicFiles) {
	for (Value &v : musicFiles) {
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(v.asString().c_str());
		progressUpdate();
	}
}

void PreLoad::loadEffect(ValueVector effectFiles) {
	for (Value &v : effectFiles) {
		SimpleAudioEngine::getInstance()->preloadEffect(v.asString().c_str());
		progressUpdate();
	}
}

void PreLoad::loadSpriteSheets(ValueVector spriteSheets) {
	for (Value &v : spriteSheets) {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(v.asString().c_str());
		progressUpdate();
	}
}

void PreLoad::progressUpdate() {
	if (--_sourceCount) {
		_progress->setPercentage(100.0f - (_progressInterval * _sourceCount));
	}
	else {
		// 以渐进的方式显示图片,每次动作持续0.5s
		auto display_action = ProgressFromTo::create(0.5f, _progress->getPercentage(), 100);
		auto call_function = CallFunc::create([=] {
			// 当加载资源完成，延迟1秒跳转到游戏主场景
			auto delay = DelayTime::create(1.0f);
			auto call_function = CallFunc::create([] {
				Director::getInstance()->replaceScene(GameMenu::createScene());
			});
			auto action = Sequence::create(delay, call_function, NULL);
			this->runAction(action);
		});
		auto action = Sequence::create(display_action, call_function, NULL);

		_progress->runAction(action);
	}
}

