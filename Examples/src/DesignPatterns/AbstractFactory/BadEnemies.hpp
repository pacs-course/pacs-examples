/*
 * BadEnemies.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef BADENEMIES_HPP_
#define BADENEMIES_HPP_
#include "AbstractEnemyFactory.hpp"
#include "enemies.hpp"
class BadSoldier : public Soldier
{
public:
  void speak() override;
  virtual ~BadSoldier() = default;
};
class BadMonster : public Monster
{
public:
  void speak() override;
  virtual ~BadMonster() = default;
};
class BadSuperMonster : public SuperMonster
{
public:
  void speak() override;
  virtual ~BadSuperMonster() = default;
};

class AdvancedLevelEnemyFactory : public AbstractEnemyFactory
{
public:
  std::unique_ptr<Enemy> MakeSoldier() override;
  std::unique_ptr<Enemy> MakeMonster() override;
  std::unique_ptr<Enemy> MakeSuperMonster() override;
  virtual ~AdvancedLevelEnemyFactory() = default;
};

std::unique_ptr<AbstractEnemyFactory> BuildAdvancedLevelEnemyFactory();

#endif /* BADENEMIES_HPP_ */
