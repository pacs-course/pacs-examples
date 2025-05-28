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
class SillySoldier final : public Soldier
{
public:
  void speak() const override;
};
class SillyMonster final : public Monster
{
public:
  void speak() const override;
};
class SillySuperMonster final : public SuperMonster
{
public:
  void speak() const override;
};

class EasyLevelEnemyFactory : public AbstractEnemyFactory
{
public:
  std::unique_ptr<Enemy> operator()(std::string_view Identifier) const override;
};
#endif /* SILLYENEMIES_HPP_ */
