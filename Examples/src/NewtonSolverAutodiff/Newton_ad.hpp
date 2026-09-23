/*! \file Newton.hpp
 *  \brief Newton solver that obtains Jacobians with forward-mode autodiff.
 */
#ifndef NEWTON_SOLVER_AUTODIFF_HPP
#define NEWTON_SOLVER_AUTODIFF_HPP

#include "NewtonMethodsSupport_ad.hpp"
#include <limits>
#include <utility>

namespace apsc
{
namespace newton_ad
{
  class Newton : public NewtonTraits
  {
  public:
    Newton() = default;
    /*!
     * \brief Construct a Newton solver.
     * \param nls The non linear system.
     * \param opt The options for the Newton iteration.
     */
    explicit Newton(NonLinearSystemType nls,
                    NewtonOptions       opt = NewtonOptions{})
      : nonLinSys{std::move(nls)}, options{opt}
    {}
    /*!
     * \brief Construct a Newton solver.
     * \param nls The non linear system.
     * \param opt The options for the Newton iteration.
     */
    template <class NLS>
    Newton(NLS &&nls, NewtonOptions opt = NewtonOptions{})
      : nonLinSys{std::forward<NLS>(nls)}, options{opt}
    {}

    /*!
     * \brief Get the options for the Newton iteration.
     * \return The options for the Newton iteration.
     */
    [[nodiscard]] const NewtonOptions &
    getOptions() const
    {
      return options;
    }
    /*!
     * \brief Set the options for the Newton iteration.
     *  \param opt The options for the Newton iteration.
     */
    void
    setOptions(const NewtonOptions &opt)
    {
      options = opt;
      state = NewtonState{};
    }
    /*!
     * \brief Get the non linear system.
     * \return The non linear system.
     */
    [[nodiscard]] NonLinearSystemType
    getNonLinSys() const
    {
      return nonLinSys;
    }
    /*!
     * \brief Set the non linear system.
     * \param nls The non linear system.
     */
    template <class NLS>
    void
    setNonLinSys(NLS &&nls)
    {
      nonLinSys = std::forward<NLS>(nls);
      state = NewtonState{};
    }
    /*!
     * \brief Solve the non linear system.
     * \param x0 The initial guess for the solution.
     * \return The result of the Newton iteration.
     * \note The solve takes the initial guess as a standard Eigen vector, and
     * returns the solution as a standard Eigen vector. The non linear system is
     * expected instead to take and return autodiff::VectorXreal. This has een
     * done to simplify the interface of the Newton solver, and to reduce the
     * exposition of the autodiff types to the user. The user is expected to
     * write the non linear system using autodiff::VectorXreal, and to use the
     * Newton solver with standard Eigen vectors. The Newton solver will convert
     * the standard Eigen vectors to autodiff::VectorXreal, and will convert the
     * autodiff::VectorXreal to standard Eigen vectors.     */
    [[nodiscard]] NewtonResult solve(const ArgumentType &x0);
    virtual ~Newton() = default;

  protected:
    virtual void
    callback() const
    {}
    struct NewtonState
    {
      ReturnType   currentSolution;
      unsigned int currentIteration{0u};
      double       currentResidualNorm{std::numeric_limits<double>::max()};
      double       currentStepLength{std::numeric_limits<double>::max()};
    };

    NonLinearSystemType nonLinSys;
    NewtonOptions       options;
    NewtonState         state;
  };

  //! Newton variant that prints iteration diagnostics.
  struct NewtonVerbose : public Newton
  {
    using Newton::Newton;
    void callback() const override;
  };
} // namespace newton_ad
} // namespace apsc

#endif
