/*
 * AbstractEnemyFactory.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "AbstractEnemyFactory.hpp"

std::unique_ptr<Enemy>
AbstractEnemyFactory::MakeSoldier()
{
  return std::unique_ptr<Enemy>();
}

std::unique_ptr<Enemy>
AbstractEnemyFactory::MakeMonster()
{
  return std::unique_ptr<Enemy>();
}

std::unique_ptr<Enemy>
AbstractEnemyFactory::MakeSuperMonster()
{
  return std::unique_ptr<Enemy>();
}
