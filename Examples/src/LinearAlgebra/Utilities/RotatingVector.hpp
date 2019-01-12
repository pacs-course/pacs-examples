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
   *  Useful if you need to keep track of just the last N elements
   */
template <class T, std::size_t N>
class RotatingVector
{
public:
  //! Returns the underlying container
  auto  getVector()const {return M_vec;}
  //! Returns the underlying container
  auto& getVector(){return M_vec;}
  //! Returns i-th stored item
  T  operator[](std::size_t i)const {return M_vec[i];}
  //! Returns i-th stored item
  T& operator[](std::size_t i){return M_vec[i];}
  //! The current size
  auto  size()const {return M_size;}
  //! The last element
  auto & back(){return M_vec[M_size];}
  //! The last element
  auto  back()const {return M_vec[M_size];}
  //! true if all elements are filled up
  bool full()const {return M_size==N;}
  //! true if empty
  bool empty()const {return M_size==0u;}
  //! Add an element to the end of the vector by callint T(args)
  template <class... Args>
  void emplace_back (Args&&... args)
  {
    if (M_size < N)
      {
        M_vec[M_size]= T(std::forward<Args>(args)...);
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
        M_vec[M_size]=args;
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
  std::array<T,N> M_vec;
};

}



#endif /* SRC_LINEARALGEBRA_UTILITIES_ROTATINGVECTOR_HPP_ */
