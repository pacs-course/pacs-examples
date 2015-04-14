/*! MyMat0 A simple class for full matrix
    Luca Formaggia 2005     */
#ifndef _MYMAT0__HH
#define _MYMAT0__HH
#include <iostream> 
#include<cstddef> // for size_t 
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
    size_t is an integral type that is guaranteed to be valid to
    store indices to POD arrays. It is convertible to int, so
    don't worry, int would (probably) work as fine, but using
    size_t we are sure that we are safe. I rename it size_type
    for consistency with the standard library
   */
  typedef std::size_t size_type;

  //! A simple matrix class of double
  /*!
   * It stores a matrix of double entries, allowing different type of
   * storage through an internal policy. The policy is implemented via
   * a pointer to function selected at construction time.
   */
  class MyMat0{
  private:
    size_type nr,nc;
    //! Data storage
    /*!
     * We store data in pointer to double.
     * This is not the best choiche since the class has to handle 
     * contstrunction and destruction of data
     */
    double * data;
    //! Policy cannot be changed, but it may be queried
    StoragePolicySwitch myPolicy;
    //! The policy for storing by rows.
    /*!
      I cannot inline it because I need to store it in a pointer (getIndex)
      This is not efficient! We will see a better technique in another example
      @param i row index.
      @param j column index
      @return the corresponding position in data.
    */
    size_type rowMajorPolicy(size_type const & i, size_type const & j) const;
    //! The policy for storing by columns.
    /*!
      I cannot inline it because I need to store it in a pointer (getIndex)
      This is not efficient! We will see a better technique in another example
      @param i row index.
      @param j column index
      @return the corresponding position in data.
    */
    size_type columnMajorPolicy(size_type const & i, size_type const & j) const;
    //! The pointer to member function used to select the current policy.
    /*!
      We will see later a better solution. Indeed goig through a pointer to
      to function makes the inlining of the policies useless!
     */
    typedef size_type (MyMat0::*storagePolicy) (
						size_type const &, 
						size_type const &) const;
    storagePolicy getIndex;
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
    explicit MyMat0(size_type n=0, size_type m=0,
		    StoragePolicySwitch sPolicy=ROWMAJOR); 
    //! Copy constructor.
    /*!
      I need a specialized one to handle data.
     */
    MyMat0(MyMat0 const & mat);
    //! Copy assignment operator.
    MyMat0 & operator = (MyMat0 const & rhs); 
    //! Move constructor
    MyMat0(MyMat0&& m);
    //! Move assignement operator
    MyMat0 & operator = (MyMat0&& rhs); 

    //! Destructor.
    /*!
      I need a destructor for memory management.
      @note since I have cared to set data to the null
      pointer when we have an empty matrix, I can just call delete[]
     */
    ~MyMat0(){delete[] data;}
    //! Resizing the matrix
    /*!
     * The storage policy cannot be changed;
     * If nrow*ncol are equal to that of the existing matrix
     * then the values are kept, otherwise they are set to zero
     */
    void resize(size_type const nrow, size_type const ncol);
    //! Number of rows
    inline  size_type nrow()const {return nr;};
    //! Number of columns
    inline  size_type ncol()const {return nc;};
    //! Returns element with no bound check (const version)
    /*!
      It allows a=m(1,1) on constant matrix m
     */
    inline double operator () (const size_type i, const size_type j) const
    {
    	return data[(this->*getIndex)(i,j)];
    }
    //! Returns element with no bound check (non-const version)
    /*!
      It allows m(1,1)=1 on non-constant matrix m
     */
    inline double & operator () (const size_type i, const size_type j)
    {
    	return data[(this->*getIndex)(i,j)];
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
    	return myPolicy;}
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
  std::vector<double> operator * (MyMat0 const & m,std::vector<double> const & v);
}
#endif
