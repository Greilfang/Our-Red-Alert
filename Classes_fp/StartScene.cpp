#include "StartScene.h"
#include "GameMenu.h"
#include "ui\CocosGUI.h"

using namespace ui;
USING_NS_CC;

cocos2d::Scene * StartScene::createScene() {
	auto scene = Scene::create();
	auto layer = StartScene::create();

	scene->addChild(layer);
	return scene;
}

bool StartScene::init() {
	if (!Layer::init()) {
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* BackGround */
	auto back_ground = Sprite::create("background4.png");
	back_ground->setPosition(origin + visibleSize / 2);
	back_ground->setScaleX(visibleSize.width / back_ground->getContentSize().width);
	back_ground->setScaleY(visibleSize.height / back_ground->getContentSize().height);
	addChild(back_ground, -5);

	/* ServerMenu */
	auto server_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(StartScene::menuServerCallback, this));
	server_menu->setScale(2);
	auto server_label = Label::createWithTTF("Create Game", "fonts/Marker Felt.ttf", 32);
	server_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.7);
	this->addChild(server_label);

	/* ClientMenu */
	auto client_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(StartScene::menuClientCallback, this));
	client_menu->setScale(2);
	auto client_label = Label::createWithTTF("Join Game", "fonts/Marker Felt.ttf", 32);
	client_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.5);
	this->addChild(client_label);

	/* ReturnMenu */
	auto back_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(StartScene::menuBackCallback, this));
	back_menu->setScale(2);
	auto back_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	back_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.3);
	this->addChild(back_label);

	/* AddThreeMenu */
	auto menu = Menu::create(server_menu, client_menu, back_menu, NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height / 2));
	menu->alignItemsVerticallyWithPadding(35);
	this->addChild(menu, -2);

	return true;
}

void StartScene::menuServerCallback(cocos2d::Ref* pSender) {
	auto scene = ServerMenu::createScene();
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}

void StartScene::menuClientCallback(cocos2d::Ref* pSender) {
	auto scene = ClientMenu::createScene();
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}

void StartScene::menuBackCallback(cocos2d::Ref* pSender) {
	auto scene = GameMenu::createScene();
	Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(0.5, scene));
}

cocos2d::Scene * ServerMenu::createScene() {
	auto scene = Scene::create();
	auto layer = ServerMenu::create();

	scene->addChild(layer);
	return scene;
}

bool ServerMenu::init() {
	if (!Layer::init()){
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* BackGround */
	auto back_ground = Sprite::create("background4.png");
	back_ground->setPosition(origin + visibleSize / 2);
	back_ground->setScaleX(visibleSize.width / back_ground->getContentSize().width);
	back_ground->setScaleY(visibleSize.height / back_ground->getContentSize().height);
	addChild(back_ground, -5);

	/* InputBox */


	/* StartMenu */

	/* BackMenu */

	return true;
}

void ServerMenu::menuStartServerCallback(cocos2d::Ref * pSender) {
}

void ServerMenu::menuStartGameCallback(cocos2d::Ref * pSender) {
}

void ServerMenu::menuBackCallback(cocos2d::Ref * pSender) {
}

cocos2d::Scene * ClientMenu::createScene() {
	auto scene = Scene::create();
	auto layer = ClientMenu::create();

	scene->addChild(layer);
	return scene;
}

bool ClientMenu::init() {
	return false;
}

void ClientMenu::menuStartGameCallback(cocos2d::Ref * pSender) {
}

void ClientMenu::menuBackCallback(cocos2d::Ref * pSender) {
}
