#include "CombatScene.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;
#define BOX_EDGE_WITDH_SMALL 10
#define BOX_EDGE_WITDH 40
#define SCROLL_LENGTH 10


Scene * CombatScene::createScene() {
	auto scene = Scene::create();
	auto layer = CombatScene::create();

	scene->addChild(layer);
	return scene;
}

bool CombatScene::init() {
	if (!Layer::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();



	/* 加载地图 */
	_combat_map = TMXTiledMap::create("map/BasicMap1.tmx");
	_combat_map->setAnchorPoint(Vec2(0, 0));
	addChild(_combat_map, 0);



	/* 加载鼠标事件 */
	auto mouse_event = EventListenerMouse::create();
	mouse_event->onMouseMove = [&](Event *event) {
		EventMouse* pem = static_cast<EventMouse*>(event);
		_cursor_position = Vec2(pem->getCursorX(), pem->getCursorY());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mouse_event, 1);

	/*生产基地按钮*/
	auto base_button = Sprite::create("basebutton.jpg");
	base_button->setPosition(Vec2(origin.x + visibleSize.width - 60,
		origin.y + visibleSize.height / 2));
	addChild(base_button, 10);

	/*生产基地按钮单点触摸监听器*/
	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->setSwallowTouches(true);
	listener1->onTouchBegan = [](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			return true;
		}
		return false;
	};

	msgs = new GameMessageSet;

	listener1->onTouchEnded = [=](Touch* touch, Event* event) {

		Vec2 touchLocation = touch->getLocation();
		auto new_msg = msgs->add_game_message();
		new_msg->set_cmd_code(GameMessage::CmdCode::GameMessage_CmdCode_CRT);
		//此处有问题，无法自动释放，但还不能随便删
		GridPath *gridpath = new GridPath;
		auto newgridpoint = gridpath->add_grid_point();
		newgridpoint->set_x(touchLocation.x);
		newgridpoint->set_y(touchLocation.y);
		new_msg->set_allocated_grid_path(gridpath);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, base_button);


	schedule(schedule_selector(CombatScene::update));
	return true;
}

void CombatScene::update(float f) {
	updateUnitsState();
	scrollMap();

}

void CombatScene::scrollMap() {
	map_center = _combat_map->getPosition();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int horizontal_state, vertical_state;
	horizontal_state = (origin.x + visibleSize.width - BOX_EDGE_WITDH_SMALL < _cursor_position.x)
		+ (origin.x + visibleSize.width - BOX_EDGE_WITDH < _cursor_position.x)
		- (origin.x + BOX_EDGE_WITDH_SMALL > _cursor_position.x)
		- (origin.x + BOX_EDGE_WITDH > _cursor_position.x);

	vertical_state = (origin.y + visibleSize.height - BOX_EDGE_WITDH_SMALL < _cursor_position.y)
		+ (origin.y + visibleSize.height - BOX_EDGE_WITDH < _cursor_position.y)
		- (origin.y + BOX_EDGE_WITDH_SMALL > _cursor_position.y)
		- (origin.y + BOX_EDGE_WITDH > _cursor_position.y);

	Vec2 scroll(0, 0);
	scroll += Vec2(-SCROLL_LENGTH, 0)*horizontal_state;
	scroll += Vec2(0, -SCROLL_LENGTH)*vertical_state;
	map_center += scroll;
	if (_combat_map->getBoundingBox().containsPoint((-scroll) + Director::getInstance()->getVisibleSize())
		&& _combat_map->getBoundingBox().containsPoint(-scroll))
		_combat_map->setPosition(map_center);
}

void MouseRect::update(float f) {
	clear();
	Node* parent = getParent();
	_end = _touch_end - parent->getPosition();
	drawRect(_start, _end, Color4F(0, 1, 0, 1));
}

void CombatScene::updateUnitsState()
{

	for (int i = 0; i < msgs->game_message_size(); i++)
	{
		const GameMessage&  msg = msgs->game_message(i);

		/*如果是创建新单位消息*/
		if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_CRT)
		{

			_player = Sprite::create("base_0.png");
			_combat_map->addChild(_player, 10);
			_player->setPosition(-map_center.x + msg.grid_path().grid_point(0).x(), -map_center.y + msg.grid_path().grid_point(0).y());

		}
		/*如果是有关移动的消息*/
		else if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_MOV)
		{
			log("Empty Message, there must be something wrong");
		}

	}

	msgs->clear_game_message();
}