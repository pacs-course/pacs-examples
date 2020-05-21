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

namespace apsc
{
  //! This class represents a vector that keeps only the latest insertions
  /*!
   *  Useful if you need to keep track of just the last inserted
   *  elements, up to a maximal number, when a newly inserted element
   *  causes the elimination of the front element, a shift to the left
   *  by one, and the addition at the back. It is a thin wrapper around
   *  an std::array, where I redefine emplace_back and push_back, and
   *  back methods. The array is composed in the class.  
   *
   *  More precisely, if we have a sequence of items \f$ K_i, i=0,\ldots$
   *  of type T, a RotatingVector<T<N> will store only the last inserted,
   *  up to N, preserving the order of insertion, i.e. the last inserted
   *  is at the back.
      
      /tparam T the type stored in the rotating vector 
      /tparam N the maximal number of elements in the rotating vector
   */
template <class T, std::size_t N>
class RotatingVector
{
public:
  //! Returns the underlying container
  //! This way I have access also to all other methods of std::array
  auto  getVector()const {return M_vec;}
  //! Returns the underlying container
  auto& getVector(){return M_vec;}
  //! Returns i-th stored item
  T  operator[](std::size_t i)const {return M_vec[i];}
  //! Returns i-th stored item
  T& operator[](std::size_t i){return M_vec[i];}
  //! The current size
  auto  size()const {return M_size;}
  //! The max number of elements
  auto constexpr max_size(){return N;}
  //! The last element
  auto & back(){return M_vec[M_size-1];}
  //!  get the last element inserted starting from last
  //! @note no check is made to verify that the element is valid
  //! back<0> is equivalent to back()
  template<unsigned int i>
  auto & back(){return M_vec[M_size-1-i];}
  //! The last element
  auto  back()const {return M_vec[M_size-1];}
  //!  get the last element inserted starting from last
  //! @note no check is made to verify that the element is valid
  //! back<0> is equivalent to back()
  template<unsigned int i>
  auto  back()const {return M_vec[M_size-1-i];}  //! true if all elements are filled up
  //! @note no check is made to test if the call is valid
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
  std::size_t M_size=0u;
  std::array<T,N> M_vec;
};

  //! A variant of RotatingVector<T,N> where the maximal number of elements can by set run time
  /*!
   *  Useful if you need to keep track of just the last inserted
   *  elements, up to a maximal number. When the max size is reached, 
   *  a newly inserted element will cause the elimination of the front element,
   *  a shift to the left by one, and the addition at the back. 
   *  It is a thin wrapper around an std::vector, where I redefine 
   *  emplace_back and push_back, and back methods. 
   *  The vector is composed in the class.  
      /tparam T the type stored in the rotating vector 
   */
  template <class T>
  class RotatingVectorXd
  {
  public:
    //!Default constructor
    RotatingVectorXd()=default;
    //! Specify max size
    RotatingVectorXd(std::size_t n):M_N{n}{M_vec.reserve(n);}
    //! Returns the underlying container
    //! This way I have access also to all other methods of std::vector
    auto  getVector()const {return M_vec;}
    //! Returns the underlying container
    auto& getVector(){return M_vec;}
    //! Returns i-th stored item
    T  operator[](std::size_t i)const {return M_vec[i];}
    //! Returns i-th stored item
    T& operator[](std::size_t i){return M_vec[i];}
    //! The current size
    auto  size()const {return M_vec.size();}
    //! Set a new maximal size
    void setMaxSize(std::size_t m)
    {
      if(m<M_N)
        {
          // I have to save the last elements up to m: If I shrink I want to keep the last elements!
          std::vector<T> tmp{M_vec.begin()+std::max(std::size_t(0), M_vec.size()-m), M_vec.end()};
          M_vec=std::move(tmp);// to spare useless temporaries
        }
      M_N=m;
    }
    //! The max number of elements
    auto constexpr max_size(){return M_N;}
    //! The last element
    auto & back(){return M_vec.back();}
    //!  get the last element inserted starting from last
    //! @note no check is made to verify that the element is valid
    //! back<0> is equivalent to back()
    template<unsigned int i>
    auto & back(){return *(M_vec.end()-1-i);}
    //! The last element
    auto  back()const {return M_vec.back();}
    //!  get the last element inserted starting from last
    //! @note no check is made to verify that the element is valid
    //! back<0> is equivalent to back()
    template<unsigned int i>
    auto  back()const {return *(M_vec.end()-1-i);}
    //! true if all elements are filled up
    bool full()const {return this->size()=M_N;}
    //! true if empty
    bool empty()const {return M_vec.empty();}
    //! true if no max size has been set
    bool unallocated()const {return M_N==0u;}
    //! Add an element to the end of the vector by calling T(args)
    /*!
      \note No check is made to test that the max size >0. You will get a runtime error
    */
    template <class... Args>
    void emplace_back (Args&&... args)
    {
      if (M_vec.size() < M_N)
        {
          M_vec.emplace_back(std::forward<Args>(args)...);
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
      if (M_vec.size() < M_N)
        {
          M_vec.push_back(args);
        }
      else
        {
          std::rotate(M_vec.begin(),M_vec.begin()+1,M_vec.end());
          M_vec.back()=args;
        }
    }
  private:
    std::size_t M_N=0u;
    std::vector<T> M_vec;
  };
}//end namespace apsc



#endif /* SRC_LINEARALGEBRA_UTILITIES_ROTATINGVECTOR_HPP_ */
