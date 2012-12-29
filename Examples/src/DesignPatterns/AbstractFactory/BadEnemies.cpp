/*
 * BadEnemies.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "BadEnemies.hpp"
#include <iostream>

void BadSuperMonster::speak()
{
	std::cout<<" I am a Bad SuperMonster and I'll kill you"<<std::endl;
}



void BadMonster::speak()
{
	std::cout<<" I am a Bad Monster and I'll kill you"<<std::endl;
}



void BadSoldier::speak()
{
	std::cout<<" I am a Bad Soldier and I'll kill you"<<std::endl;
}

Enemy *AdvancedLevelEnemyFactory::MakeMonster()
{
	return new BadMonster;
}



Enemy *AdvancedLevelEnemyFactory::MakeSuperMonster()
{
	return new BadSuperMonster;
}



Enemy *AdvancedLevelEnemyFactory::MakeSoldier()
{
	return new BadSoldier;
}

std::unique_ptr<AbstractEnemyFactory> BuildAdvancedLevelEnemyFactory(){
  return std::unique_ptr<AbstractEnemyFactory>(new AdvancedLevelEnemyFactory);
}


