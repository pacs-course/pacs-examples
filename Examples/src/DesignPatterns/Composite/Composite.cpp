/*
 * Composite.cpp
 *
 *  Created on: 04/gen/2011
 *      Author: formaggia
 */
#include <iostream>
#include "Composite.hpp"
void Element::operation() const
{
	std::cout<<this->_name<<", ";
}

Element * Element::clone()const
{
	return new Element(*this);
}

void Composite::add(Component & c)
{
	this->_composite.emplace_back(c.clone());
}


void Composite::operation() const
{
	std::cout<<std::endl<<this->_name<<", which is composed by: "<<std::endl;
	for (auto & i : this->_composite) i->operation();
}


Composite *Composite::clone()const
{
	return new Composite(*this);
}


Composite::Composite(const Composite & c):Component(c._name)
{
  for (auto & i : c._composite)_composite.emplace_back(i->clone());
}

Composite & Composite::operator= (const Composite & c)
{
  if (&c != this){
    _composite.clear();
    for (auto & i : c._composite)_composite.emplace_back(i->clone());
    this->_name=c._name;
  }
  return *this;
}

