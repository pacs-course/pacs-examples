/*! MyMat0 A simple class for full matrix
    Luca Formaggia 2005     */
#ifndef HH_MYMAT0__HH
#define HH_MYMAT0__HH
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <vector>
/*!
  @file MyMat0.hpp
  @author Luca Formaggia
  @version alpha
 */

//! All functionalities are in this namespace
namespace LinearAlgebra
{
//! Sets the policy for data storage.
/*!
 *  ROWMAJOR = Matrix is stored by rows
 *  COLUMNMAJOR = Matrix is stored by columns
 */
enum StoragePolicySwitch
{
  ROWMAJOR,
  COLUMNMAJOR
};

//! Converts a storage-policy value into a type.
/*!
  This alias is used for tag dispatching so that overload resolution can select
  the most suitable implementation for a given storage layout at compile time.
 */
template <StoragePolicySwitch S>
using StorageType = std::integral_constant<StoragePolicySwitch, S>;

using size_type = std::size_t;

//! A simple dense matrix class.
/*!
 * The matrix stores its entries in a flat `std::vector<T>`. The storage order
 * is selected at compile time through the `storagePolicy` template parameter.
 *
 * @tparam T Scalar type stored in the matrix.
 * @tparam storagePolicy Storage layout used internally.
 */
template <class T = double, StoragePolicySwitch storagePolicy = ROWMAJOR>
class MyMat0
{
public:
  //! Type to hold indexex.
  /*!
    The standard library gives me one, so we use it std::size_t is
    an integral type that is guaranteed to be valid to store indices
    to arrays and vector. It is convertible to int, so don't worry,
    unsigned int would (probably) work as fine, but using size_type we
    are sure that we are consistent with the standard library (and
    avoid problem with particular computer architectures).
    We use and alias so it is easy to change if needed.
   */
  using size_type = std::size_t;
  //! I expose the type of the elements
  using value_type = T;

private:
  size_type nr, nc;
  //! Data storage
  /*!
   * We store data in a vector. This is not the absolute best for
   * efficiency, but it makes the code safer (resource will be
   * released automatically when a MyMat0 object expires).
   * In alternative I might have used a smart pointer, in particular
   * std::unique_ptr<T>.
   */
  std::vector<T> data;
  //! The other storage system
  static constexpr StoragePolicySwitch otherPolicy =
    storagePolicy == ROWMAJOR ? COLUMNMAJOR : ROWMAJOR;
  /*!
    \defgroup getIndex Functions returning the linearized index
    @{
   */
  //! Linearizes a matrix index for row-major storage.
  constexpr size_type
  getIndex(size_type const i, size_type const j, StorageType<ROWMAJOR>) const
  {
    return j + i * nc;
  }
  //! Linearizes a matrix index for column-major storage.
  constexpr size_type
  getIndex(size_type const i, size_type const j, StorageType<COLUMNMAJOR>) const
  {
    return i + j * nr;
  }
  /*!
    @}
  */

