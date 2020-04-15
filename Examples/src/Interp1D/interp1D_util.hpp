/*
 * interp1D_util.hpp
 *
 *  Created on: Apr 15, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_INTERP1D_INTERP1D_UTIL_HPP_
#define EXAMPLES_SRC_INTERP1D_INTERP1D_UTIL_HPP_
#include "interp1D.hpp"
#include<vector>
#include <numeric>
namespace apsc
{
  //
  /*! Implementation for vector<T> where T behaves like an array with 2 components (key value).
   *
   * @tparam CompOper Comparison operator
   * @tparam T the type stores in the array. it should behave as an array of 2 components. The first is the key, the second is the value
   * @param v a vector
   * @param keyVal the point to be interpolates
   * @param comp Comparison operator, defaults to less<T>
   * @return
   */
  template <typename A, typename CompOper=std::less<double>>
      inline double interp1D(std::vector<A> const & v, double const & keyVal,CompOper const & comp=std::less<double>{})
  {

    return interp1D(
	v.cbegin(),
	v.cend(),
	keyVal,
	[](A const & x){return x[0];},
	[](A const & x){return x[1];},
	comp
    );

  }
  namespace internals
  {
    //! Maps vector of keys and vector of values in a single entity.
    template <class T>
    class map2Vectors
    {
      using KeyVec=std::vector<double>;
      using ValueVec=std::vector<T>;
      using const_iterator = std::vector<std::size_t>::const_iterator;
    public:
      map2Vectors(KeyVec const & k, ValueVec const & v):keys{k},values{v},indexes{}
      {
	indexes.resize(keys.size());
	std::iota(indexes.begin(),indexes.end(),0);
      }

      const_iterator cbegin() const noexcept {return indexes.cbegin();}
      const_iterator cend() const noexcept {return indexes.cend();}

      KeyVec const & keys;
      ValueVec const & values;
    private:
     std::vector<std::size_t> indexes;
    };

    //! extracts the key
    template <class T>
    class extractKey_map2Vectors
    {
    public:
      extractKey_map2Vectors(map2Vectors<T> const & map):map2v{map}{}
      double operator () (size_t const & i)const {return map2v.keys[i];}
    private:
      map2Vectors<T> const & map2v;
    };

    //! Extracts the value
    template <class T>
    class extractValue_map2Vectors
    {
    public:
      extractValue_map2Vectors(map2Vectors<T> const & map):map2v{map}{}
      T operator () (size_t const & i)const {return map2v.values[i];}
    private:
      map2Vectors<T> const & map2v;
    };
  }
  //

  /*!
   *  Implementation when interpolation nodes and values are stores in two different vectors
   *
   * @tparam T  The type of values to be interpolated
   * @tparam CompOper The comparison operator among keys (default less<double>)
   * @param keys The vector containing the keys (interpolation nodes)
   * @param values The vector containing the values
   * @param keyVal The value of the key where I have to interpolate
   * @param comp The comparison operator (defaulted to less<double>{}).
   * @return
   */
  template <typename T, typename CompOper=std::less<double>>
      inline double interp1D(std::vector<double> const & keys, std::vector<T> const & values,
			     double const & keyVal, CompOper const & comp=std::less<double>{})
  {
    using namespace internals;
    map2Vectors<T> map(keys,values);
    return interp1D
	(
	map.cbegin(),
	map.cend(),
	keyVal,
	extractKey_map2Vectors<T>{map},
	extractValue_map2Vectors<T>{map},
	comp
	);
  }
}


#endif /* EXAMPLES_SRC_INTERP1D_INTERP1D_UTIL_HPP_ */
