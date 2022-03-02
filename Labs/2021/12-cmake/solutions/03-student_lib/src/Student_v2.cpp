#include "Student_v2.hpp"

#include <iostream>

Student::Student(const std::string &name, const unsigned int &age)
  : name(name)
  , age(age)
{}

void
Student::display()
{
  std::cout << "A student with name " << this->name << " and age " << this->age
            << "." << std::endl;
}
