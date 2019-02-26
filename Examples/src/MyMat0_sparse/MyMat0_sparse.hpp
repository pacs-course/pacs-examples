/*! MyMat0 A simple class for an INEFFICIENT sparse matrix
    Luca Formaggia 2005     */
#ifndef _MYMAT0_SPARSE_HH
#define _MYMAT0_SPARSE_HH
#include <iostream> 
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <map>
#include <array>
#include <type_traits>
#include <functional>
/*!
  @file MyMat0Sparse.hpp
  @author Luca Formaggia
  @version alpha
 */

//! All functionalities are in this namespace
namespace LinearAlgebra{
//! Sets the policy for data storage.
/*!
 *  ROWMAJOR = Matrix is stored by rows
 *  COLUMNMAJOR = Matrix is stored by columns
 */
  enum StoragePolicySwitch {ROWMAJOR,COLUMNMAJOR};

  //! Type to hold indexex.
  /*!
    The standard library gives me one, so we use it
    size_type is an integral type that is guaranteed to be
    valid to store indices to arrays and vector. It is convertible
    to int, so don't worry, int would (probably) work as fine, but
    using size_type we are sure that we are consistent with the standard
    library (and avoid problem with particular computer architectures)
   */


  namespace HelpersSparse
  {
  //! A Helper class that allow to distinguish row and column ordering
  /*! It ia a nice trick by Alexandrescu. I convert an enumerator to a type
    so that I can apply overloading (its called type tag)
  */
    template<StoragePolicySwitch storagePolicy>
    struct storagePolicyType{};
    //! A helper class to distinguish true from false
    template<bool T>
    struct fillSwitch{};

    using size_type=std::vector<double>::size_type;
    using Indices=std::array<2,size_type>; // @suppress("Invalid template argument")
    using orderingFunction=std::function<bool (const Indices & a, const Indices & b)> ; // @suppress("Invalid template argument")


    orderingFunction rowMajorOrdering=
      [](const Indices & a, const Indices & b)
    {
      return a[0]==b[0]?a[1]<b[1]:a[0]<b[0];
    };
    
    orderingFunction columnMajorOrdering=
			     [](const Indices & a, const Indices & b)
    {
      return a[1]==b[1]?a[0]<b[0]:a[1]<b[1];
    };
    
    template <StoragePolicySwitch storagePolicy> theOrderingFunction
    {
    };
    template <> theOrderingFunction<ROWMAJOR>
    {
      static const orderingFunction;
    };
    theOrderingFunction<ROWMAJOR>::
    template <> theOrderingFunction<COLUMNMAJOR>;
    {
      static const orderingFunction;
    };

  }
  //! A simple sparse matrix class
  /*!
   * Very inefficient, but rather general.
   */
  template<class T=double, StoragePolicySwitch storagePolicy=ROWMAJOR>
  class MyMat0Sparse{
  public:
    typedef HelpersSparse::size_type size_type;
  private:
    HelpersSparse::
    size_type nr,nc;
    //! Data storage
    /*!
     * We store data in a vector. This is not the absolute best for
     * efficiency, but it makes the code safer (resource will be
     * released automatically when a MyMat0Sparse object expires).
     * In alternative I might have used a smart pointer, in particular
     * std::unique_ptr<double>
     */
    std::map<Indices,T> data;
      //! The general template for the policies. Only declaration since I will always use full specializations
    //    template<StoragePolicySwitch thePolicy> size_type M_getIndex(size_type const & i, size_type const & j) const;
    /*! This does not work since partial specialization is not possible
      size_type getIndex(size_type const & i, size_type const & j) const;

      I use enable_if to select the correct one 
    */
    //! The actual function returning the index
    Indices getIndex(size_type const & i, size_type const & j) const
    {
      return Indices{i,j};
    }
  public:
    //! Returns the type of the stored values
    typedef T value_type;
    //! It builds a matrix with n rows and m columns.
    /*!
      Since we give default value for the parameters, this
      constructor acts also as default constructor, in which case
      the matix is initialised to zero.  We make it explicit because 
      otherwise we would have an implicit conversion from int!
     
      @param n number of rows.
      @param m number of columns.
      @param sPolicy the sorage policy (default rowmajor).
     */
    explicit MyMat0Sparse(size_type n=0, size_type m=0): nr(n), nc(m), data(n*m)
    {
      // Set to zero if arithmetic
      fillZero();
    };
    //! Default copy constructor is ok
    MyMat0Sparse(const MyMat0Sparse&)=default;
    //! Default move constructor is ok
    MyMat0Sparse(MyMat0Sparse&&)=default;
    //! Default move assign is ok
    MyMat0Sparse& operator=(MyMat0Sparse&&)=default;
    //! Fills of zero. Enabled only for arithmetic types
    void fillZero(HelpersSparse::fillSwitch<true>)
    {
      for (auto & i : data) i=T(0);
    }
    void fillZero(HelpersSparse::fillSwitch<false>){}
    //! Fills with zero if arithmetic type or pointer type.
    void fillZero()
    {
      fillZero(HelpersSparse::fillSwitch<std::is_arithmetic<T>::value||std::is_pointer<T>::value>());      
    }
    //! Resizing the matrix
    /*!
     * The storage policy cannot be changed;
     * If nrow*ncol are equal to that of the existing matrix
     * then the values are kept, otherwise they are set to zero
     */
    void resize(size_type const nrow, size_type const ncol);
    //! Number of rows
    size_type nrow()const {return nr;};
    //! Number of columns
    size_type ncol()const {return nc;};
    //! Returns element with no bound check (const version)
    /*!
      It allows a=m(1,1) on constant matrix m
     */
    T operator () (const size_type i, const size_type j) const
    {
    	return data[getIndex(i,j)];
    }
    //! Returns element with no bound check (non-const version)
    /*!
      It allows m(1,1)=1 on non-constant matrix m
     */
    T & operator () (const size_type i, const size_type j)
    {
    	return data[getIndex(i,j)];
    }
    //! Policy cannot be changed, but it may be queried
    constexpr StoragePolicySwitch getStoragePolicy()const 
    {
      return storagePolicy;
    }
    //! Computes \f$ ||A||_\infty \f$
    /*!
      This method makes sense only for arithmetic types. 
      I use static assert in the definition to make it work only for arithmetic types.
      \code
      T normInf()
      {
      static_assert(std::is_arithmetic<T>::value,"NormInf can be called only on arithmetic types");
      ...
      }
      \endcode

      As an alternative, I can use enable_if
      \code
      template <class T1=T>
      typename std::enable_if<std::is_arithmetic<T1>::value, T1>::type normInf() const;
      \endcode

      In this case the method is not enabled at all if the condition (arithmetic type) is not satisfied.
     */
    T normInf() const;
    //! Computes \f$ ||A||_1 \f$
    /*!
      To enable it only for arithmetic types I use here a trick that involves enable_if. 
      But to make SFINAE work I need
      to make the method a template method. But I give a default value, so in practice I can use
      it as it were a non-template method
    */

