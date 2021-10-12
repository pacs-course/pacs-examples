/*
 * main_composite.cpp
 *
 *  Created on: 04/gen/2011
 *      Author: formaggia
 */
#include "Composite.hpp"
#include <iostream>
int
main()
{
  Composite car("Car");
  Composite chassis("Chassis");
  Element   weel1("weel1");
  Element   weel2("weel2");
  Element   weel3("weel3");
  Element   weel4("weel4");
  Element   sweel("Steering weel");
  chassis.add(weel1);
  chassis.add(weel2);
  chassis.add(weel3);
  chassis.add(weel4);
  chassis.add(sweel);
  Composite engine("Engine");
  Element   splugs("spark plugs");
  Element   pistons("Pistons");
  Element   gbox("Gear Box");
  engine.add(splugs);
  engine.add(pistons);
  engine.add(gbox);
  car.add(engine);
  car.add(chassis);
  car.operation();
  std::cout << std::endl << "END OF ALL COMPOSITES" << std::endl;
}
