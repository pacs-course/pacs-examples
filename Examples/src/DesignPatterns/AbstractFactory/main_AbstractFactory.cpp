/*
 * main_AbstractFactory.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include "BadEnemies.hpp"
#include "SillyEnemies.hpp"
#include <iostream>
#include <memory>
#include <vector>
typedef std::vector<BuildEnemyFactory> LevelFactory;
int
main()
{
  // Initialize the object factory to choose the level
  LevelFactory myLevelFactory;
  // Level 0
  myLevelFactory.push_back(&BuildEasyLevelEnemyFactory);
  // Level 1
  myLevelFactory.push_back(&BuildAdvancedLevelEnemyFactory);
  //
  std::vector<std::unique_ptr<Enemy> > theEnemies;
  int                                  level;
  std::cout << "Give me level: 0 Beginner, 1 Advanced" << std::endl;
  std::cin >> level;
  std::unique_ptr<AbstractEnemyFactory> enemyFactory(myLevelFactory[level]());
  //
  // I use emplace_back (C++11!!!) that calls directly the
  // constructor: unique_ptr<Enemy>() If I want to use push_back I
  // need to do
  //
  // theEnemies.push_back(std::unique_ptr<Enemy>(enemyFactory->MakeMonster()));
  //
  // etc. In fact push_back calls the default constructor and then the
  // assigmenet operator. But a unique_ptr cannot ne assigned to a
  // pointer.

  theEnemies.emplace_back(enemyFactory->MakeMonster());
  theEnemies.emplace_back(enemyFactory->MakeSuperMonster());
  theEnemies.emplace_back(enemyFactory->MakeSoldier());

  for(unsigned int i = 0; i < theEnemies.size(); ++i)
    theEnemies.at(i)->speak();
}
