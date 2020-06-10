/*
 * ReadingVector.hpp
 *
 *  Created on: Jun 10, 2020
 *      Author: forma
 */
#include <vector>
#ifndef EXAMPLES_SRC_READINGVECTORS_READINGVECTOR_HPP_
#define EXAMPLES_SRC_READINGVECTORS_READINGVECTOR_HPP_
/*!
 * @brief Overloading of the streaming operator to fill std::vectors
 *
 * You may fill a vector using the streaming operator
 * @tparam T the type stored in the vector (automatically deduced)
 * @tparam Args Additional vector argument (automatically deduced)
 * @param v the vector
 * @param i the value
 * @return The vector
 *
 */
template<class T, class ...Args>
std::vector<T,Args...> & operator << (std::vector<T,Args...> & v, T const & i)
{
  v.emplace_back(i);
  return v;
}
/*!
 * Overloading the comma operator if you want the "Eigen style" way of filling vectors
 *
 * @tparam T the type stored in the vector (automatically deduced)
 * @tparam Args Additional vector argument (automatically deduced)
 * @param v the vector
 * @param i the value
 * @return The vector
 */
template<class T, class ...Args>
std::vector<T,Args...> & operator , (std::vector<T,Args...> & v, T const & i)
{
  v.emplace_back(i);
  return v;
};


#endif /* EXAMPLES_SRC_READINGVECTORS_READINGVECTOR_HPP_ */
