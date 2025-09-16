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
class BadSoldier final : public Soldier
{
public:
  void speak() const override;
};
class BadMonster final : public Monster
{
public:
  void speak() const override;
};
class BadSuperMonster final : public SuperMonster
{
public:
  void speak() const override;
};

class AdvancedLevelEnemyFactory : public AbstractEnemyFactory
{
public:
  std::unique_ptr<Enemy> operator()(std::string_view Identifier) const override;
};

#endif /* BADENEMIES_HPP_ */
