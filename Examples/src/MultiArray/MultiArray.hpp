#ifndef HH_MULTI_ARRAY_HPP
#define HH_MULTI_ARRAY_HPP
#include <array>
#include <memory>
#include "extendedAssert.hpp"

namespace LinearAlgebra
{

  /*!
    Defines an array with arbitrary number of dimensions
    \param T  the stored type
    \param N  the number of dimensions
   */
template<typename T, int N>
class MultiArray
{
public:

  //  using ptr_Type=T*;
  //! Where we store the data
  using ptr_Type=std::unique_ptr<T[]>;

  MultiArray ();
  //! Gives the dimensions.
  template<typename ...Ints>
  MultiArray (Ints... dims);
  MultiArray (const MultiArray& source);
  MultiArray (MultiArray&& source)=default;
  virtual ~MultiArray ()=default;

  MultiArray<T,N>& operator= (const MultiArray<T,N>& rhs);
  MultiArray<T,N>& operator= (MultiArray<T,N>&& rhs)=default;

  template<typename... Ints>
  const T& operator () (Ints... indices) const
  {
    static_assert(sizeof...(indices)==N, "Error! Accesing the multi array with too many indices.\n");
    int i = index1d(indices...);
    return M_values[i];
  }

  template<typename... Ints>
  T& operator () (Ints... indices)
  {
    static_assert(sizeof...(indices)==N, "Error! Accesing the multi array with too many indices.\n");
    int i = Helper<0>::index1d(M_strides,M_dims,indices...);
    return M_values[i];
  }

  template<typename... Ints>
  void resize(Ints... dims);

private:

  template<int D>
  struct Helper
  {
    static int index1d (const std::array<int,N>& strides, const std::array<int,N>& dims, int i)
    {
      static_assert (D==N-1, "Error! Something is wrong with 1d-indexing in MultiArray.\n");
      ASSERTM (i>=0 && i<dims[D], "Error! Index out of bounds.\n");
      return strides[i];
    }

    template<typename... Ints>
    static int index1d (const std::array<int,N>& strides, const std::array<int,N>& dims, int i, Ints... tail)
    {
      ASSERTM (i>=0 && i<dims[D], "Error! Index out of bounds.\n");
      return strides[D]*Helper<D+1>::index1d(strides,dims,tail...);
    }

    static void computeStridesAndDims (std::array<int,N>& strides, std::array<int,N>& dims, int dim)
    {
      static_assert (D==N-1, "Error! Something is wrong with the calculation of the strides and dimensions.\n");

      strides[N-1] = 1;
      dims[N-1] = dim;
    }

    template<typename... Ints>
    static void computeStridesAndDims (std::array<int,N>& strides, std::array<int,N>& dims, int dim, Ints... tail)
    {
      Helper<D+1>::computeStridesAndDims (strides,dims,tail...);

      strides[D] = strides[D+1]*dims[D+1];
      dims[D] = dim;
    }
  };

  template<int CurrentDim>
  void computeStridesAndDims (int dim);
  template<typename... Ints, int CurrentDim>
  void computeStridesAndDims (int dim, Ints... tail);

  int                 M_length1d;
  ptr_Type                  M_values;
  std::array<int,N>   M_strides;
  std::array<int,N>   M_dims;
};

// ================================ IMPLEMENTATION ================================ //

template<typename T, int N>
MultiArray<T,N>::MultiArray ():M_values(),M_length1d(0)
{
  static_assert (N>0, "Error! Cannot create a 0-dimensional array.\n");
  //M_values = nullptr;
  //M_length1d = 0;
}

template<typename T, int N>
template<typename ...Ints>
MultiArray<T,N>::MultiArray (Ints... dims)
{
  static_assert (sizeof...(dims)==N, "Error! Template argument does not match pack length.\n");

  Helper<0>::computeStridesAndDims (M_strides,M_dims,dims...);

  M_length1d = 1;
  for (int i(0); i<N; ++i)
    M_length1d *= M_dims[i];

  M_values = std::unique_ptr<T[]>(new T[M_length1d]);
}

template<typename T, int N>
MultiArray<T,N>::MultiArray (const MultiArray& source):
  M_length1d(source.M_length1d),
  M_values(std::unique_ptr<T[]>(new T[M_length1d])),
  M_strides(source.M_strides),
  M_dims(source.M_dims)
{
  std::copy (source.M_values.get(), source.M_values.get()+M_length1d, M_values.get());
}


template<typename T, int N>
MultiArray<T,N>& MultiArray<T,N>::operator= (const MultiArray<T,N>& rhs)
{
  if (&rhs==this)
    return *this;

  M_length1d = rhs.M_length1d;
  M_strides  = rhs.M_strides;
  M_dims     = rhs.M_dims;

  M_values.reset(new T[M_length1d]);
  std::copy (rhs.M_values.get(), rhs.M_values.get()+M_length1d, M_values.get());
}


template<typename T, int N>
template<typename... Ints>
void MultiArray<T,N>::resize(Ints... dims)
{
  Helper<0>::computeStridesAndDims (M_strides,M_dims,dims...);
  M_length1d = 1;
  for (int i(0); i<N; ++i)
    M_length1d *= M_dims[i];
  M_values.release();
}

} // Namespace LinearAlgebra

#endif // LIFEV_MULTI_ARRAY_HPP
