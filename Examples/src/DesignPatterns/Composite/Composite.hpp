/*
 * Composite.hpp
 *
 *  Created on: 03/gen/2011
 *      Author: formaggia
 */

#ifndef COMPOSITE_HPP_
#define COMPOSITE_HPP_
#include <list>
#include <string>
#include "wrapper.hpp"
class Component{
public:
	Component(std::string const & n):_name(n){};
	virtual void operation() const=0;
	virtual Component * clone() const=0;
	virtual ~Component(){};
protected:
	typedef Wrapper<Component> pComponent;
	std::string _name;
};

class Element: public Component{
public:
	Element(std::string const & n):Component(n){};
	virtual void operation() const;
	virtual Element * clone() const;
	virtual ~Element(){};
protected:
};

class Composite: public Component{
public:
	Composite(std::string const & n):
		Component(n),_composite(){};
	virtual void add(Component & c);
	virtual void operation() const;
	Composite(Composite const & c);
	Composite & operator = (const Composite & c);
	virtual Composite * clone() const;
	virtual ~Composite(){};
	typedef std::list<Component::pComponent> ComponentList;
protected:
private:
	ComponentList _composite;
};









#endif /* COMPOSITE_HPP_ */