  /*!
    \defgroup extractors-inserters Functions extracting or replacing rows/columns
    These overloads are selected by tag dispatching so the implementation can
    follow the matrix storage order.
    @{
  */
  //! Extracts a column from a row-major matrix.
  std::vector<T> col(size_type i, StorageType<ROWMAJOR>) const;
  //! Extracts a column from a column-major matrix.
  std::vector<T> col(size_type i, StorageType<COLUMNMAJOR>) const;
  //! Extracts a row from a row-major matrix.
  std::vector<T> row(size_type i, StorageType<ROWMAJOR>) const;
  //! Extracts a row from a column-major matrix.
  std::vector<T> row(size_type i, StorageType<COLUMNMAJOR>) const;
  //! Replaces a column in a row-major matrix.
  void replaceCol(size_type i, std::vector<T> const &, StorageType<ROWMAJOR>);
  //! Replaces a column in a column-major matrix.
  void replaceCol(size_type i, std::vector<T> const &,
                  StorageType<COLUMNMAJOR>);
  //! Replaces a row in a row-major matrix.
  void replaceRow(size_type i, std::vector<T> const &, StorageType<ROWMAJOR>);
  //! Replaces a row in a column-major matrix.
  void replaceRow(size_type i, std::vector<T> const &,
                  StorageType<COLUMNMAJOR>);
  /*!
    @}
  */
public:
  //! Returns the offset in the flat storage corresponding to `(i,j)`.
  /*!
    This function is public so that helper algorithms can access the linearized
    indexing logic without duplicating it.
   */
  constexpr size_type
  getIndex(size_type const i, size_type const j) const
  {
    return getIndex(i, j, StorageType<storagePolicy>{});
  }
  //! It builds a matrix with n rows and m columns.
  /*!
    Since we give default value for the parameters, this
    constructor acts also as default constructor, in which case
    the matix is initialised to zero.  We make it explicit because
    otherwise we would have an implicit conversion from int!

    @param n number of rows.
    @param m number of columns.
    @param init value used to initialize all entries.
   */
  explicit MyMat0(size_type n = 0, size_type m = 0, T const &init = T())
    : nr(n), nc(m), data(n * m, init)
  {}
  //! Default copy constructor.
  MyMat0(MyMat0 const &) = default;
  //! Builds a matrix with the same entries but opposite storage order.
  /*!
    @param m Source matrix stored with the other policy.
   */
  MyMat0(MyMat0<T, otherPolicy> const &m);
  //! Default move constructor is ok
  MyMat0(MyMat0 &&) = default;
  //! Default copy assign is ok
  MyMat0 &operator=(MyMat0 const &) = default;
  //! Assigns from a matrix with the opposite storage order.
  /*!
    The matrix is resized if needed and the entries are copied preserving the
    logical matrix layout.
    @param m Source matrix stored with the other policy.
    @return `*this`
   */
  MyMat0 &operator=(MyMat0<T, otherPolicy> const &);
  //! Default move assign is ok
  MyMat0 &operator=(MyMat0 &&) = default;
  //! Resizing the matrix
  /*!
   * The storage policy cannot be changed;
   * If nrow*ncol are equal to that of the existing matrix
   * then the values are kept, otherwise they are set to zero
   * @param nrow New number of rows.
   * @param ncol New number of columns.
   */
  void resize(size_type const nrow, size_type const ncol);
  //! Number of rows
  size_type
  nrow() const
  {
    return nr;
  };
  //! Number of columns
  size_type
  ncol() const
  {
    return nc;
  };
  //! Returns the ith column
  /*!
    I use tagging technique to implement the best method according
    to the storage policy
    @param i Column index.
    @return A copy of the selected column.
   */
  std::vector<T>
  col(size_type i) const
  {
    return col(i, StorageType<storagePolicy>{});
  }
  //! Returns the ith row
  /*!
    I use tagging technique to implement the best method according
    to the storage policy
    @param i Row index.
    @return A copy of the selected row.
   */
  std::vector<T>
  row(size_type i) const
  {
    return row(i, StorageType<storagePolicy>{});
  }
  //! Replaces the ith column
  /*!
    I use tagging technique to implement the best method according
    to the storage policy
    @param i Column index.
    @param v Values to be copied into the column.
    @throws std::invalid_argument if `v.size()!=nrow()`.
   */
  void
  replaceCol(size_type i, std::vector<T> const &v)
  {
    replaceCol(i, v, StorageType<storagePolicy>{});
  }
  //! Replaces the ith row
  /*!
    I use tagging technique to implement the best method according
    to the storage policy
    @param i Row index.
    @param v Values to be copied into the row.
    @throws std::invalid_argument if `v.size()!=ncol()`.
   */
  void
  replaceRow(size_type i, std::vector<T> const &v)
  {
    replaceRow(i, v, StorageType<storagePolicy>{});
  }
  //! Returns element with no bound check (const version)
  /*!
    It allows a=m(1,1) on constant matrix m
    @param i Row index.
    @param j Column index.
    @return Read-only reference to the selected entry.
    @warning No bounds checking is performed.
   */
  T const &
  operator()(const size_type i, const size_type j) const
  {
    return data[getIndex(i, j)];
  }