    template <class T1=T>
    typename std::enable_if<std::is_arithmetic<T1>::value, T1>::type norm1() const;
    //! Computes Frobenious norm
    /*
      Cannot return an integer type (in case T is an integer). A simple, yet not so nice
      Alternative: use conditional
      /code
      template <class T1=T>
      typename std::enable_if<std::is_arithmetic<T1>::value, 
      typename std::conditional<std::is_integer<T1>::value,double,T1>::type
      >::type normF() const;
      /endcode
     */
    template <class T1=T>
    typename std::enable_if<std::is_arithmetic<T1>::value, long double>::type normF() const;
    //! An example of matrix times vector
    /*!
     * It checks for consistency: the size of the vector must be equal
     * to the number of columns
     */
    void vecMultiply(const std::vector<T> &v, std::vector<T> & res) const;

    //! Generates a random matrix
    /*!
     * It fills the matrix with random numbers in [0,1)
     *
     * @param seed Sets the seed value to initiate the pseudorandom generator.
     * If it is equal to 0 (default) the seed is set using the current time.
     * Otherwise the given value is used.
     * ONly for arithmetic types
     */
    template <class T1=T>
    typename std::enable_if<std::is_arithmetic<T1>::value, void>::type  fillRandom(unsigned int seed=0);
    //! It shows matrix content
    /*!
     *  It pretty prints the matrix
     *  @todo replace with operator <<
     *  @param out The output stream (defaults to cout)
     */
    void showMe(std::ostream  & out=std::cout) const;
  };
  //! Matrix times vector via operator *
  /*!
   * @param m The matrix
   * @param v The vector (with size equal to the number of columns)
   * @return The result in a vector of the size = to the number of row
   */
  template<class T, StoragePolicySwitch storagePolicy>
  std::vector<T> operator * (MyMat0Sparse<T, storagePolicy> const & m,std::vector<T> const & v);

  //                 DEFINITIONS
  
  /*  THIS  DOES NOT WORK: PARTIAL SPECIALIZATION OF METHODS IS NOT ALLOWED */
  //! Specialization for row major ordering
  /*!
   Note: important to declare it inline. Otherwise it should go to a cpp file! And it will be less
   efficient!
  */
  /*
    template<class T>
  inline size_type MyMat0Sparse<T,ROWMAJOR>::getIndex(size_type const & i, size_type const & j) const
  {
    return i + j*nr;
    }*/
  //! Specialization for column major ordering
  /*!
    Note: important to declare it inline. Otherwise it should go to a cpp file! And it will be less
    efficient!
  */
  /*  template<class T>
  inline size_type MyMat0Sparse<T,COLUMNMAJOR>::getIndex(size_type const & i, size_type const & j) const
  {
  return j + i*nc;
  }
  */

