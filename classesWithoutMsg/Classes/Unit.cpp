#include"cocos2d.h"
#include"Unit.h"
#include<string>
#include<random>
//#define DEBUG

USING_NS_CC;
/*ÑªÌõ¸üÐÂ*/
void Bar::updateBarDisplay(float rate) {
#ifdef DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif // DEBUG
	
	setVisible(true);
	clear();
	auto tg = static_cast<Unit*>(this->getParent());
#ifdef DEBUG
	log("bar father:address %d", this->getParent());
#endif // DEBUG

	auto s = tg->getContentSize();
	
	drawRect(Point((s.width - length) / 2, s.height + 5), Point((s.width + length) / 2, s.height + 5 + width), color);
	Point endpoint{ s.width / 2 - length / 2 + length * rate,s.height + 5 + width };
	drawSolidRect(Point((s.width - length) / 2, s.height + 5), endpoint, color);
}

void Bar::stopKeepingVisible() {
	setVisible(false);
	is_visible = false;
}
void Bar::keepVisible() {
	setVisible(true);
	is_visible = true;
}

void Unit::set(TMXTiledMap * AM, Layer * AL, EventListenerTouchOneByOne * ALis)
{
	addMap = AM;
	addLayer = AL;
	addTouchListener = ALis;
}

void Unit::set_movable(bool can)
{
	can_move = can;
}

bool Unit::is_movable()
{
	return can_move;
}

bool Unit::is_in(Point p1, Point p2) {
	Point unitPoint = this->getPosition();
	if (abs(unitPoint.x - p1.x) + abs(unitPoint.x - p2.x) != abs(p1.x - p2.x)) {
		return false;
	}
	else if(abs(unitPoint.y - p1.y) + abs(unitPoint.y - p2.y) != abs(p1.y - p2.y)) {
		return false;
	}
	return true;
}
void Unit::displayHP() {
	if (hp_bar) {
		hp_bar->keepVisible();
	}
}
void Unit::hideHP() {
	if (hp_bar) {
		hp_bar->stopKeepingVisible();
	}
}