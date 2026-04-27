#ifndef H_FACTORY_HPP_
#define H_FACTORY_HPP_
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <vector>
namespace GenericFactory
{
/*!
 * @brief Singleton factory mapping identifiers to builders.
 *
 * This template implements a generic factory pattern. A factory instance stores
 * builder objects indexed by an identifier and later uses them either to create
 * concrete objects derived from a common base class or, in the special
 * function-factory case, to retrieve callable objects directly.
 *
 * The factory is implemented as a Meyers singleton, so all access goes through
 * `Factory::Instance()`.
 *
 * Typical usage:
 * @code
 * using MyFactory = GenericFactory::Factory<Base, std::string>;
 * auto &factory = MyFactory::Instance();
 * factory.add("item", [] { return std::make_unique<Derived>(); });
 * auto object = factory.create("item");
 * @endcode
 *
 * @tparam AbstractProduct Base type of the objects produced by the factory.
 * Use `void` when the factory stores functions instead of object builders.
 * @tparam Identifier Key type used to select the builder.
 * @tparam Builder Callable type stored in the factory. Its return type must be
 * compatible with `std::unique_ptr<AbstractProduct>` when `create()` is used.
 *
 * @throw std::invalid_argument Thrown by `get()` and `add()` on lookup failure
 * or duplicate registration.
 *
 * @note `Identifier` must be streamable to `std::ostream`, because diagnostic
 * messages are built through `std::stringstream`.
 */
template <typename AbstractProduct, typename Identifier,
          typename Builder = std::function<std::unique_ptr<AbstractProduct>()> >
class Factory
{
public:
  //! Base-product type associated with the factory.
  using AbstractProduct_type = AbstractProduct;
  //! Identifier type used as the lookup key.
  using Identifier_type = Identifier;
  //! Builder type stored in the factory.
  /*!
   * The default is a nullary function wrapper returning
   * `std::unique_ptr<AbstractProduct>`.
   */
  using Builder_type = Builder;
  //! Returns the unique singleton instance of the factory.
  static Factory &Instance();

  /*!
   * @brief Creates an object by invoking the builder associated with `name`.
   *
   * The arguments are perfectly forwarded to the stored builder. This allows
   * the factory to work also with builders that take runtime parameters.
   *
   * @tparam Args Argument types forwarded to the builder.
   * @param name Identifier selecting the registered builder.
   * @param args Arguments forwarded to the stored builder.
   * @return A smart pointer to the created object.
   *
   * @note This method is intended for object factories. When
   * `AbstractProduct = void`, use `get()` instead.
   */
  template <typename... Args>
  std::unique_ptr<AbstractProduct> create(Identifier const &name,
                                          Args &&...args) const;

  /*!
   * @brief Returns the builder associated with an identifier.
   * @param name Identifier selecting the builder.
   * @return A copy of the registered builder.
   * @throw std::invalid_argument If `name` is not registered.
   */
  Builder get(Identifier const &name) const;

  /*!
   * @brief Registers a builder under the given identifier.
   * @param name Identifier used as the registration key.
   * @param builder Builder to be stored in the factory.
   * @throw std::invalid_argument If `name` is already registered.
   */
  void add(Identifier const &, Builder_type const &);

  /*!
   * @brief Returns the list of currently registered identifiers.
   * @return A vector containing every identifier stored in the factory.
   */
  std::vector<Identifier> registered() const;

  /*!
   * @brief Removes one registered builder.
   * @param name Identifier to erase.
   *
   * If `name` is not present, the operation is a no-op.
   */
  void
  unregister(Identifier const &name)
  {
    _storage.erase(name);
  }

  //! Removes every registered builder from the factory.
  void
  clear()
  {
    _storage.clear();
  }

  /*!
   * @brief Checks whether the factory is empty.
   * @return `true` if no builders are currently registered.
   */
  bool
  empty() const
  {
    return _storage.empty();
  }

private:
  /*!
   * @brief Internal associative container used to store the builders.
   *
   * An `std::unordered_map` is used because the factory needs fast lookup and
   * does not rely on any ordering of the identifiers.
   */
  using Container_type = std::unordered_map<Identifier, Builder_type>;

  //! Constructor is private because the factory is a singleton.
  Factory() = default;

  //! Copy construction is disabled because the factory is a singleton.
  Factory(Factory const &) = delete;

  //! Copy assignment is disabled because the factory is a singleton.
  Factory &operator=(Factory const &) = delete;

  //! Storage of registered identifier-to-builder associations.
  Container_type _storage;
};

//! Convenience alias for a factory of callable objects.
/*!
 * In this case the factory stores functions directly instead of object
 * builders, so client code typically uses `get()` rather than `create()`.
 */
template <typename Identifier, typename FunType>
using FunctionFactory = Factory<void, Identifier, FunType>;

//    ****   IMPLEMENTATIONS  ****

template <typename AbstractProduct, typename Identifier, typename Builder>
Factory<AbstractProduct, Identifier, Builder> &
Factory<AbstractProduct, Identifier, Builder>::Instance()
{
  // Meyers singleton: initialized on first use.
  static Factory theFactory;
  return theFactory;
}

template <typename AbstractProduct, typename Identifier, typename Builder>
Builder
Factory<AbstractProduct, Identifier, Builder>::get(Identifier const &name) const
{
  auto f = _storage.find(name);
  if(f == _storage.end())
    {
      std::stringstream idAsString;
      // The identifier is rendered into the diagnostic message.
      idAsString << name;
      std::string out =
        "Identifier " + idAsString.str() + " is not stored in the factory";
      throw std::invalid_argument(out);
    }
  else
    {
      return f->second;
    }
}

template <typename AbstractProduct, typename Identifier, typename Builder>
template <typename... Args>
std::unique_ptr<AbstractProduct>
Factory<AbstractProduct, Identifier, Builder>::create(Identifier const &name,
                                                      Args &&...args) const
{
  static_assert(!std::is_same_v<AbstractProduct, void>,
                "You should use get() not create() on FunctionFactories");
  // Perfect forwarding preserves value category and constness.
  return this->get(name)(std::forward<Args>(args)...);
}

template <typename AbstractProduct, typename Identifier, typename Builder>
void
Factory<AbstractProduct, Identifier, Builder>::add(Identifier const   &name,
                                                   Builder_type const &func)
{
  auto f = _storage.insert(std::make_pair(name, func));
  if(f.second == false)
    {
      std::stringstream idAsString;
      idAsString << name;
      std::string message =
        std::string("Double registration in Factory of id: ") +
        idAsString.str() + std::string(" is not allowed");
      throw std::invalid_argument(message);
    }
}

template <typename AbstractProduct, typename Identifier, typename Builder>
std::vector<Identifier>
Factory<AbstractProduct, Identifier, Builder>::registered() const
{
  std::vector<Identifier> tmp;
  tmp.reserve(_storage.size());
  for(auto const &i : _storage)
    tmp.emplace_back(i.first);
  return tmp;
}

} // namespace GenericFactory

#endif /* BC_FACTORY1_HPP_ */
