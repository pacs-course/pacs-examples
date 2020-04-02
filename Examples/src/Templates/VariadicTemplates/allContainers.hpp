/*
 * allContainers.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_ALLCONTAINERS_HPP_
#define EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_ALLCONTAINERS_HPP_
#include <iostream>
namespace apsc{

 //! A function that prints the content of all containers that
 //! may be constructed as Container<Type> (maybe with additional template parameters defaulted)
 //!
 //! I assume that Type is printable
 //!
  template <template <typename, typename...> class ContainerType,
            typename ValueType, typename... Args>
  void printContainerValues(const ContainerType<ValueType, Args...>& c)
  {
    for (const auto& v : c) {
	std::cout << v << ' ';
    }
      std::cout << std::endl;
  }
}//end namespace apsc




#endif /* EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_ALLCONTAINERS_HPP_ */
