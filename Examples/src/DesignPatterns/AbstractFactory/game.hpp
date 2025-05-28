#ifndef __GAME_HPP__
#define __GAME_HPP__
#include "AbstractEnemyFactory.hpp"
#include "enemies.hpp"
#include "SillyEnemies.hpp"
#include "BadEnemies.hpp"
struct GameFactory
{

	std::unique_ptr<AbstractEnemyFactory> createEnemyFactory(int level) const
	{
		std::string_view type;
		if (level ==0)
		{
			return std::make_unique<EasyLevelEnemyFactory>();
		}
		else if (level == 1)
		{
			return std::make_unique<NormalEnemyFactory>();
		}
		else if (level ==2 )
		{
			return std::make_unique <AdvancedLevelEnemyFactory>();
		}
		else
		{
			throw std::invalid_argument("Unknown enemy factory type");
		}
	}
};
#endif // __GAME_HPP__