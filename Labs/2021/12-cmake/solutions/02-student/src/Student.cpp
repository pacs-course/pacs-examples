#include "Student.hpp"

#include <iostream>

Student::Student(const std::string &name)
  : name(name)
{}

void
Student::display()
{
  std::cout << "A student with name " << this->name << "." << std::endl;
}
