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

	schedule(schedule_selector(CombatScene::update));

	/* 加载鼠标事件 */
	auto mouse_event = EventListenerMouse::create();
	mouse_event->onMouseMove = [&](Event *event) {
		EventMouse* pem = static_cast<EventMouse*>(event);
		_cursor_position = Vec2(pem->getCursorX(), pem->getCursorY());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mouse_event, 1);

	return true;
}

void CombatScene::update(float f){
	scrollMap();
}

void CombatScene::scrollMap(){
	auto map_center = _combat_map->getPosition();
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

void MouseRect::update(float f){
	clear();
	Node* parent = getParent();
	_end = _touch_end - parent->getPosition();
	drawRect(_start, _end, Color4F(0, 1, 0, 1));
}
