#include "AdvancedUnit.h"

USING_NS_CC;

Fighter* Fighter::create(const std::string& filename)
{
	Fighter *ret = new (std::nothrow) Fighter();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
/*
void Fighter::motivate()
{
	moving = true;
	grid_map->leavePosition(cur_pos, true);
}
*/
void Fighter::setProperties()
{
	type = 1;


	speed = 100.0f;

	mobile = true;

	z_index = 15;

}
/*
void Fighter::move()
{
	auto esp = (grid_map->getPointWithOffset(cur_dest) - getPosition()).getNormalized();
	Point next_pos = getPosition() + esp * move_speed;
	GridPoint next_gp = grid_map->getGridPoint(next_pos);

	if (cur_pos == next_gp)
	{
		setPosition(next_pos);
	}
	else
	{
		grid_map->occupyPosition(id, next_gp, false);
		roc_cnt = 0;
		setPosition(next_pos);
		grid_map->leavePosition(cur_pos, false);
		cur_pos = next_gp;
		if (camp == unit_manager->player_id)
			grid_map->clearFog(GridRect(cur_pos, vision_range, true));
	}

	if (hasArrivedAtDest())
		if (grid_path.size())
		{
			cur_dest = grid_path.back();
			grid_path.pop_back();
		}
		else
		{
			if (grid_map->checkPosition(cur_dest))
			{
				log("Unit ID: %d, fighter landing", id);
				grid_map->occupyPosition(id, next_gp, true);
				moving = false;
			}
			else
			{
				log("Unit ID: %d, fighter failed to land", id);
				cur_dest = grid_map->findFreePositionNear(cur_dest);
				final_dest = cur_dest;
			}
		}
}

GridPath Fighter::findPath(const GridPoint & dest) const
{
	return GridPath{ dest };
}
*/
Tank* Tank::create(const std::string& filename)
{
	Tank *ret = new (std::nothrow) Tank();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}
/*
void Tank::attack()
{
	if (!cd)
	{
		const auto& splash_center = unit_manager->getUnitPosition(target_id);
		const auto& splash_rect = GridRect(splash_center - splash_range / 2, splash_range);
		const auto& splash_ids = grid_map->getUnitIDAt(splash_rect);
		for (const auto& its_id : splash_ids)
		{
			log("Tank %d attack! Unit: %d been splashed", id, its_id);
			int its_camp = unit_manager->getUnitCamp(its_id);
			if (its_camp != 0 && its_camp != camp)
				unit_manager->msgs->add_game_message()->genGameMessage(GameMessage::CmdCode::GameMessage_CmdCode_ATK, id, its_id, atk, camp, 0, {});
		}

		cd = cd_max;
	}
	else
		cd--;
}
*/
void Tank::setProperties()
{
	type = 2;

	speed = 200.0f;


	z_index = 10;

	mobile = true;
}


Soldier* Soldier::create(const std::string& filename)
{
	Soldier *ret = new (std::nothrow) Soldier();
	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}

void Soldier::setProperties()
{
	type = 3;

	speed = 150.0f;

	z_index = 10;

	mobile = true;
}