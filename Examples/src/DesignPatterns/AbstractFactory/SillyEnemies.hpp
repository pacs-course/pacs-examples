/*
 * SillyEnemies.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef SILLYENEMIES_HPP_
#define SILLYENEMIES_HPP_
#include "enemies.hpp"
#include "AbstractEnemyFactory.hpp"
class SillySoldier: public Soldier
{
public:
	void speak();
};
class SillyMonster: public Monster
{
public:
	void speak();
};
class SillySuperMonster: public SuperMonster
{
public:
	void speak();
};

class EasyLevelEnemyFactory : public AbstractEnemyFactory
{
public:
	 Enemy * MakeSoldier();
	 Enemy * MakeMonster();
	 Enemy * MakeSuperMonster();
};

std::unique_ptr<AbstractEnemyFactory> BuildEasyLevelEnemyFactory();
#endif /* SILLYENEMIES_HPP_ */
