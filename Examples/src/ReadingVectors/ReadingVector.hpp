/*
 * ReadingVector.hpp
 *
 *  Created on: Jun 10, 2020
 *      Author: forma
 */
#include <vector>
#ifndef EXAMPLES_SRC_READINGVECTORS_READINGVECTOR_HPP_
#define EXAMPLES_SRC_READINGVECTORS_READINGVECTOR_HPP_

namespace apsc::Utilities
{
/*!
 * @brief Overloading of the streaming operator to fill std::vectors
 *
 * You may fill a vector using the streaming operator
 * @tparam T the type stored in the vector (automatically deduced)
 * @tparam Args Additional vector arguments (automatically deduced)
 * @param v the vector
 * @param i the value
 * @return The vector
 *
 */
template <class T>
std::vector<T> &
operator<<(std::vector<T> &v, T&&i)
{
  v.emplace_back(std::forward<T>(i));
  return v;
}
/*!
 * Overloading the comma operator if you want the "Eigen style" way of filling
 * vectors
 *
 * @tparam T the type stored in the vector (automatically deduced)
 * @tparam Args Additional vector arguments (automatically deduced)
 * @param v the vector
 * @param i the value
 * @return The vector
 */
template<class T>
std::vector<T> & operator , (std::vector<T> & v, T&& i)
{
  v.emplace_back(std::forward<T>(i));
  return v;
};
}
#endif /* EXAMPLES_SRC_READINGVECTORS_READINGVECTOR_HPP_ */
