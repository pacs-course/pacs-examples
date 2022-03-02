#include "Student_v1.hpp"

#include <iostream>

Student::Student(const std::string &name)
  : name(name)
{}

void
Student::display()
{
  std::cout << "My name is " << this->name << "." << std::endl;
}
