#ifndef _MYMAT0__HH
#define _MYMAT0__HH
#include <iostream> 
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <stdexcept>
/*! MyMat0.

  @file MyMat0.hpp
  @author Luca Formaggia
  @version alpha
  This version uses more advanced features, some of them only c++11
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
  typedef std::vector<double>::size_type size_type;
  
  //! A simple template matrix class of double.
  /*!
    It stores a matrix of double entries, allowing different type of
    storage through an internal policy. The policy is implemented via
    a function selected by the template argument.
    The template argument may take only two values: ROWMAJOR and COLUMNMAJOR
    so `MyMat0<ROWMAJOR> a;` defines a matrix whose entries will be stores in 
    row wise order (which is the default).
   */
  template<StoragePolicySwitch T=ROWMAJOR>
  class MyMat0{
  private:
    size_type nr,nc;
    //! Data storage
    /*!
     * We store data in a vector. This is not the absolute best for
     * efficiency, but it makes the code safer (resource will be
     * released automatically when a MyMat0 object expires).
     * In alternative I might have used a smart pointer, in particular
     * std::unique_ptr<double>
     */
    std::vector<double> data;
    //! Function to map \f$(i,j)\f$ into the correct position in the data vector.
    /*!
      It implements the row or column major strategy. In fact the definition 
      of this function consists of two total specializations, one for T=ROWMAJOR 
      and one for T=COLUMNMAJOR.
      The advantage with respect to the solution with a pointer to function is that now the
      method can be inlined, with greater efficiency if it is called many times (as it normally
      would be...).
     */
    inline size_type getIndex (size_type const , 
			       size_type const ) const;
  public:
    //! It builds a matrix with n rows and m columns.
    /*!
      Since we give default value for the parameters, this
      constructor acts also as default constructor, in which case
      the matrix is initialised to zero.  *We make it explicit because*
      otherwise we would have an implicit conversion from int!*
      @param n number of rows.
      @param m number of columns.
    */
    explicit MyMat0(size_type n=0, size_type m=0); 
    //! Use default copy constructor
    MyMat0(MyMat0<T> const & m)=default; // C++11 extension
    //! Another copy constructor.
    /*!
      It accepts also a matrix with different storage order.
     */
    template <StoragePolicySwitch S>
    MyMat0(MyMat0<S> const & m);
#ifndef NOMOVE
#warning "Move constructor active"
    //! Move copy constructor (only if matrix is of the same type)
    MyMat0(MyMat0<T>&& m);
#endif
    //! Copy assignment operator
    /*!
      The synthetic one is enough.
     */
    MyMat0<T> & operator =(MyMat0<T> const & m)=default;
    //! Copy assignement operator
    /*!
      It accepts a matrix with different storage order.
    */
    template <StoragePolicySwitch S>
    MyMat0<T> & operator =(MyMat0<S> const & m);
#ifndef NOMOVE
#warning "Move copyassignement active"
    //! Copy assignement with move semantic. Only for matrices of same type.
    MyMat0<T> & operator =(MyMat0<T>&& m);
#endif
    //! Resizing the matrix.
    /*!
     * The storage policy cannot be changed;
     * If nrow*ncol are equal to that of the existing matrix
     * then the values are kept, otherwise they are set to zero
     */
    void resize(size_type const nrow, size_type const ncol);
    //! Number of rows.
    size_type nrow()const {return nr;};
    //! Number of columns.
    size_type ncol()const {return nc;};
    //! Returns element with no bound check (const version).
    /*!
      It allows a=m(1,1) on constant matrix m.
    */
    double operator () (const size_type i, const size_type j) const
    {
      return data[(this->getIndex)(i,j)];
    }
    //! Returns element with no bound check (non-const version)
    /*!
      It allows m(1,1)=1 on non-constant matrix m
    */
    double & operator () (const size_type i, const size_type j)
    {
      return data[(this->getIndex)(i,j)];
    }
    //! Return element with bound check (for debugging)
    double getValue(const size_type i, const size_type j) const;
    //! Sets element with bound check (for debugging)
    /*!
     * @param i row index
     * @param j column index
     * @param val The value to set
     */
    void setValue(const size_type i, const size_type j, double const & val);
    
    //! Get storage policy
    inline StoragePolicySwitch getStoragePolicy()const {
      return T;}
    //! Computes \f$ ||A||_\infty \f$
    const double normInf() const;
    //! Computes \f$ ||A||_1 \f$
    const double norm1() const;
    //! Computes Frobenious norm
    const double normF() const;
    //! An example of matrix times vector
    /*!
     * It checks for consistency: the size of the vector must be equal
     * to the number of columns
     */
    void vecMultiply(const std::vector<double> &v, std::vector<double> & res) const;
    
    //! Generates a random matrix
    /*!
     * It fills the matrix with random numbers in [0,1)
     *
     * @param seed Sets the seed value to initiate the pseudorandom generator.
     * If it is equal to 0 (default) the seed is set using the current time.
     * Otherwise the given value is used.
     * We will make use of the new random functions provided with c++11
     */
    void fillRandom(unsigned int seed=0);
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
  template<StoragePolicySwitch T>
  std::vector<double> operator * (MyMat0<T> const & m,std::vector<double> const & v);
}
  
  //***************** IMPLEMENTATIONS

