/*
 * enemies.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef ENEMIES_HPP_
#define ENEMIES_HPP_

class Enemy
{
public:
  virtual void speak() = 0;
  virtual ~Enemy() = default;
};

class Soldier : public Enemy
{
public:
  virtual void speak() override;
  virtual ~Soldier() = default;
};

class Monster : public Enemy
{
public:
  virtual void speak() override;
  virtual ~Monster() = default;
};

class SuperMonster : public Enemy
{
public:
  virtual void speak() override;
  virtual ~SuperMonster() = default;
};

#endif /* ENEMIES_HPP_ */
