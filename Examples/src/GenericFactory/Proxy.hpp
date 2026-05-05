#ifndef GENERICPROXY_HPP_
#define GENERICPROXY_HPP_
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
namespace GenericFactory
{
/*!
 * @brief Helper object that registers a concrete product into a factory.
 *
 * The proxy is typically instantiated as a namespace-scope static object so
 * that registration happens during static initialization, without requiring an
 * explicit call from client code.
 *
 * @tparam Factory Type of the target factory. It must be a specialization of
 * `GenericFactory::Factory`.
 * @tparam ConcreteProduct Concrete type to be registered.
 */
template <typename Factory, typename ConcreteProduct> class Proxy
{
public:
  //! Abstract base type associated with the target factory.
  using AbstractProduct_type = typename Factory::AbstractProduct_type;
  //! Identifier type used by the target factory.
  using Identifier_type = typename Factory::Identifier_type;
  //! Builder type expected by the target factory.
  using Builder_type = typename Factory::Builder_type;
  //! Return type of the builder callable.
  using Result_type = std::invoke_result_t<Builder_type>;
  //! Target factory type.
  using Factory_type = Factory;

  /*!
   * @brief Registers `ConcreteProduct` using the built-in builder.
   * @param name Identifier used for registration.
   *
   * This overload assumes that `Builder_type` is compatible with the static
   * member `Build()`.
   */
  explicit Proxy(Identifier_type const &name);

  /*!
   * @brief Registers `ConcreteProduct` using an explicitly supplied builder.
   * @param name Identifier used for registration.
   * @param b Builder to be stored in the factory.
   *
   * This overload is useful when the default `Build()` helper does not match
   * the desired builder signature.
   */
  Proxy(Identifier_type const &name, Builder_type const &b);

  /*!
   * @brief Default builder used by the proxy.
   * @return A newly created concrete product wrapped in `Result_type`.
   *
   * This implementation assumes that `Result_type` can be constructed from
   * `new ConcreteProduct()`, which is true for the default
   * `std::unique_ptr<AbstractProduct>`-based builders used by `Factory`.
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
  //! Proxies are not copyable.
  Proxy(Proxy const &) = delete; //
  //! Proxies are not copy-assignable.
  Proxy &operator=(Proxy const &) = delete; //
};

template <typename F, typename C>
Proxy<F, C>::Proxy(Identifier_type const &name)
{
  // Accessing the singleton factory also creates it on first use.
  Factory_type &factory = Factory_type::Instance();
  // Register the default builder for the concrete product.
  factory.add(name, &Proxy<F, C>::Build);
  std::clog << "Added " << name << " to factory" << std::endl;
}

template <typename F, typename C>
Proxy<F, C>::Proxy(Identifier_type const &name, Builder_type const &b)
{
  // Accessing the singleton factory also creates it on first use.
  Factory_type &factory = Factory_type::Instance();
  // Register the caller-supplied builder.
  factory.add(name, b);
  std::clog << "Added " << name << " to factory" << std::endl;
}

} // namespace GenericFactory

#endif /* RULESHANDLER_HPP_ */
