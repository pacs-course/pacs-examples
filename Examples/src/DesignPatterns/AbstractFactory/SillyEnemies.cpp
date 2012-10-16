/*
 * SillyEnemies.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "SillyEnemies.hpp"
#include <iostream>

void SillySuperMonster::speak()
{
	std::cout<<" I am a Silly SuperMonster and I'll kill you (or at least I try)"<<std::endl;
}



void SillyMonster::speak()
{
	std::cout<<" I am a Silly Monster and I'll kill you (or at least I try)"<<std::endl;
}



void SillySoldier::speak()
{
	std::cout<<" I am a Silly Soldier and I'll kill you (or at least I try)"<<std::endl;
}

Enemy *EasyLevelEnemyFactory::MakeSuperMonster()
{
	return new SillySuperMonster;
}



Enemy *EasyLevelEnemyFactory::MakeMonster()
{
	return new SillyMonster;
}



Enemy *EasyLevelEnemyFactory::MakeSoldier()
{
	return new SillySoldier;
}

std::auto_ptr<AbstractEnemyFactory> BuildEasyLevelEnemyFactory(){
  return std::auto_ptr<AbstractEnemyFactory>(new EasyLevelEnemyFactory);
}


