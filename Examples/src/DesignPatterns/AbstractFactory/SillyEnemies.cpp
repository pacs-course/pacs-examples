/*
 * SillyEnemies.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "SillyEnemies.hpp"
#include <iostream>

void
SillySuperMonster::speak() const
{
  std::cout
    << " I am a Silly SuperMonster and maybe I kill you (but I am not so sure)"
    << std::endl;
}

void
SillyMonster::speak() const
{
  std::cout << " I am a Silly Monster and I'll kill you (or at least I try to "
               "do my best)"
            << std::endl;
}

void
SillySoldier::speak() const
{
  std::cout << " I am a Silly Soldier: hit me and I die" << std::endl;
}

/*!
 * \brief Creates a silly enemy based on the identifier.
 * @param Identifier The type of enemy to create (e.g., "Soldier", "Monster",
 * "SuperMonster").
 * @return A unique pointer to the created Enemy object.
 */
std::unique_ptr<Enemy>
EasyLevelEnemyFactory::operator()(std::string_view Identifier) const	
{
  if(Identifier == "Soldier")
    return std::make_unique<SillySoldier>();
  else if(Identifier == "Monster")
    return std::make_unique<SillyMonster>();
  else if(Identifier == "SuperMonster")
    return std::make_unique<SillySuperMonster>();
  return nullptr;
}