namespace LinearAlgebra {
  template<StoragePolicySwitch T>
  MyMat0<T>::MyMat0(size_type n, size_type m):
    nr(n), nc(m), data(n*m,0.0){
  }

  //Type is the same
  //  template <StoragePolicySwitch T>
  //MyMat0<T>::MyMat0(MyMat0<T> const & m):nr(m.nr),nc(m.nc),data(m.data){
  //}

  //General case (different storage order since otherwise the non-templated version has precedence)
  template <StoragePolicySwitch T>
  template <StoragePolicySwitch S>
  MyMat0<T>::MyMat0(MyMat0<S> const & m):nr(m.nrow()),nc(m.ncol()){
    // Cannot access private data: the two types are different!
    data.resize(nr*nc);
    for (size_type i=0;i<nr;++i)
      for (size_type j=0;j<nc;++j)(*this)(i,j)=m(i,j);
  }

#ifndef NOMOVE
  // std containers implement move semantic, so I just move
  template <StoragePolicySwitch T>
  MyMat0<T>::MyMat0(MyMat0<T>&& m):nr(m.nr),nc(m.nc),data(std::move(m.data))
  {
    m.nr=0;m.nc=0;
  }
#endif

  // Copy assignment for matrices with different storage order
  // if S=T the non-template version defined in the class wins!
  template <StoragePolicySwitch T>
  template <StoragePolicySwitch S>
  MyMat0<T> & MyMat0<T>::operator =(MyMat0<S> const & m){
    // Cannot access private data: the two types are different!
    this->nr=m.nrow();
    this->nc=m.ncol();
    // Resize data
    std::vector<double>(nr*nc).swap(data);
    for (size_type i=0;i<nr;++i)
      for (size_type j=0;j<nc;++j)(*this)(i,j)=m(i,j);
  }
  
#ifndef NOMOVE
  template <StoragePolicySwitch T>
  MyMat0<T> & MyMat0<T>::operator =(MyMat0<T>&& m){
    // std containers implement move semantic, so I just move
    nr=std::move(m.nr);
    nc=std::move(m.nc);
    data=std::move(m.data);
    // I am not sure if move sets integers to zero so I do it myself
    m.nr=m.nc=0;
    return *this;
  }
#endif

  //! Specialized version for ROWMAJOR matrices
  template <>
  size_type MyMat0<ROWMAJOR>::getIndex(size_type const i, 
				       size_type const j) const{
    return j + i*nc;
  }
  
  //! Specialized version for COLUMNMAJOR matrices
  template <>
  size_type MyMat0<COLUMNMAJOR>::getIndex (size_type const i, 
					   size_type const j) const{
    return i + j*nr;
  }

  template<StoragePolicySwitch T>
  double  MyMat0<T>::getValue(size_type const i, size_type const j) const
  {
    // todo : this test should be hidden in a private method
    if  (i<0 || i>=nr || j<0 || i<=nc)
	throw std::out_of_range(" Out of bounds in MyMat0");
    return this->operator ()(i,j);
  }
  
