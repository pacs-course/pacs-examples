/*
 * BadEnemies.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "BadEnemies.hpp"
#include <iostream>

void
BadSuperMonster::speak() const
{
  std::cout << " I am a Bad SuperMonster, you must be brave to fight me"
            << std::endl;
}

void
BadMonster::speak() const
{
  std::cout << " I am a Bad Monster, you'd better hide" << std::endl;
}

void
BadSoldier::speak() const
{
  std::cout << " I am a Bad Soldier and I'll kill you unless you are very good"
            << std::endl;
}

std::unique_ptr<Enemy>
AdvancedLevelEnemyFactory::operator()(std::string_view Identifier) const
{
  if(Identifier == "Soldier")
    return std::make_unique<BadSoldier>();
  else if(Identifier == "Monster")
    return std::make_unique<BadMonster>();
  else if(Identifier == "SuperMonster")
    return std::make_unique<BadSuperMonster>();
  return nullptr;
}