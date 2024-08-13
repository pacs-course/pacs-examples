#ifndef GENERICPROXY_HPP_
#define GENERICPROXY_HPP_
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
namespace GenericFactory
{
/*! A simple proxy for registering into a factory.

  It provides the builder as static method
  and the automatic registration mechanism.

  \tparam Factory The type of the factory. It must be a specialization of
  GenericFactory::Factory<>.
  \tparam ConcreteProduct is the derived (concrete)
  type to be registered in the factory

*/
template <typename Factory, typename ConcreteProduct> class Proxy
{
public:
  using AbstractProduct_type = typename Factory::AbstractProduct_type;
  using Identifier_type = typename Factory::Identifier_type;
  using Builder_type = typename Factory::Builder_type;
  // The type returned by the builder
  using Result_type = std::invoke_result_t<Builder_type>;
  using Factory_type = Factory;

  /*! The constructor does the registration.
    @param name The identifier
    @note I use the builder type provided by the factory. No check is made to
    verify that it is consistant with the one provided by this Proxy. the
    builder type must take no arguments and have a return type that accepts a
    pointer to the ConcreteProduct as argument.
  */
  Proxy(Identifier_type const &name);

  /*! This version takes also the builder. It does not use the builder already
    provided by the class.To be used in case on incompatibilities.
    @param name The identifier
    @param builder The builder
   */
  Proxy(Identifier_type const &name, Builder_type const &b = Builder_type{});

  //! The in-built builder. Must comply with the signature.
  /*!
    Actually you do not need to use the proxy to define the builder,
    but this way you have only one object that does everything
    I assume that the builder is a type that returns Result_type
    and that Result_type can be constructed with a pointer the
    concrete product. I also assume that Result_type takes
    care of memory handling (i.e. is a unique_ptr or something
    that behaves like a unique_prt).
   */
  static Result_type
  Build()
  {
    return Result_type(new ConcreteProduct());
  }
  //! This is a more explicit version
  //    static std::unique_ptr<AbstractProduct_type> Build(){
  // return std::make_unique<ConcreteProduct>();
  //}

private:
  Proxy(Proxy const &) = delete;            //
  Proxy &operator=(Proxy const &) = delete; //
};

template <typename F, typename C>
Proxy<F, C>::Proxy(Identifier_type const &name)
{
  // get the factory. First time creates it.
  Factory_type &factory(Factory_type::Instance());
  // Insert the builder. The & is not needed, but it does not hurt.
  factory.add(name, &Proxy<F, C>::Build);
  std::clog << "Added " << name << " to factory" << std::endl;
}

template <typename F, typename C>
Proxy<F, C>::Proxy(Identifier_type const &name, Builder_type const &b)
{
  // get the factory. First time creates it.
  Factory_type &factory(Factory_type::Instance());
  // Insert the builder. The & is not needed, but it does not hurt.
  factory.add(name, b);
  std::clog << "Added " << name << " to factory" << std::endl;
}

} // namespace GenericFactory

#endif /* RULESHANDLER_HPP_ */
