#include "PreLoadScene.h"
#include "StartMenuScene.h"


USING_NS_CC;

Scene* PreLoad::createScene() {
	auto scene = Scene::create();
	auto layer = PreLoad::create();

	scene->addChild(layer);
	return scene;
}

bool PreLoad::init() {
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* ���ñ���ͼƬ */
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

	/*����_progress�ĸ������*/
	_progress = ProgressTimer::create(bar_sprite);
	_progress->setPercentage(0.0f);
	_progress->setScale(2);
	_progress->setMidpoint(Point(0.0f, 0.5f));
	_progress->setBarChangeRate(Point(1.0f, 0.f));
	_progress->setType(ProgressTimer::Type::BAR);
	_progress->setPosition(visibleSize.width / 2, visibleSize.height * 0.15);
	this->addChild(_progress);

	/* ��ӱ�ǩ */
	Label *label = Label::createWithTTF("Loading...", "fonts/Marker Felt.ttf", 45);
	Size size = Director::getInstance()->getVisibleSize();
	label->setPosition(Vec2(size.width / 2, size.height * 0.23));
	this->addChild(label);

	return true;
}

void PreLoad::onEnterTransitionDidFinish() {
	// ��ȡpreloadResources�ļ��е���Ϸ��Դ�����б�
	ValueMap map = FileUtils::getInstance()->getValueMapFromFile("preloadResources.plist");
	ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
	ValueVector effects = map.at("Sounds").asValueVector();
	ValueVector musics = map.at("Musics").asValueVector();

	_sourceCount = spriteSheets.size() + effects.size() + musics.size();
	// ���ý��������´���=100/��Ҫ���ص���Դ����
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
		// �Խ����ķ�ʽ��ʾͼƬ,ÿ�ζ�������0.5s
		auto display_action = ProgressFromTo::create(0.5f, _progress->getPercentage(), 100);
		auto call_function = CallFunc::create([=] {
			// ��������Դ��ɣ��ӳ�1����ת����Ϸ������
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

