/*
 * main_AbstractFactory.cpp
 *
 *  Created on: 02/gen/2011
 *      Author: formaggia
 */
#include <iostream>
#include <vector>
#include <vector>
#include "SillyEnemies.hpp"
#include "BadEnemies.hpp"
#include <memory>
typedef std::vector<BuildEnemyFactory> LevelFactory;
int main(){
  // Initialize the object factory to choose the level
  LevelFactory myLevelFactory;
  // Level 0
  myLevelFactory.push_back(&BuildEasyLevelEnemyFactory);
  // Level 1
  myLevelFactory.push_back(&BuildAdvancedLevelEnemyFactory);
  //
  std::vector<Enemy* > theEnemies;
  int level;
  std::cout<<"Give me level: 0 Beginner, 1 Advanced"<<std::endl;
  std::cin>>level;
  std::auto_ptr<AbstractEnemyFactory> enemyFactory( myLevelFactory[level]());
  //
  theEnemies.push_back(enemyFactory->MakeMonster());
  theEnemies.push_back(enemyFactory->MakeSuperMonster());
  theEnemies.push_back(enemyFactory->MakeSoldier());

  for (unsigned int i=0; i<theEnemies.size();++i)
    theEnemies.at(i)->speak();

// Clean memory not necessary in this case but advisable:
// for any new you need a delete!

  for (unsigned int i=0; i<theEnemies.size();++i) 
    delete theEnemies[i];
}
