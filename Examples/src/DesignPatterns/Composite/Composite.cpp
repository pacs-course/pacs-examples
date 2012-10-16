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
	this->_composite.push_back(*c.clone());
}


void Composite::operation() const
{
	std::cout<<std::endl<<this->_name<<", which is composed by: "<<std::endl;
	for (ComponentList::const_iterator i=_composite.begin();
			i != _composite.end();++i )
		(*i)->operation();
}


Composite *Composite::clone()const
{
	return new Composite(*this);
}


Composite::Composite(const Composite & c):Component(c._name),
		_composite(c._composite)
{
}

Composite & Composite::operator= (const Composite & c)
{
	if (&c != this){
		ComponentList tmp(c._composite);
		this->_composite.swap(tmp);
		this->_name=c._name;
	}
	return *this;
}

