/*
 * AbstractEnemyFactory.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef ABSTRACTENEMYFACTORY_HPP_
#define ABSTRACTENEMYFACTORY_HPP_
#include "enemies.hpp"
#include <memory>
class AbstractEnemyFactory
{
public:
  virtual std::unique_ptr<Enemy> MakeSoldier() = 0;
  virtual std::unique_ptr<Enemy> MakeMonster() = 0;
  virtual std::unique_ptr<Enemy> MakeSuperMonster() = 0;
  virtual ~AbstractEnemyFactory() = default;
};

typedef std::unique_ptr<AbstractEnemyFactory> (*BuildEnemyFactory)();
#endif /* ABSTRACTENEMYFACTORY_HPP_ */
