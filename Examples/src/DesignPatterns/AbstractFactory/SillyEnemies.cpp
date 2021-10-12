/*
 * SillyEnemies.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "SillyEnemies.hpp"
#include <iostream>

void
SillySuperMonster::speak()
{
  std::cout
    << " I am a Silly SuperMonster and I'll kill you (or at least I try)"
    << std::endl;
}

void
SillyMonster::speak()
{
  std::cout << " I am a Silly Monster and I'll kill you (or at least I try)"
            << std::endl;
}

void
SillySoldier::speak()
{
  std::cout << " I am a Silly Soldier and I'll kill you (or at least I try)"
            << std::endl;
}

std::unique_ptr<Enemy>
EasyLevelEnemyFactory::MakeSuperMonster()
{
  return std::make_unique<SillySuperMonster>();
}

std::unique_ptr<Enemy>
EasyLevelEnemyFactory::MakeMonster()
{
  return std::make_unique<SillyMonster>();
}

std::unique_ptr<Enemy>
EasyLevelEnemyFactory::MakeSoldier()
{
  return std::make_unique<SillySoldier>();
}

std::unique_ptr<AbstractEnemyFactory>
BuildEasyLevelEnemyFactory()
{
  return std::unique_ptr<AbstractEnemyFactory>(new EasyLevelEnemyFactory);
}
