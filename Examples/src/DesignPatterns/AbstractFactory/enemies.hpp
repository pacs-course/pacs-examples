/*
 * enemies.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef ENEMIES_HPP_
#define ENEMIES_HPP_
#include "AbstractEnemyFactory.hpp"
#include "enemiesBase.hpp"

class Soldier : public Enemy
{
public:
  void speak() const override;
};

class Monster : public Enemy
{
public:
  void speak() const override;
};

class SuperMonster : public Enemy
{
public:
  void speak() const override;
};

class NormalEnemyFactory : public AbstractEnemyFactory
{
public:
  /*! \brief Creates an enemy based on the identifier.
   * @param Identifier The type of enemy to create (e.g., "Soldier", "Monster",
   * "SuperMonster").
   * @return A unique pointer to the created Enemy object.
   */
  std::unique_ptr<Enemy> operator()(std::string_view Identifier) const override;
};
#endif /* ENEMIES_HPP_ */
