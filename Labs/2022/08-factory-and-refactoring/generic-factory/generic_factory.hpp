/**
 * @file
 *
 * @author Pasquale Claudio Africa <pasqualeclaudio.africa@polimi.it>.
 */

#ifndef __GENERIC_FACTORY_HPP_
#define __GENERIC_FACTORY_HPP_

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>

/**
 * @brief Generic self-registering factory class.
 *
 * References:
 * -
 * https://lordsoftech.com/programming/generic-self-registering-factory-no-more-need-to-write-factories/
 * - https://github.com/Dugy/generic_factory
 * @endcode
 */
template <class Base, class... Args>
class GenericFactory
{
public:
  /**
   * Register a constructor of a possible child.
   *
   * @param[in] name    The name of the child type.
   * @param[in] builder A function that returns a <kbd>std::unique_ptr</kbd>
                        to a newly constructed child when called.
   * @return <kbd>true</kbd> if successfully added,
             <kbd>false</kbd> if the child already exists.
   */
  static bool
  register_child(const std::string &                                  name,
                 const std::function<std::unique_ptr<Base>(Args...)> &builder)
  {
    auto &factory = get_generic_factory();

    auto found = factory.children.find(name);

    if (found != factory.children.end())
      return false;

    factory.children[name] = builder;

    return true;
  }

  /**
   * Register a child from a constructor.
   * The template argument is the class of the child being created.
   *
   * @param[in] name The name of the child.
   * @return <kbd>true</kbd> if successfully added,
   *         <kbd>false</kbd> if the child already exists.
   */
  template <class Child>
  static bool
  register_child(const std::string &name)
  {
    return register_child(name, [](Args... args) {
      return std::make_unique<Child>(args...);
    });
  }

  /**
   * Unregister a child.
   *
   * @param[in] name The name of the child.
   * @return <kbd>true</kbd> if it was already registered,
   *         <kbd>false</kbd> if it wasn't.
   */
  static bool
  unregister_child(const std::string &name)
  {
    auto &factory = get_generic_factory();

    auto found = factory.children.find(name);

    if (found == factory.children.end())
      return false;

    factory.children.erase(found);

    return true;
  }

  /**
   * Create a child given its name and the arguments to be passed to its
   * builder/constructor.
   *
   * @param[in] name The name of the child.
   * @param[in] args Constructor arguments.
   */
  static std::unique_ptr<Base>
  create_child(const std::string &name, Args... args)
  {
    auto &factory = get_generic_factory();

    auto found = factory.children.find(name);

    if (found == factory.children.end())
      {
        std::cerr << "Unknown child: " + name << std::endl;
        std::exit(1);
      }

    return found->second(args...);
  }

  /**
   * Get the keys of all registered children (or those matching the
   * subset specified as an input argument).
   */
  static std::set<std::string>
  get_registered_keys(const std::set<std::string> &valid_children = {})
  {
    auto &factory = get_generic_factory();

    std::set<std::string> children_names;

    for (const auto &child : factory.children)
      {
        if (valid_children.empty() ||
            valid_children.find(child.first) != valid_children.end())
          children_names.insert(child.first);
      }

    return children_names;
  }

private:
  /// Default constructor (defaulted).
  /// There is no need to forbid copying or moving, because it's impossible to
  /// obtain an instance from outside.
  GenericFactory() = default;

  /**
   * Force the initialization of @ref children by circumventing the
   * "Static Initialization Order Fiasco"
   * (see <a
   * href="https://www.jibbow.com/posts/cpp-header-only-self-registering-types/">here</a>).
   * Since this method is invoked from this very same file, the static member
   * GenericFactory declared inside of it is always initialized before other
   * <kbd>static</kbd> variables that can possibly depend on the @ref children
   * map.
   */
  static GenericFactory &
  get_generic_factory()
  {
    static GenericFactory factory;
    return factory;
  }

  /// Children registry. It is stored as a map, whose keys are strings and
  /// values are functions returning a pointer to the base class.
  std::map<std::string, std::function<std::unique_ptr<Base>(Args...)>> children;
};

#endif /* __GENERIC_FACTORY_HPP_ */
