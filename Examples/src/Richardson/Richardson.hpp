#ifndef EXAMPLES_SRC_RICHARDSON_RICHARDSON_CPP_
#define EXAMPLES_SRC_RICHARDSON_RICHARDSON_CPP_

#include <vector>
#include <functional>
#include <numeric>
#include <cmath>
#include <utility>

namespace apsc::LinearAlgebra{
  /*!
   * A class for Richardson extrapolation
   * It performs richardson extrapolation on a function that
   * approximates y and has the expansion
   * \f[
   * y(h)=y + c_0 h^{k_0}+c_1h^{k_1}\ldots
   * \f]
   * @tparam Function A callable object double -> double representing y(h)
   */
  template<class Function=std::function<double(double const &)>>
  class Richardson
  {
  public:
    using FunctionType=Function;
    /*!
     * Creates an empty Richardson object
     */
    Richardson()=default;
    /*!
     * Provides the function, the number of terms and assumes that  the expansion is
     * a standard Taylor expansion, i.e. k_i=i
     * @param f The function
     * @param M The number of terms.
     */
    Richardson(Function const & f, unsigned int M): row0(M+1),row1(M+1),y(f),k(M),kfact(M),iterates(M+1)
    {
      std::iota(k.begin(), k.end(),1u);
      setKfact();
    }
    /*!
     * In this constructor I also give the exponents k explicitly
     * @param f The function
     * @param k the exponents
     */
    Richardson(Function const & f, std::vector<unsigned int> const & k):y(f)
    {
      this->setK(k);
    }
    /*!
     * Extrapolates
     * @param h The parameter
     * @return The extrapolated value of y(h)
     */
    double operator()(double h);
    /*!
     * The last error
     * @return the error
     */
    double getError()const {return std::abs(row0.back()-*(row1.end()-2u));}
    /*!
     * @return the number of terms in the Richardson extrapolation
     */
    auto size()const {return k.size();};
    /*!
     * Sets the coefficients k of the expansion
     * @param kvalues the coefficients
     */
    void setK(std::vector<unsigned> const & kvalues)
    {
      k = kvalues;
      const std::size_t M=k.size();
      resize(M);
      setKfact();
    }
    /*!
      * Sets the coefficients k of the expansion following a Taylor truncated series
      * @param M the number of coefficients
      */
    void setK(std::size_t M)
    {
      k.resize(M);
      k = std::iota(k.begin(), k.end(),1u);
      resize(M);
      setKfact();
    }
    /*!
     * Returns all the Richardson extrapolations
     */
    auto getIterates()const {return iterates;}
  private:
    // I use t=2, a rather common choice. Change it if you want
    inline static constexpr double t=2.0;
    std::vector<double> row0;
    std::vector<double> row1;
    FunctionType y;
    std::vector<unsigned int> k;
    std::vector<double> kfact;
    //! The different level of expansions
    std::vector<double> iterates;
    void resize(std::size_t M)
    {
      row0.resize(M+1);
      row1.resize(M+1);
      iterates.resize(M+1);
      kfact.resize(M);
    }
    void setKfact()
    {
      std::size_t j=0u;
      for (auto kval:k)
        kfact[j++]=std::pow(t,kval);
    }
 };

  template<class Function>
    double Richardson<Function>::operator()(double h)
  {
    const unsigned int M=k.size();
    for (unsigned int i=0; i<=M;++i)
      {
        row1[0]=y(h);
        h/=t;
        for (unsigned int j=1;j<=i;++j)
          {
            row1[j] = (kfact[j-1]*row1[j-1]-row0[j-1])/(kfact[j-1]-1.);
          }
        iterates[i]=row1[i];
        row0.swap(row1);
      }
    return iterates[M];
  }
}

#endif /* EXAMPLES_SRC_RICHARDSON_RICHARDSON_HPP_ */
