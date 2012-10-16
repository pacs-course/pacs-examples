/*
 * BadEnemies.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef BADENEMIES_HPP_
#define BADENEMIES_HPP_
#include "enemies.hpp"
#include "AbstractEnemyFactory.hpp"
class BadSoldier: public Soldier
{
public:
	void speak();
};
class BadMonster: public Monster
{
public:
	void speak();
};
class BadSuperMonster: public SuperMonster
{
public:
	void speak();
};

class AdvancedLevelEnemyFactory : public AbstractEnemyFactory
{
public:
	 Enemy * MakeSoldier();
	 Enemy * MakeMonster();
	 Enemy * MakeSuperMonster();
};

std::auto_ptr<AbstractEnemyFactory> BuildAdvancedLevelEnemyFactory();

#endif /* BADENEMIES_HPP_ */
