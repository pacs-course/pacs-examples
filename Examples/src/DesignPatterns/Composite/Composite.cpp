/*
 * Composite.cpp
 *
 *  Created on: 04/gen/2011
 *      Author: formaggia
 */
#include "Composite.hpp"
#include <iostream>
void
Element::operation() const
{
  std::cout << this->my_name << ", ";
}

Component::pComponent
Element::clone() const
{
  return std::make_unique<Element>(*this);
}

void
Composite::add(Component &c)
{
  this->my_composite.emplace_back(c.clone());
}

void
Composite::add(Component::pComponent const &p)
{
  this->my_composite.emplace_back(p->clone());
}

void
Composite::add(Component::pComponent &&p)
{
  this->my_composite.emplace_back(std::move(p));
}

void
Composite::operation() const
{
  std::cout << std::endl
            << this->my_name << ", which is composed by: " << std::endl;
  for(auto &i : this->my_composite)
    i->operation();
}

Composite::pComponent
Composite::clone() const
{
  return std::make_unique<Composite>(*this);
}

Composite::Composite(const Composite &c) : Component(c.my_name)
{
  for(auto &i : c.my_composite)
    this->my_composite.emplace_back(i->clone());
}

Composite &
Composite::operator=(const Composite &c)
{
  if(&c != this)
    {
      my_composite.clear();
      for(auto &i : c.my_composite)
        this->my_composite.emplace_back(i->clone());
      this->my_name = c.my_name;
    }
  return *this;
}
