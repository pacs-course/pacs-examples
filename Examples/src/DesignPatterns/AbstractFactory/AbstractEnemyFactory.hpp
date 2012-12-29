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
class AbstractEnemyFactory{
public:
	 virtual Enemy * MakeSoldier()=0;
	 virtual Enemy * MakeMonster();
	 virtual Enemy * MakeSuperMonster();
};

typedef std::unique_ptr<AbstractEnemyFactory> (*BuildEnemyFactory)();
#endif /* ABSTRACTENEMYFACTORY_HPP_ */