  template<StoragePolicySwitch T>
  void  MyMat0<T>::setValue(size_type const i, size_type const j, double const & v)
  {
    if  (i<0 || i>=nr || j<0 || i<=nc)
	throw std::out_of_range(" Out of bounds in MyMat0");
    this->operator ()(i,j)=v;
  }
  
  
  template<StoragePolicySwitch T>
  void MyMat0<T>::resize(size_type const n, size_type const m)
  {
    if(n*m != nc*nr)
      {
	// clear data storage
	data.clear();
	// in c++11 we could use move semantic
	// but it is already implemented int the swap method
	std::vector<double>().swap(data);
	if(n*m != 0)
	    data.resize(n*m,0.0);
      }
    //! fix number of rows and column
    nr=n;
    nc=m;
  }
  
  template<StoragePolicySwitch T>
  const double MyMat0<T>::normInf() const{
    double vmax(0.0);
    if(nr*nc==0)return 0;
    
    for (size_type i=0;i<nr;++i){
      double vsum=0;
      for (size_type j=0;j<nc;++j) vsum+=std::abs(this->operator()(i,j));
      vmax=std::max(vsum,vmax);
    }
    return vmax;
  }
  
  template<StoragePolicySwitch T>
  const double MyMat0<T>::norm1() const{
    if(nr*nc==0)return 0;
    double vmax(0);
    for (size_type j=0;j<nc;++j){
      double vsum=0;
      for (size_type i=0;i<nr;++i) vsum+=std::abs(this->operator()(i,j));
      vmax=std::max(vsum,vmax);
    }
    return vmax;
  }
  
  template<StoragePolicySwitch T>
  const double MyMat0<T>::normF() const{
    if(nr*nc==0)return 0;
    double vsum=0;
    // Here an example of lambda usage!
    std::for_each(data.begin(),data.end(),[&](double const & v){vsum+=v*v;});
//    for (size_type i=0;i<nr*nc;++i) vsum+=data[i]*data[i];
    vsum=std::sqrt(vsum);
    return vsum;
  }
  
  //! Specialized version for COLUMNMAJOR matrices
  template<>
  void MyMat0<COLUMNMAJOR>::vecMultiply(const std::vector<double> &v, std::vector<double> & res) const
  {
    if(v.size() != nc){
      std::cerr<<" Vector must have the right size"<<std::endl;
      std::exit(1);
    }
    res.resize(nr,0);
    // result is a linear combination of the columns of A
    for (size_type i=0;i<nc*nr;++i) res[i % nr] += data[i]*v[i / nr];
  }
  
  //! Specialized version for ROWMAJOR matrices
  template<>
  void MyMat0<ROWMAJOR>::vecMultiply(const std::vector<double> &v, std::vector<double> & res) const
  {
    if(v.size() != nc){
      std::cerr<<" Vector must have the right size"<<std::endl;
      std::exit(1);
    }
    res.resize(nr,0);
    // Classic A*v row by row
    for (size_type i=0;i<nc*nr;++i) res[i / nc] += data[i]*v[i % nc];
  }
  
  template<StoragePolicySwitch T>
  void  MyMat0<T>:: fillRandom(unsigned int seed)
  {
    // If I do not give the sees I initialize the
    // sequence using the random device. Every call
    // will produce a different matrix
    if(seed==0){
      std::random_device rd;
      seed=rd();
    }
    // I use the standard engine for random numbers
    // Not the best one but simple and effective
    std::default_random_engine dre(seed);    
    // A uniform distribution between 0 and 1
    std::uniform_real_distribution<double> ur(0.,1.);
    // Generate random numbers
    for (auto & i : data) i=ur(dre);
  }
  
  
  template<StoragePolicySwitch T>
  void MyMat0<T>::showMe(std::ostream & out) const{
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
  
  template<StoragePolicySwitch T>
  std::vector<double> operator * (MyMat0<T> const & m,std::vector<double> const & v){
    std::vector<double> tmp;
    m.vecMultiply(v,tmp);
    return tmp;
  }

 } // End of Namespace

#endif
