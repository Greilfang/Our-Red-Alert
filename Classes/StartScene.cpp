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
	this->addChild(server_label, 2);

	/* ClientMenu */
	auto client_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(StartScene::menuClientCallback, this));
	client_menu->setScale(2);
	auto client_label = Label::createWithTTF("Join Game", "fonts/Marker Felt.ttf", 32);
	client_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.5);
	this->addChild(client_label, 2);

	/* ReturnMenu */
	auto back_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(StartScene::menuBackCallback, this));
	back_menu->setScale(2);
	auto back_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	back_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.3);
	this->addChild(back_label, 2);

	/* AddThreeMenu */
	auto menu = Menu::create(server_menu, client_menu, back_menu, NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height / 2));
	menu->alignItemsVerticallyWithPadding(35);
	this->addChild(menu, 1);

	return true;
}

void StartScene::menuServerCallback(cocos2d::Ref* pSender) {
	auto scene = MapChoiceScene::createScene();
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

cocos2d::Scene * ServerMenu::createScene(int * map_choice) {
	auto scene = Scene::create();
	auto layer = ServerMenu::create(map_choice);

	scene->addChild(layer);
	return scene;
}

ServerMenu * ServerMenu::create(int * map_choice)
{
	ServerMenu *server_menu_scene = new (std::nothrow) ServerMenu();
	if (server_menu_scene && server_menu_scene->init(map_choice))
	{
		server_menu_scene->autorelease();
		return server_menu_scene;
	}
	CC_SAFE_DELETE(server_menu_scene);

	return nullptr;
}

bool ServerMenu::init(int * map_choice) {
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
	this->addChild(start_server_label, 2);

	/* StartGameMenu */
	auto start_game_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ServerMenu::menuStartGameCallback, this));
	start_game_menu->setScale(2);
	auto start_game_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 32);
	start_game_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.5);
	this->addChild(start_game_label, 2);

	/* SetMap */
	_map_choice = *map_choice == 1 ? 1 : 2;

	/* BackMenu */
	auto back_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ServerMenu::menuBackCallback, this));
	back_menu->setScale(2);
	auto back_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	back_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.3);
	this->addChild(back_label, 2);

	/* AddThreeMenu */
	auto menu = Menu::create(start_server_menu, start_game_menu, back_menu, NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height / 2));
	menu->alignItemsVerticallyWithPadding(35);
	this->addChild(menu, 1);

	//auto map_menu = MenuItemImage::create("minimap1.png", "minimap1.png",
	//	CC_CALLBACK_1(ServerMenu::menuSelectMap1Callback, this));
	//map_menu->setScale(1);

	//auto map2_menu = MenuItemImage::create("minimap2.png", "minimap2.png",
	//	CC_CALLBACK_1(ServerMenu::menuSelectMap2Callback, this));
	//map2_menu->setScale(1);

	//auto menu2 = Menu::create(map_menu, map2_menu, NULL);
	//menu2->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height / 2));
	//menu2->alignItemsVerticallyWithPadding(35);
	//this->addChild(menu2, 1);

	/* ConnectionLabel */
	connection_label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	connection_label->setAnchorPoint(Vec2(0.5, 0));
	connection_label->setPosition(Vec2(origin.x + visibleSize.width*0.7, origin.y + visibleSize.height*0.8));
	this->addChild(connection_label, 3);

	return true;
}

//void ServerMenu::menuSelectMap1Callback(cocos2d::Ref * pSender) {
//	server_side->map = 1;
//	std::cout << "set map wei 1";
//
//}
//void ServerMenu::menuSelectMap2Callback(cocos2d::Ref * pSender) {
//	server_side->map = 2;
//	std::cout << "set map wei 2";
//
//}

void ServerMenu::menuStartServerCallback(cocos2d::Ref * pSender) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	server_side = chat_server::create();
	client_side = chat_client::create();

	server_side->map = _map_choice;
	schedule(schedule_selector(ServerMenu::connectionUpdate), 0.1);
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
	if (server_side->connection_num())
		connection_label->setString("Total connection num: " + std::to_string(server_side->connection_num()));
	else
		connection_label->setString("Port already used, please change another one");
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
	auto ip_box = EditBox::create(Size(300, 60), Scale9Sprite::create("button.png"));
	ip_box->setPosition(Vec2(origin.x + visibleSize.width * 0.35, origin.y + visibleSize.height * 0.7));
	ip_box->setTextHorizontalAlignment(TextHAlignment::CENTER);
	ip_box->setFontName("/fonts/Marker Felt.ttf");
	ip_box->setFontSize(28);
	ip_box->setMaxLength(20);
	ip_box->setFontColor(Color3B::WHITE);
	ip_box->setText("127.0.0.1");
	ip_box->setTag(1);
	this->addChild(ip_box, 2);

	/* IpLabel */
	auto ip_label = Label::createWithTTF("IP:", "fonts/Marker Felt.ttf", 32);
	ip_label->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height * 0.7));
	this->addChild(ip_label, 2);

	/* StartGameMenu */
	auto start_game_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ClientMenu::menuStartGameCallback, this));
	start_game_menu->setScale(2);
	auto start_game_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 32);
	start_game_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.5);
	this->addChild(start_game_label, 2);

	/* BackMenu */
	auto back_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(ClientMenu::menuBackCallback, this));
	back_menu->setScale(2);
	auto back_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	back_label->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.3);
	this->addChild(back_label, 2);

	/* AddTwoMenu */
	auto menu = Menu::create(start_game_menu, back_menu, NULL, NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.4));
	menu->alignItemsVerticallyWithPadding(35);
	this->addChild(menu, 1);

	return true;
}

