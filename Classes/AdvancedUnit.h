#pragma once
#ifndef __ADVANCEDUNIT_H__
#define __ADVANCEDUNIT_H__


#include "Unit.h"

class Dog : public Unit
{
public:
	static Dog* create(const std::string& filename);
private:
	void setProperties() override;
};
class Fighter : public Unit
{
public:
	static Fighter* create(const std::string& filename);

	GridPath findPath(const GridPoint& dest)const override;

private:

	void setProperties() override;

	void move() override;
};


class Tank : public Unit
{
public:
	static Tank* create(const std::string& filename);
private:

	void setProperties() override;
};

class Soldier : public Unit
{
public:
	static Soldier* create(const std::string& filename);
private:

	void setProperties() override;
};

#endif // !__ADVANCEDUNIT_H__