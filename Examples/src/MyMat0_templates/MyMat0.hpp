/*! MyMat0 A simple class for full matrix
    Luca Formaggia 2005     */
#ifndef _MYMAT0__HH
#define _MYMAT0__HH
#include <iostream> 
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
/*!
  @file MyMat0.hpp
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
  typedef std::vector<double>::size_type size_type;

  //! A simple matrix class of double
  /*!
   * It stores a matrix of double entries, allowing different type of
   * storage through an internal policy. The policy is implemented via
   * a pointer to function selected at construction time.
   */
  template<StoragePolicySwitch storagePolicy=ROWMAJOR>
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
      //! The general template for the policies. Only declaration since I will always use full specializations
    //    template<StoragePolicySwitch thePolicy> size_type M_getIndex(size_type const & i, size_type const & j) const;
    //! Function returning index according to ordering
    size_type getIndex(size_type const & i, size_type const & j) const;
  public:
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
    explicit MyMat0(size_type n=0, size_type m=0): nr(n), nc(m), data(n*m,0.){};
    //! Default copy constructor is ok
    MyMat0(const MyMat0&)=default;
    //! Default move constructor is ok
    MyMat0(MyMat0&&)=default;
    //! Default move assign is ok
    MyMat0& operator=(MyMat0&&)=default;
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
    double operator () (const size_type i, const size_type j) const
    {
    	return data[getIndex(i,j)];
    }
    //! Returns element with no bound check (non-const version)
    /*!
      It allows m(1,1)=1 on non-constant matrix m
     */
    double & operator () (const size_type i, const size_type j)
    {
    	return data[getIndex(i,j)];
    }
    //! Policy cannot be changed, but it may be queried
    constexpr StoragePolicySwitch getStoragePolicy()const 
    {
      return storagePolicy;
    }
    //! Computes \f$ ||A||_\infty \f$
    double normInf() const;
    //! Computes \f$ ||A||_1 \f$
    double norm1() const;
    //! Computes Frobenious norm
    double normF() const;
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
  template<StoragePolicySwitch storagePolicy>
  std::vector<double> operator * (MyMat0<storagePolicy> const & m,std::vector<double> const & v);

  //                 DEFINITIONS
  
  //! Specialization for row major ordering
  /*!
   Note: important to declare it inline. Otherwise it should go to a cpp file! And it will be less
   efficient!
  */
  template<>
  inline size_type MyMat0<ROWMAJOR>::getIndex(size_type const & i, size_type const & j) const
  {
    return i + j*nr;;
  }
  //! Specialization for column major ordering
  /*!
    Note: important to declare it inline. Otherwise it should go to a cpp file! And it will be less
    efficient!
  */
  template<>
  inline size_type MyMat0<COLUMNMAJOR>::getIndex(size_type const & i, size_type const & j) const
  {
    return j + i*nc;
  }
  
  template<StoragePolicySwitch storagePolicy>
  void MyMat0<storagePolicy>::resize(size_type const n, size_type const m)
  {
    if(n*m != nc*nr)
      {
	// clear data storage
	data.resize(n*m);
	data.shrink_to_fit();
	  for (auto& i: data)i=0.0;
      }
    //! fix number of rows and column
    nr=n;
    nc=m;
  }
  
  template<StoragePolicySwitch storagePolicy>
  double MyMat0<storagePolicy>::normInf() const{
    if(nr*nc==0)return 0;
    double vmax(0.0);
    
    for (size_type i=0;i<nr;++i){
      double vsum=0;
      for (size_type j=0;j<nc;++j) vsum+=data[getIndex(i,j)];
      vmax=std::max(vsum,vmax);
    }
    return vmax;
  }
  
  template<StoragePolicySwitch storagePolicy>
  double MyMat0<storagePolicy>::norm1() const{
    if(nr*nc==0)return 0;
    double vmax(0);
    for (size_type j=0;j<nc;++j){
      double vsum=0;
      for (size_type i=0;i<nr;++i) vsum+=data[getIndex(i,j)];
      vmax=std::max(vsum,vmax);
    }
    return vmax;
  }
  
  template<StoragePolicySwitch storagePolicy>
  double MyMat0<storagePolicy>::normF() const{
    if(nr*nc==0)return 0;
    double vsum=0;
    for (auto const x: data) vsum+=x*x;
    return std::sqrt(vsum);
  }
  
  
  template<StoragePolicySwitch storagePolicy>
  void MyMat0<storagePolicy>::vecMultiply(const std::vector<double> &v, std::vector<double> & res) const
  {
    if(v.size() != nc)
      {
	std::cerr<<" Vector must have the right size"<<std::endl;
	std::exit(1);
      }
    res.resize(nr,0);
    // for efficiency I use two different algorithms

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
    
    template<StoragePolicySwitch storagePolicy>
    void  MyMat0<storagePolicy>:: fillRandom(unsigned int seed)
    {
      if (seed==0) seed=std::time(0);
      double rmax=static_cast<double>(RAND_MAX+2.0);
    std::srand(seed);
    if(nr*nc>0)
      for (auto& x: data) x=static_cast<double>(std::rand()+1)/rmax;
  }
  
  
  template<StoragePolicySwitch storagePolicy>
  void MyMat0<storagePolicy>::showMe(std::ostream & out) const{
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

  template<StoragePolicySwitch storagePolicy>
  std::vector<double> operator * (MyMat0<storagePolicy> const & m,std::vector<double> const & v)
  {
    std::vector<double> tmp;
    m.vecMultiply(v,tmp);
    return tmp;
  }

}


#endif
