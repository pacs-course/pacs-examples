/*
 * SillyEnemies.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef SILLYENEMIES_HPP_
#define SILLYENEMIES_HPP_
#include "AbstractEnemyFactory.hpp"
#include "enemies.hpp"
class SillySoldier : public Soldier
{
public:
  void speak() override;
  virtual ~SillySoldier() = default;
};
class SillyMonster : public Monster
{
public:
  void speak() override;
  virtual ~SillyMonster() = default;
};
class SillySuperMonster : public SuperMonster
{
public:
  void speak() override;
  virtual ~SillySuperMonster() = default;
};

class EasyLevelEnemyFactory : public AbstractEnemyFactory
{
public:
  std::unique_ptr<Enemy> MakeSoldier() override;
  std::unique_ptr<Enemy> MakeMonster() override;
  std::unique_ptr<Enemy> MakeSuperMonster() override;
  virtual ~EasyLevelEnemyFactory() = default;
};

std::unique_ptr<AbstractEnemyFactory> BuildEasyLevelEnemyFactory();
#endif /* SILLYENEMIES_HPP_ */
