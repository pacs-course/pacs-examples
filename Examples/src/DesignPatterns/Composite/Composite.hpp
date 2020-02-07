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
#include <memory>
#include <memory>
class Component{
public:
  Component(std::string const & n):_name(n){};
  virtual void operation() const=0;
  virtual std::unique_ptr<Component> clone() const=0;
  virtual ~Component()=default;
protected:
  using pComponent=std::unique_ptr<Component>;
  std::string _name;
};

class Element: public Component
{
public:
  Element(std::string const & n):Component(n){};
  void operation() const override;
  Component::pComponent clone() const override;
  virtual ~Element()=default;
protected:
};

class Composite: public Component{
public:
  Composite(std::string const & n): Component(n),_composite(){};
  //! Stores a copy of any component
  void add(Component & c);
  //! Takes a unique pointer lvalue
  /*
    Uses clone to create the copy. The unique pointer given as
    argument will remain unaltered
   */
  void add(Component::pComponent  const & p);
  //! Takes a unique pointer rvalue
  /*
    The unique pointer is moved
   */
  void add(Component::pComponent&& p);
  
  void operation() const override;
  //! Does a Deep Copy
  Composite(Composite const & c);
  //! Does a Deep Assignment
  Composite & operator = (const Composite & c);
  Component::pComponent clone() const override;
  virtual ~Composite()=default;
  //! The type of the list
  using ComponentList=std::list<Component::pComponent>;
protected:
private:
  ComponentList _composite;
};









#endif /* COMPOSITE_HPP_ */
