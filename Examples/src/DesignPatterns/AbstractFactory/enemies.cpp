/*
 * enemies.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "enemies.hpp"
#include <iostream>

void
Soldier::speak() const
{
  std::cout << "A soldier: ready for battle!" << std::endl;
}

void
Monster::speak() const
{
  std::cout << "A monster: grrr!" << std::endl;
}

void
SuperMonster::speak() const
{
  std::cout << "A super monster: I'm (almost) invincible!" << std::endl;
}

std::unique_ptr<Enemy>
NormalEnemyFactory::operator()(std::string_view Identifier) const
{
  if(Identifier == "Soldier")
    return std::make_unique<Soldier>();
  else if(Identifier == "Monster")
    return std::make_unique<Monster>();
  else if(Identifier == "SuperMonster")
    return std::make_unique<SuperMonster>();
  return nullptr;
}