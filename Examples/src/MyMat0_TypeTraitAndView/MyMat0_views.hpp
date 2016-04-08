#ifndef HH_MYMAT0_Views_HH
#define HH_MYMAT0_Views_HH
#include "MyMat0.hpp"
#include <type_traits>
namespace LinearAlgebra{
  //! @file MyMat0_views.hpp Example of views.
  /*!
    @detail Views are a design pattern to access an existing resources with a different semantic.
    For instance, the transpose of a given matrix may be a view on a matrix where the semantic of some methods
    has changed. 

    The key idea is to have a class that hold a reverence to the object to be 'vewed' and redifines all methods
    of the viewed class as function of the methods defined in the references object. It is an example of delegation.

    Here some examples.
   */

  /*!
    A view for the transpose of a Matix.
   */
  template<typename MAT>
  class DiagonalView
  {
  public:
    using value_type = typename MAT::value_type;
    using size_type  = typename MAT::size_type;
  private:
    using rtype = typename std::conditional<
    std::is_const<MAT>::value,
    value_type const &, 
    value_type &>::type;
  public:
    explicit DiagonalView(MAT & m):M_mat(m){}
    void fillZero(Helpers::fillSwitch<true>){return M_mat.fillZero();}
    //! Resizing the matrix
    /*!
     * I resize the reference, exchanging rows with cols
     */
    void resize(size_type const nrow, size_type const ncol){M_mat.resize(ncol,nrow);}
    //! Number of rows
    size_type nrow()const {return M_mat.ncol();}
    //! Number of columns
    size_type ncol()const {return M_mat.ncol();}
    //! Returns element with no bound check (non-const version)
    /*!
      The return type differs depending if MAT is a const MyMat0 or
      not. This avoids errors if I call the operator on a diagonal view
      of a constant matrix!
     */
    rtype operator () (const size_type i, const size_type j) const
    {
      return M_mat(j,i);
    }
    //! Returns element with no bound check (const version)
    value_type  operator () (const size_type i, const size_type j)
    {
      return M_mat(j,i);
    }
    //! Policy cannot be changed, but it may be queried
    constexpr StoragePolicySwitch getStoragePolicy()const 
    {
      return M_mat.getStoragePolicy();
    }
    
    /*! Norm inf
      We exploit the fact that the infinity norm of the transpose is the 1-norm of the matrix!
      
      IMPORTANT
      In c++11 we have to use the syntax
      @code
      auto normInf() const -> decltype(this->M_mat.norm1())
      @endcode
      to make sure that the return type is the correct one (the same of the underlying matrix).
      In c++14 and over you can use automatic return type 
      @code
      auto normInf() const
      {
      return M_mat.norm1();
      }
      @endcode
      but you need to activate c++14 support (or later). 

      I present here the technique that works also in c++11. 
      Unfortunately,
      @code
      auto normInf() const -> decltype(M_mat.norm1())
      @endcode
      won't work. The reason is that the compiler is unable to locate M_mat.
      
      What I can do (and it works) is
      @code
      auto normInf() const -> decltype(MAT().norm1());
      @endcode

      yet this requires that the matrix MAT have a default constructor.

      If you want to be be able to extract the type returned by a method, in this case
      norm1() without requiring any contructors you need to use the declval magic:

      @code
      auto normInf() const -> decltype(std::declval<MAT>().norm1())
      @endcode
      
      std::declval<T>() returns a reference to T and it allows to extract, using decltype(), the type of a method
      of T without *the need of any constructor*. It can be used only in unevaluated context, in practice with decltype() and 
      little else.
    */
    
    auto normInf() const -> decltype(std::declval<MAT>().norm1())
    {
      return M_mat.norm1();
    }

    auto norm1() const -> decltype(std::declval<MAT>().normInf())
    {
      return M_mat.normInf();
    }

    auto normF() const -> decltype(std::declval<MAT>().normF())
    {
      return M_mat.normF();
    }
    //! 
    void vecMultiply(const std::vector<value_type> &v, std::vector<value_type> & res) const;
    
    auto fillRandom(unsigned int seed=0) -> decltype (std::declval<MAT>().fillRandom())
    {
      return M_mat.fillRandom(seed);
    }
    
    void showMe(std::ostream  & out=std::cout) const;

    private:
    MAT & M_mat;
  };

  template<typename MAT>
  void DiagonalView<MAT>::vecMultiply(const std::vector<value_type> &v, std::vector<value_type> & res) const
  {
    if(v.size() != M_mat.nc)
      {
        std::cerr<<" Vector must have the right size"<<std::endl;
        std::exit(1);
        }
    res.resize(M_mat.nr,value_type(0));
    // for efficiency I use two different algorithms
    // I could have used overloading!
    switch(M_mat.getStoragePolicy())
      {
      case COLUMNMAJOR:
        // Classic A^T*v row by row
        for (size_type i=0;i<M_mat.nc*M_mat.nr;++i) res[i / M_mat.nc] += M_mat.data[i]*v[i % M_mat.nc];
        break;
      case ROWMAJOR:
        // result is a linear combination of the columns of A^T
        for (size_type i=0;i<M_mat.nc*M_mat.nr;++i) res[i % M_mat.nr] += M_mat.data[i]*v[i / M_mat.nr];
        break;
      }
  }

  template<typename MAT>
  void DiagonalView<MAT>::showMe(std::ostream  & out) const
  {
    auto nc = M_mat.nc;
    auto nr = M_mat.nr;
    out<<"[";
    for (size_type i=0;i<nr;++i)
      {
        for (size_type j=0;j<nc-1;++j) out<< this->operator()(i,j)<<", ";
        out<< this->operator()(i,nc-1);
        if(i==nr-1)
          out<<"]"<<std::endl;
        else
          out<<std::endl;
      }
  }
  
  template<class T, StoragePolicySwitch storagePolicy>
  std::vector<T> operator * (DiagonalView<MyMat0<T, storagePolicy> > const & m,std::vector<T> const & v)
  {
    std::vector<T> tmp;
    m.vecMultiply(v,tmp);
    return tmp;
  }
    

}
#endif