  template<class T, StoragePolicySwitch storagePolicy>
  void MyMat0Sparse<T,storagePolicy>::resize(size_type const n, size_type const m)
  {
    if(n*m != nc*nr)
      {
	// clear data storage
	data.resize(n*m);
	data.shrink_to_fit();
	fillZero();
      }
    //! fix number of rows and column
    nr=n;
    nc=m;
  }
    
    /*
      template<class T, StoragePolicySwitch storagePolicy>
      template<class T1>
      typename std::enable_if<std::is_arithmetic<T1>::value, T1>::type 
      MyMat0Sparse<T,storagePolicy>::normInf() const
      {
      if(nr*nc==0)return 0;
      T vmax(0.0);
      
      for (size_type i=0;i<nr;++i){
      T vsum=0;
      for (size_type j=0;j<nc;++j) vsum+=data[getIndex(i,j)];
      vmax=std::max(vsum,vmax);
      }
      return vmax;
      }
    */
    template<class T, StoragePolicySwitch storagePolicy>
    T MyMat0Sparse<T,storagePolicy>::normInf() const
    {
    static_assert(std::is_arithmetic<T>::value," normInf can be used only on arithmetic types"); 
    if(nr*nc==0)return 0;
    T vmax(0.0);
    
    for (size_type i=0;i<nr;++i){
      T vsum=0;
      for (size_type j=0;j<nc;++j) vsum+=data[getIndex(i,j)];
      vmax=std::max(vsum,vmax);
    }
    return vmax;
  }

  /* Selection by enable_if */
  template<class T, StoragePolicySwitch storagePolicy>
  template<class T1>
  typename std::enable_if<std::is_arithmetic<T1>::value, T1>::type 
  MyMat0Sparse<T,storagePolicy>::norm1() const{
    if(nr*nc==0)return 0;
    T vmax(0);
    for (size_type j=0;j<nc;++j){
      T vsum=0;
      for (size_type i=0;i<nr;++i) vsum+=data[getIndex(i,j)];
      vmax=std::max(vsum,vmax);
    }
    return vmax;
  }
  

  template<class T, StoragePolicySwitch storagePolicy>
  template<class T1>
  typename std::enable_if<std::is_arithmetic<T1>::value, long double>::type
  MyMat0Sparse<T,storagePolicy>::normF() const{
    if(nr*nc==0)return 0.0;
    long double vsum=0;
    for (auto const x: data) vsum+=x*x;
    return std::sqrt(vsum);
  }
  
  
  template<class T, StoragePolicySwitch storagePolicy>
  void MyMat0Sparse<T, storagePolicy>::vecMultiply(const std::vector<T> &v, std::vector<T> & res) const
  {
    if(v.size() != nc)
      {
	std::cerr<<" Vector must have the right size"<<std::endl;
	std::exit(1);
      }
    res.resize(nr,T(0));
    // for efficiency I use two different algorithms
    // I could have used overloading!
    switch(storagePolicy)
      {
      case ROWMAJOR:
	// Classic A*v row by row
	for (size_type i=0;i<nc*nr;++i) res[i / nc] += data[i]*v[i % nc];
	break;
      case COLUMNMAJOR:
	// result is a linear combination of the columns of A
	for (size_type i=0;i<nc*nr;++i) res[i % nr] += data[i]*v[i / nr];
	break;
      }
  }
  
  template<class T, StoragePolicySwitch storagePolicy>
  template<class T1>
  typename std::enable_if<std::is_arithmetic<T1>::value, void>::type 
  MyMat0Sparse<T, storagePolicy>:: fillRandom(unsigned int seed)
  {
    if (seed==0) seed=std::time(0);
    double rmax=static_cast<double>(RAND_MAX+2.0);
    std::srand(seed);
    if(nr*nc>0)
      for (auto& x: data) x=static_cast<T>(std::rand()+1)/rmax;
  }
  
  
  template<class T, StoragePolicySwitch storagePolicy>
  void MyMat0Sparse<T, storagePolicy>::showMe(std::ostream & out) const{
    out<<"[";
    for (size_type i=0;i<nr;++i){
      for (size_type j=0;j<nc-1;++j) out<< this->operator()(i,j)<<", ";
      out<< this->operator()(i,nc-1);
      if(i==nr-1)
	out<<"]"<<std::endl;
      else
	out<<std::endl;
    }
  }

  template<class T, StoragePolicySwitch storagePolicy>
  std::vector<T> operator * (MyMat0Sparse<T, storagePolicy> const & m,std::vector<T> const & v)
  {
    std::vector<T> tmp;
    m.vecMultiply(v,tmp);
    return tmp;
  }

}


#endif
