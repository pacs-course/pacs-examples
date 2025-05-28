/*
 * enemies.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef ENEMIESBASE_HPP_
#define ENEMIESBASE_HPP_
class Enemy
{
public:
  virtual void speak() const = 0;
  virtual ~Enemy() = default; // Ensure proper cleanup of derived classes
};

#endif /* ENEMIESBASE_HPP_ */
