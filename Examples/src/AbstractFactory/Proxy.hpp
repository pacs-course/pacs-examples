#ifndef GENERICPROXY_HPP_
#define GENERICPROXY_HPP_
#include <string>
#include <memory>
#include <iostream>
namespace GenericFactory {
  /*! A simple proxy for registering into a factory.

    It provides the builder as static method 
    and the automatic registration mechanism.

    \param Factory The type of the factory.  
    \param ConcreteProduct is the derived (concrete) type to be 
    registered in the factory
    
    @note I have to use the default builder provided by the factory. No check is made to verify it
    @todo Add check un builder type using type_traits and static_assert
  */
  template
  <typename Factory, typename ConcreteProduct>
  class Proxy {
  public:
    
    typedef typename  Factory::AbstractProduct_type AbstractProduct_type;
    typedef typename  Factory::Identifier_type Identifier_type;
    typedef typename  Factory::Builder_type Builder_type;
    typedef           Factory Factory_type;
    
    //! The constructor does the registration.
    Proxy(Identifier_type const &);

    //! The builder. Must comply with the signature.

    static std::unique_ptr<AbstractProduct_type> Build(){
      return std::unique_ptr<AbstractProduct_type>(new ConcreteProduct());
    }
    
  private:
    Proxy(Proxy const &)=delete; // only C++11
    Proxy & operator=(Proxy const &)=delete; // only C++11
  };
  
  
  template<typename F, typename C>
  Proxy<F,C>::Proxy(Identifier_type const & name) {
    // get the factory. First time creates it.
    Factory_type & factory(Factory_type::Instance());
    // Insert the builder. The & is not needed.
    factory.add(name,&Proxy<F,C>::Build);
    std::cout<<"Added "<< name << " to factory"<<std::endl;
  }
}

#endif /* RULESHANDLER_HPP_ */
