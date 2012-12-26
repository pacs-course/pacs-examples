#ifndef _SUM_HPP__
#define _SUM_HPP__
// Include functional for operation 
// Include boost for some example
#include <boost/type_traits.hpp>
#include "Vcr.hpp"
#include <iterator>

//! A first example of sum
/**
 It assumes that the Container has:
 a type const_iterator defining a class with iterator semantic
 a type value_type with the type of the contained elements
 a method begin() returning iterator to the start
 a method end()   returning iterator to the end of the container

  @Note  use std::accumulate(), this is only an example!
*/

template<class Container>
typename Container::value_type 
sum1(Container const & c){
  typedef typename Container::const_iterator It;
  typedef typename Container::value_type value_type;
  value_type temp(0);
  for (It i=c.begin();i!=c.end();++i)temp+=*i;
  return temp;
}
//! Functions to perform the dereferencing accounting for pointers
template<class T>
inline T deref(T const & i){
  return i; }
//! Specialization for pointers
template<class T >
inline T deref(T * const & i){
  return *i; }


//! traits to implement sum in a more generic way

template <class Container> 
struct ContainerTraits{
  typedef typename Container::value_type original_value_type;
  // if the value is a pointer value type must be the type without pointer
  // to take out the possible pointer I use the boost TypeTrait library
  typedef typename boost::remove_pointer<original_value_type>::type value_type;
  typedef typename Container::const_iterator const_iterator;
  //! Starting iterator
  static const_iterator start(const Container & c){
    return c.begin();
  }
  //! Final iterator
  static const_iterator finish(const Container & c){
    return c.end();
  }
};

//! Specialization for Vcr

template <typename T> 
struct ContainerTraits<Vcr<T> >{
  typedef Vcr<T> Container;
  typedef T original_value_type;
  // if the value is a pointer value type must be the type without pointer
  // to take out the possible pointer I use the boost TypeTrait library
  typedef typename boost::remove_pointer<original_value_type>::type value_type;
  typedef T const *  const_iterator;
  //! Starting iterator
  static const_iterator start(const Container & c){
    return &(c[0]);
  }
  //! Final iterator
  static const_iterator finish(const Container & c){
    return &(c[c.size()]);
  }
};


//! Version using tratis
template<class Container>
typename ContainerTraits<Container>::value_type 
sum2(Container const & c){
  typedef typename ContainerTraits<Container>::const_iterator It;
  typedef typename ContainerTraits<Container>::value_type value_type;
  value_type temp(0);
  for (It i= ContainerTraits<Container>::start(c);
       i!=ContainerTraits<Container>::finish(c);++i)temp+=deref(*i);
  return temp;
}


template<typename T>
struct IteratorTraits{
  // for stl compliant iterators
  typedef typename IteratorTraits::value_type original_value_type;
  typedef typename boost::remove_pointer<original_value_type>::type value_type;
};

//Specialization for pointers
template<typename T>
struct IteratorTraits<T *>{
  // for stl compliant iterators
  typedef T * original_value_type;
  typedef typename boost::remove_pointer<original_value_type>::type value_type;
};


//! Version using Iterators (to be implemented)
/*!
  Why not use std::accumulate() at this point?
 */
template<typename Iterator>
typename IteratorTraits<Iterator>::value_type 
sum3(Iterator const & b, Iterator const & e){
  typedef typename IteratorTraits<Iterator>::value_type value_type;
  value_type temp(0);
  for (Iterator i(b); i!=e;++i)temp+=deref(*i);
  return temp;
}


#endif
