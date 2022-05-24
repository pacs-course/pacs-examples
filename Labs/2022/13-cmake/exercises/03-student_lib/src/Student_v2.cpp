#include "Student_v2.hpp"

#include <iostream>

Student::Student(const std::string &name, const unsigned int &age)
  : name(name)
  , age(age)
{}

void
Student::display()
{
  std::cout << "My name is " << this->name << " and I am " << this->age
            << " years old." << std::endl;
}
