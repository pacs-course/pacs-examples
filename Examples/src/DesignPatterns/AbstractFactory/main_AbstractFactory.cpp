/*
 * main_AbstractFactory.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "game.hpp"
#include <iostream>
#include <vector>
int
main()
{
  //
  std::vector<std::unique_ptr<Enemy> > theEnemies;
  int              level;
  GameFactory theGame;
  std::cout << "Give me level: 0 Beginner, 1 Normal, 2 Advanced" << std::endl;
  std::cin >> level;
  std::unique_ptr<AbstractEnemyFactory> enemyFactoryptr =
	  theGame.createEnemyFactory(level);
  auto const & enemyFactory = *enemyFactoryptr;
  

  theEnemies.emplace_back(enemyFactory("Monster"));
  theEnemies.emplace_back(enemyFactory("Soldier"));
  theEnemies.emplace_back(enemyFactory("SuperMonster"));

  for(unsigned int i = 0; i < theEnemies.size(); ++i)
    theEnemies.at(i)->speak();
}
