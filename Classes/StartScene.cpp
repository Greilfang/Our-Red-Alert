#define ASIO_STANDALONE
#pragma warning(disable:4996)
#pragma once
#include<iostream>
#include "StartScene.h"
#include "StartMenuScene.h"
#include "CombatScene.h"
#include "ui\CocosGUI.h"
#include "asio.hpp"
#include"chat_message.hpp"
#include"chat_server.h"
#include"chat_client.h"
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
	addChild(back_ground, 0);

	/* ServerMenu */
	auto server_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(StartScene::menuServerCallback, this));
	server_menu->setScale(2);
	auto server_label = Label::createWithTTF("Create Game", "fonts/Marker Felt.ttf", 32);
	server_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.7);
	this->addChild(server_label,2);

	/* ClientMenu */
	auto client_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(StartScene::menuClientCallback, this));
	client_menu->setScale(2);
	auto client_label = Label::createWithTTF("Join Game", "fonts/Marker Felt.ttf", 32);
	client_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.5);
	this->addChild(client_label,2);

	/* ReturnMenu */
	auto back_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(StartScene::menuBackCallback, this));
	back_menu->setScale(2);
	auto back_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	back_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.3);
	this->addChild(back_label,2);

	/* AddThreeMenu */
	auto menu = Menu::create(server_menu, client_menu, back_menu, NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height / 2));
	menu->alignItemsVerticallyWithPadding(35);
	this->addChild(menu, 1);

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
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* BackGround */
	auto back_ground = Sprite::create("background4.png");
	back_ground->setPosition(origin + visibleSize / 2);
	back_ground->setScaleX(visibleSize.width / back_ground->getContentSize().width);
	back_ground->setScaleY(visibleSize.height / back_ground->getContentSize().height);
	addChild(back_ground, 0);

	/* StartServerMenu */
	auto start_server_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ServerMenu::menuStartServerCallback, this));
	start_server_menu->setScale(2);
	auto start_server_label = Label::createWithTTF("Start Server", "fonts/Marker Felt.ttf", 32);
	start_server_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.7);
	this->addChild(start_server_label,2);

	/* StartGameMenu */
	auto start_game_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ServerMenu::menuStartGameCallback, this));
	start_game_menu->setScale(2);
	auto start_game_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 32);
	start_game_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.5);
	this->addChild(start_game_label,2);

	/* BackMenu */
	auto back_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ServerMenu::menuBackCallback, this));
	back_menu->setScale(2);
	auto back_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	back_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.3);
	this->addChild(back_label,2);

	/* AddThreeMenu */
	auto menu = Menu::create(start_server_menu, start_game_menu, back_menu, NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height / 2));
	menu->alignItemsVerticallyWithPadding(35);
	this->addChild(menu, 1);

	/* ConnectionLabel */
	auto connection_label = Label::createWithTTF("", "/fonts/arial.ttf", 18);
	connection_label->setAnchorPoint(Vec2(0.5, 0));
	connection_label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y));
	this->addChild(connection_label,2);

	return true;
}

void ServerMenu::menuStartServerCallback(cocos2d::Ref * pSender) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	server_side = chat_server::create();
	client_side = chat_client::create();

}

void ServerMenu::menuStartGameCallback(cocos2d::Ref * pSender) {
	server_side->button_start();

	auto scene = CombatScene::createScene(server_side, client_side);
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}
void ServerMenu::menuBackCallback(cocos2d::Ref * pSender) {
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}

void ServerMenu::connectionUpdate(float f) {

}

cocos2d::Scene * ClientMenu::createScene() {
	auto scene = Scene::create();
	auto layer = ClientMenu::create();

	scene->addChild(layer);
	return scene;
}

bool ClientMenu::init() {
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* BackGround */
	auto back_ground = Sprite::create("background4.png");
	back_ground->setPosition(origin + visibleSize / 2);
	back_ground->setScaleX(visibleSize.width / back_ground->getContentSize().width);
	back_ground->setScaleY(visibleSize.height / back_ground->getContentSize().height);
	addChild(back_ground, 0);

	/* IpBox */
	auto ip_box = EditBox::create(Size(150, 60), Scale9Sprite::create("button.png"));
	ip_box->setPosition(Vec2(origin.x + visibleSize.width * 0.35, origin.y + visibleSize.height * 0.7));
	ip_box->setTextHorizontalAlignment(TextHAlignment::CENTER);
	ip_box->setFontName("/fonts/Marker Felt.ttf");
	ip_box->setFontSize(28);
	ip_box->setMaxLength(20);
	ip_box->setFontColor(Color3B::WHITE);
	ip_box->setText("127.0.0.1");
	ip_box->setTag(1);
	this->addChild(ip_box,2);

	/* IpLabel */
	auto ip_label = Label::createWithTTF("IP:", "fonts/Marker Felt.ttf", 32);
	ip_label->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height * 0.7));
	this->addChild(ip_label,2);

	/* StartGameMenu */
	auto start_game_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ClientMenu::menuStartGameCallback, this));
	start_game_menu->setScale(2);
	auto start_game_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 32);
	start_game_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.5);
	this->addChild(start_game_label,2);

	/* BackMenu */
	auto back_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ClientMenu::menuBackCallback, this));
	back_menu->setScale(2);
	auto back_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	back_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.3);
	this->addChild(back_label,2);

	/* AddTwoMenu */
	auto menu = Menu::create(start_game_menu, back_menu, NULL, NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.4));
	menu->alignItemsVerticallyWithPadding(35);
	this->addChild(menu, 1);

	return true;
}

void ClientMenu::menuStartGameCallback(cocos2d::Ref * pSender) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	log("client ok");
	client_side = chat_client::create();
	schedule(schedule_selector(ClientMenu::startSchedule), 0.1);
}

void ClientMenu::menuBackCallback(cocos2d::Ref * pSender) {
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}

void ClientMenu::startSchedule(float f)
{
	/*
	static int timer = 0;
	static cocos2d::Label* connection_msg_;
	if (client_side->error())
	{
	unscheduleAllCallbacks();
	client_side->close();
	delete client_side;
	client_side = nullptr;
	connection_msg_->setString("Cannot connect to the server, please try again");
	return;
	}

	switch ((timer++ % 32) / 4)
	{
	case 0: connection_msg_->setString("Connected, wait for server"); break;
	case 1: connection_msg_->setString("Connected, wait for server."); break;
	case 2: connection_msg_->setString("Connected, wait for server.."); break;
	case 3: connection_msg_->setString("Connected, wait for server..."); break;
	default: break;
	}
	*/
	if (client_side->started())
	{
		wait_start();
	}
}

void ClientMenu::wait_start()
{
	//socket_client_->camp();
	unscheduleAllCallbacks();
	log("get the camp");
	log("start game");
	auto scene = CombatScene::createScene(nullptr, client_side);
	//	auto scene = BattleScene::createScene(socket_client_);
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}
