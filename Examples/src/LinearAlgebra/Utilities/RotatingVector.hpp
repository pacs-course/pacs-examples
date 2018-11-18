/*
 * RotatingVector.hpp
 *
 *  Created on: Nov 18, 2018
 *      Author: forma
 */

#ifndef SRC_LINEARALGEBRA_UTILITIES_ROTATINGVECTOR_HPP_
#define SRC_LINEARALGEBRA_UTILITIES_ROTATINGVECTOR_HPP_
#include <vector>
#include <algorithm>

namespace LinearAlgebra
{
  //! This class represent a vector that rotates cyclically the components
  /*!
   *
   */
template <class T, std::size_t N>
class RotatingVector
{
public:
  //! Returns the underlying vector
  auto  getVector()const {return M_vec;}
  //! Returns the underlying vector
  auto& getVector(){return M_vec;}
  //! Returns i-th stored item
  T  operator[](std::size_t i)const {return M_vec[i];}
  //! Returns i-th stored item
  T& operator[](std::size_t i){return M_vec[i];}
  //! The current size
  auto  size()const {return M_size;}
  //! Reserves space. Useful if T is of big size
  /*!
   * Reserving avoids the use of automatic reallocation
   */
  void  reserve(std::size_t n)
  {
    M_vec.reserve(n);
  }
  //! Add an element to the end of the vector by callint T(args)
  template <class... Args>
  void emplace_back (Args&&... args)
  {
    if (M_size < N)
      {
        M_vec.emplace_back(std::forward<Args>(args)...);
        M_size++;
      }
    else
      {
        std::rotate(M_vec.begin(),M_vec.begin()+1,M_vec.end());
        M_vec.back()= T(std::forward<Args>(args)...);
      }
  }
  //! Add an element to the end of the vector
  void push_back (const T& args)
  {
    if (M_vec.size() < M_size)
      {
        M_vec.push_back(args);
        M_size++;
      }
    else
      {
        std::rotate(M_vec.begin(),M_vec.begin()+1,M_vec.end());
        M_vec.back()=args;
      }
  }
private:
  unsigned int M_size=0u;
  std::vector<T> M_vec;
};

}



#endif /* SRC_LINEARALGEBRA_UTILITIES_ROTATINGVECTOR_HPP_ */