  //! Returns element with no bound check (non-const version)
  /*!
    It allows m(1,1)=1 on non-constant matrix m
    @param i Row index.
    @param j Column index.
    @return Reference to the selected entry.
    @warning No bounds checking is performed.
   */
  T &
  operator()(const size_type i, const size_type j)
  {
    return data[getIndex(i, j)];
  }
// Activate this part only if compiler with std=c++23
#if __cplusplus >= 202100L
  //! Returns element with no bound check (const version)
  /*!
    It allows `a = m[i,j]` on a constant matrix.
    @param i Row index.
    @param j Column index.
    @return Read-only reference to the selected entry.
    @warning No bounds checking is performed.
    @note This overload requires C++23 multidimensional subscripting.
  */
  T const &
  operator[](const size_type i, const size_type j) const
  {
    return data[getIndex(i, j)];
  }
  /*!
    It allows `m[i,j] = value` on a non-constant matrix.
    @param i Row index.
    @param j Column index.
    @return Reference to the selected entry.
    @warning No bounds checking is performed.
    @note This overload requires C++23 multidimensional subscripting.
  */
  T &
  operator[](const size_type i, const size_type j)
  {
    return data[getIndex(i, j)];
  }
#endif

  //! Policy cannot be changed, but it may be queried
  constexpr StoragePolicySwitch
  getStoragePolicy() const
  {
    return storagePolicy;
  }
  /*! \defgroup Norms Various matrix norms
    The return type is a double. However I can make it better
    by use of type traits.
    Need specialization for matrix of complex numbers.
    They make sense only if T has sensible arithmetic operators.
    @{
  */
  //! Gives direct access to the flat storage.
  /*!
    @param i Linearized entry index.
    @return Read-only reference to the `i`-th stored value.
    @warning No bounds checking is performed.
   */
  auto const &
  operator[](size_type i) const
  {
    return data[i];
  }
  //! Gives direct access to the flat storage.
  /*!
    @param i Linearized entry index.
    @return Reference to the `i`-th stored value.
    @warning No bounds checking is performed.
   */
  auto &
  operator[](size_type i)
  {
    return data[i];
  }