void ClientMenu::menuStartGameCallback(cocos2d::Ref * pSender) {
	if (!client_side)
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		log("client ok");
		auto ip_box = static_cast<ui::EditBox*>(getChildByTag(1));
		std::string ip = ip_box->getText();

		client_side = chat_client::create(ip, 1024);

		schedule(schedule_selector(ClientMenu::startSchedule), 0.1);
	}
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

cocos2d::Scene * MapChoiceScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MapChoiceScene::create();

	scene->addChild(layer);
	return scene;
}

bool MapChoiceScene::init()
{
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* BackGround */
	auto back_ground = Sprite::create("background5.png");
	back_ground->setPosition(origin + visibleSize / 2);
	back_ground->setScaleX(visibleSize.width / back_ground->getContentSize().width);
	back_ground->setScaleY(visibleSize.height / back_ground->getContentSize().height);
	addChild(back_ground, 0);

	/* Map1 Select */
	MenuItemImage *rememberName1 = MenuItemImage::create("minimap1_unselected.png", "minimap1_unselected.png");
	MenuItemImage *rememberName2 = MenuItemImage::create("minimap1.png", "minimap1.png");
	MenuItemToggle *rememberNameItem = MenuItemToggle::createWithTarget(this, menu_selector(MapChoiceScene::menuSelectMap1Callback), rememberName1, rememberName2, NULL);
	rememberNameItem->setPosition(Vec2(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height * 0.6));
	auto menu1 = Menu::create(rememberNameItem, NULL);
	menu1->setPosition(Vec2(0, 0));
	this->addChild(menu1, 1);

	/* Map2 Select */
	MenuItemImage *rememberName3 = MenuItemImage::create("minimap2_unselected.png", "minimap2_unselected.png");
	MenuItemImage *rememberName4 = MenuItemImage::create("minimap2.png", "minimap2.png");
	MenuItemToggle *rememberNameItem2 = MenuItemToggle::createWithTarget(this, menu_selector(MapChoiceScene::menuSelectMap2Callback), rememberName3, rememberName4, NULL);
	rememberNameItem2->setPosition(Vec2(origin.x + visibleSize.width * 0.25, origin.y + visibleSize.height * 0.6));
	auto menu2 = Menu::create(rememberNameItem2, NULL);
	menu2->setPosition(Vec2(0, 0));
	this->addChild(menu2, 1);

	///* MapChoiceMenu */
	//auto map1_menu = MenuItemImage::create("minimap1_unselected.png", "minimap1.png",
	//	CC_CALLBACK_1(MapChoiceScene::menuSelectMap1Callback, this));

	//auto map2_menu = MenuItemImage::create("minimap2_unselected.png", "minimap2.png",
	//	CC_CALLBACK_1(MapChoiceScene::menuSelectMap2Callback, this));

	//auto menu = Menu::create(map2_menu, map1_menu, NULL);
	//menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.6));
	//menu->alignItemsHorizontallyWithPadding(visibleSize.width * 0.3);
	//this->addChild(menu, 1);

	/* MapNameLabel */
	auto map2_label = Label::createWithTTF("Lost City", "fonts/Marker Felt.ttf", 32);
	map2_label->setPosition(Vec2(origin.x + visibleSize.width * 0.25, origin.y + visibleSize.height * 0.3));
	this->addChild(map2_label);

	auto map1_label = Label::createWithTTF("Spring of Siberia ", "fonts/Marker Felt.ttf", 32);
	map1_label->setPosition(Vec2(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height * 0.3));
	this->addChild(map1_label);

	/* NextMenu */
	auto next_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(MapChoiceScene::menuNextCallback, this));
	next_menu->setScale(2);
	auto next_label = Label::createWithTTF("Next", "fonts/Marker Felt.ttf", 32);
	next_label->setPosition(origin.x + visibleSize.width * 0.38, origin.y + visibleSize.height * 0.2);
	this->addChild(next_label, 2);

	/* BackMenu */
	auto back_menu = MenuItemImage::create("button.png", "button.png",
		CC_CALLBACK_1(MapChoiceScene::menuBackCallback, this));
	back_menu->setScale(2);
	auto back_label = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	back_label->setPosition(origin.x + visibleSize.width * 0.62, origin.y + visibleSize.height * 0.2);
	this->addChild(back_label, 2);

	/* AddTwoMenu */
	auto menu3 = Menu::create(next_menu, back_menu, NULL);
	menu3->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.2));
	menu3->alignItemsHorizontallyWithPadding(visibleSize.width / 32);
	this->addChild(menu3, 1);
}

void MapChoiceScene::menuSelectMap1Callback(cocos2d::Ref * pSender)
{
	_map_choice = 1;
}

void MapChoiceScene::menuSelectMap2Callback(cocos2d::Ref * pSender)
{
	_map_choice = 2;
}

void MapChoiceScene::menuNextCallback(cocos2d::Ref * pSender)
{
	auto scene = ServerMenu::createScene(&_map_choice);
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}

void MapChoiceScene::menuBackCallback(cocos2d::Ref * pSender)
{
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}
