/*
 * enemies.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef ENEMIES_HPP_
#define ENEMIES_HPP_
class Enemy{
public:
  virtual void speak()=0;
  virtual ~Enemy(){};
};

class Soldier: public Enemy
{
public:
  virtual void speak();
};

class Monster: public Enemy
{
public:
  virtual void speak();
};

class SuperMonster: public Enemy
{
public:
  virtual void speak();
};

#endif /* ENEMIES_HPP_ */
