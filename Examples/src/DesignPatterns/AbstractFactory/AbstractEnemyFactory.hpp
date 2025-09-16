/*
 * AbstractEnemyFactory.hpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */

#ifndef ABSTRACTENEMYFACTORY_HPP_
#define ABSTRACTENEMYFACTORY_HPP_
#include <string>
#include <memory>
#include <functional>
/*!
 * \brief Abstract factory for creating enemies.
* This abstract factory defines the interface for creating different types of enemies.
 * It allows for the creation of soldiers, monsters, and super monsters without specifying the concrete classes.
 * @note It is a modified version of the original Abstract Factory pattern.
 * @see https://en.wikipedia.org/wiki/Abstract_factory_pattern
 */
class Enemy;
 class AbstractEnemyFactory
{
public:
  /*!
   * \brief Creates a soldier enemy.
   * @return A unique pointer to a Soldier object.
   */
  virtual std::unique_ptr<Enemy> operator()(std::string_view Identifier) const = 0;	
  virtual ~AbstractEnemyFactory() = default;		
};
/*!
 * \brief Type alias for a function that builds an enemy.
 * This type alias represents a function that takes an identifier as a string and returns a unique pointer to an Enemy object.
 */
using BuildEnemy = std::function<std::unique_ptr<Enemy>(std::string_view Identifier)>;

#endif /* ABSTRACTENEMYFACTORY_HPP_ */