  //! Computes \f$ ||A||_\infty \f$
  double normInf() const;
  //! Computes \f$ ||A||_1 \f$
  double norm1() const;
  //! Computes the Frobenius norm.
  double normF() const;
  /*! @} */
  //! Generates a random matrix
  /*!
   * It fills the matrix with random numbers in [0,1)
   *
   * @param seed Sets the seed value to initiate the pseudorandom generator.
   * If it is equal to 0, a nondeterministic seed is requested from
   * `std::random_device`. Otherwise the given value is used.
   */
  void fillRandom(unsigned int seed = 0);
  /*! Multiplies the matrix by a vector.
    @param v Vector to be multiplied.
    @param res Vector where the result is stored.
    @throws std::invalid_argument if `v.size()!=ncol()`.
   */
  void vecMultiply(const std::vector<T> &v, std::vector<T> &res) const;
  //! Iterator to the begin of the internal structure
  /*!
    To be used for fast operation on the data
    @return Iterator to the first stored entry.
   */
  auto
  begin() -> decltype(data.begin())
  {
    return data.begin();
  }
  //! Iterator to the begin of the internal structure
  /*!
    To be used for fast operation on the data
    @return Constant iterator to the first stored entry.
   */
  auto
  cbegin() const -> decltype(data.cbegin())
  {
    return data.cbegin();
  }
  //! Iterator to the end of the internal structure
  /*!
    To be used for fast operation on the data
    @return Iterator past the last stored entry.
   */
  auto
  end() -> decltype(data.end())
  {
    return data.end();
  }
  //! Iterator to the end of the internal structure
  /*!
    To be used for fast operation on the data
    @return Constant iterator past the last stored entry.
   */
  auto
  cend() const -> decltype(data.cend())
  {
    return data.cend();
  }
  //! It shows matrix content
  /*!
   *  It pretty prints the matrix
   *  @todo replace with operator <<
   *  @param out The output stream (defaults to cout)
   */
  void showMe(std::ostream &out = std::cout) const;
};
//! Matrix times vector via operator *
/*!
 * @param m The matrix.
 * @param v Vector to be multiplied.
 * @return The product `m*v`.
 * @throws std::invalid_argument if `v.size()!=m.ncol()`.
 */
template <class T, StoragePolicySwitch storagePolicy>
std::vector<T> operator*(MyMat0<T, storagePolicy> const &m,
                         std::vector<T> const           &v);

//                 DEFINITIONS

template <class T, StoragePolicySwitch storagePolicy>
MyMat0<T, storagePolicy>::MyMat0(MyMat0<T, otherPolicy> const &m)
  : nr(m.nrow()), nc(m.ncol()), data(nr * nc)
{
  for(size_type i = 0; i < nr; ++i)
    for(size_type j = 0; j < nc; ++j)
      data[getIndex(i, j)] = m(i, j);
}

template <class T, StoragePolicySwitch storagePolicy>
MyMat0<T, storagePolicy> &
MyMat0<T, storagePolicy>::operator=(MyMat0<T, otherPolicy> const &m)
{
  this->resize(m.nr, m.nc);
  for(size_type i = 0; i < nr; ++i)
    for(size_type j = 0; j < nc; ++j)
      data[getIndex(i, j)] = m(i, j);
  return *this;
}

// This in case you want to implement the different access with specialization
// instead of type tag
//! Specialization for row major ordering
/*!
 Note: important to declare it inline. Otherwise it should go to a cpp file! And
 it will be less efficient!
*/
/*
  template<class T>
inline size_type MyMat0<T,ROWMAJOR>::getIndex(size_type const & i, size_type
const & j) const
{
return j + i*nc;
}
*/
//! Specialization for column major ordering
/*!
  Note: important to declare it inline. Otherwise it should go to a cpp file!
  And it will be less efficient!
*/
/*
template<class T>
inline size_type MyMat0<T,COLUMNMAJOR>::getIndex(size_type const & i, size_type
const & j) const
{
  return i + j*nr;
}
*/
template <class T, StoragePolicySwitch storagePolicy>
void
MyMat0<T, storagePolicy>::resize(size_type const n, size_type const m)
{
  if(n * m != nc * nr)
    {
      data.assign(n * m, T{});
    }
  //! fix number of rows and column
  nr = n;
  nc = m;
}

template <class T, StoragePolicySwitch storagePolicy>
double
MyMat0<T, storagePolicy>::normInf() const
{
  if(nr * nc == 0)
    return 0;
  double vmax(0.);

  for(size_type i = 0; i < nr; ++i)
    {
      double vsum = 0;
      for(size_type j = 0; j < nc; ++j)
        vsum += std::abs(static_cast<double>(data[getIndex(i, j)]));
      vmax = std::max(vsum, vmax);
    }
  return vmax;
}

template <class T, StoragePolicySwitch storagePolicy>
double
MyMat0<T, storagePolicy>::norm1() const
{
  if(nr * nc == 0)
    return 0;
  double vmax(0);
  for(size_type j = 0; j < nc; ++j)
    {
      double vsum = 0;
      for(size_type i = 0; i < nr; ++i)
        vsum += std::abs(static_cast<double>(data[getIndex(i, j)]));
      vmax = std::max(vsum, vmax);
    }
  return vmax;
}

template <class T, StoragePolicySwitch storagePolicy>
double
MyMat0<T, storagePolicy>::normF() const
{
  if(nr * nc == 0)
    return 0.0;
  double vsum{0.0};
  for(auto const x : data)
    {
      auto const value = static_cast<double>(x);
      vsum += value * value;
    }
  return std::sqrt(vsum);
}

template <class T, StoragePolicySwitch storagePolicy>
void
MyMat0<T, storagePolicy>::vecMultiply(const std::vector<T> &v,
                                      std::vector<T>       &res) const
{
  if(v.size() != nc)
    throw std::invalid_argument("Vector size must match the number of columns");
  res.assign(nr, T{});
  // for efficiency I use two different algorithms

  if constexpr(storagePolicy == ROWMAJOR)
    {
      // Classic A*v row by row
      for(size_type i = 0; i < nc * nr; ++i)
        res[i / nc] += data[i] * v[i % nc];
    }
  else
    {
      // result is a linear combination of the columns of A
      for(size_type i = 0; i < nc * nr; ++i)
        res[i % nr] += data[i] * v[i / nr];
    }
}

template <class T, StoragePolicySwitch storagePolicy>
void
MyMat0<T, storagePolicy>::fillRandom(unsigned int seed)
{
  auto generator =
    seed == 0 ? std::mt19937{std::random_device{}()} : std::mt19937{seed};
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  for(auto &x : data)
    x = static_cast<T>(distribution(generator));
}

template <class T, StoragePolicySwitch storagePolicy>
void
MyMat0<T, storagePolicy>::showMe(std::ostream &out) const
{
  if(nr == 0 || nc == 0)
    {
      out << "[]\n";
      return;
    }
  out << "[";
  for(size_type i = 0; i < nr; ++i)
    {
      for(size_type j = 0; j < nc - 1; ++j)
        out << this->operator()(i, j) << ", ";
      out << this->operator()(i, nc - 1);
      if(i == nr - 1)
        out << "]" << std::endl;
      else
        out << std::endl;
    }
}

template <class T, StoragePolicySwitch storagePolicy>
std::vector<T>
operator*(MyMat0<T, storagePolicy> const &m, std::vector<T> const &v)
{
  std::vector<T> tmp;
  m.vecMultiply(v, tmp);
  return tmp;
}

template <class T, StoragePolicySwitch storagePolicy>
std::vector<T>
MyMat0<T, storagePolicy>::col(size_type j, StorageType<ROWMAJOR>) const
{
  // Extracting a column from a matrix stored in row major ordering is not
  // efficient
  std::vector<T> res;
  res.reserve(nr);
  for(size_type i = 0; i < nr; ++i)
    res.emplace_back(data[getIndex(i, j)]);
  return res;
}

template <class T, StoragePolicySwitch storagePolicy>
std::vector<T>
MyMat0<T, storagePolicy>::col(size_type j, StorageType<COLUMNMAJOR>) const
{
  auto const first = data.cbegin() + getIndex(0, j);
  auto const last = first + static_cast<std::ptrdiff_t>(nr);
  return std::vector<T>(first, last);
}

template <class T, StoragePolicySwitch storagePolicy>
std::vector<T>
MyMat0<T, storagePolicy>::row(size_type i, StorageType<ROWMAJOR>) const
{
  auto const first = data.cbegin() + getIndex(i, 0);
  auto const last = first + static_cast<std::ptrdiff_t>(nc);
  return std::vector<T>(first, last);
}

template <class T, StoragePolicySwitch storagePolicy>
std::vector<T>
MyMat0<T, storagePolicy>::row(size_type i, StorageType<COLUMNMAJOR>) const
{
  // Extracting a row from a matrix stored in column major ordering is not
  // efficient
  std::vector<T> res;
  res.reserve(nc);
  for(size_type j = 0; j < nc; ++j)
    res.emplace_back(data[getIndex(i, j)]);
  return res;
}

template <class T, StoragePolicySwitch storagePolicy>
void
MyMat0<T, storagePolicy>::replaceCol(size_type j, std::vector<T> const &c,
                                     StorageType<ROWMAJOR>)
{
  if(c.size() != nr)
    throw std::invalid_argument("Column size mismatch");
  for(size_type i = 0; i < nr; ++i)
    data[getIndex(i, j)] = c[i];
}

template <class T, StoragePolicySwitch storagePolicy>
void
MyMat0<T, storagePolicy>::replaceCol(size_type j, std::vector<T> const &c,
                                     StorageType<COLUMNMAJOR>)
{
  if(c.size() != nr)
    throw std::invalid_argument("Column size mismatch");
  // I can trasverse the data in the natural order. The optimizer can exploit
  // cache pre-fetching
  auto start = data.begin() + getIndex(0, j);
  for(size_type i = 0; i < nr; ++i)
    *(start++) = c[i];
}

template <class T, StoragePolicySwitch storagePolicy>
void
MyMat0<T, storagePolicy>::replaceRow(size_type i, std::vector<T> const &c,
                                     StorageType<ROWMAJOR>)
{
  if(c.size() != nc)
    throw std::invalid_argument("Row size mismatch");
  // I can trasverse the data in the natural order. The optimizer can exploit
  // cache pre-fetching
  auto start = data.begin() + getIndex(i, 0);
  for(size_type j = 0; j < nc; ++j)
    *(start++) = c[j];
}

template <class T, StoragePolicySwitch storagePolicy>
void
MyMat0<T, storagePolicy>::replaceRow(size_type i, std::vector<T> const &c,
                                     StorageType<COLUMNMAJOR>)
{
  if(c.size() != nc)
    throw std::invalid_argument("Row size mismatch");
  for(size_type j = 0; j < nc; ++j)
    data[getIndex(i, j)] = c[j];
}

} // namespace LinearAlgebra

#endif
