#ifndef NEWTON_HPP
#define NEWTON_HPP

#include "Jacobian.hpp"
#include "NewtonMethodsSupport.hpp"

#include <limits>
#include <memory>

/// This class implements Newton method
///
/// The non-linear system to be solved is composed to this class as
/// a private member, whereas the Jacobian is stored as a unique_ptr
/// to the base class. However, as a result the class is neither
/// copy-constructible nor copy-assignable. To
/// have copy operators that perform deep copy I should implement a
/// clone() method in the Jacobian classes.
///
class Newton : public NewtonTraits
{
public:
  Newton() = default;

  /// This class not meant to be copy constructible
  /// The implementation of copy constructor would require a
  /// mechanism for the deep copy of the Jacobian, which is passed
  /// as a unique pointer
  Newton(const Newton &) = delete;

  /// This constructor accepts a non linear system and a unique
  /// pointer to the Jacobian base class
  Newton(NonLinearSystemType &&        nls,
         std::unique_ptr<JacobianBase> j,
         NewtonOptions                 opt = NewtonOptions{})
    : nonLinSys(std::forward<NonLinearSystemType>(nls))
    , Jacobian_ptr(std::move(j))
    , options(opt)
  {
    // I need to connect the Jacobian to the linear system
    Jacobian_ptr->setNonLinSys(&nonLinSys);
  }

  /// This constructor accepts a non linear system and a concrete
  /// Jacobian object
  template <class Jac>
  Newton(NonLinearSystemType &&nls,
         const Jac             j,
         NewtonOptions         opt = NewtonOptions{})
    : nonLinSys(std::forward<NonLinearSystemType>(nls))
    , Jacobian_ptr(std::make_unique<Jac>(j))
    , options(opt)
  {
    // I need to connect the Jacobian to the non linear system
    Jacobian_ptr->setNonLinSys(&nonLinSys);
  }

  /// You can change the Jacobian. Note that it is moved
  /// You have to explicitly move if it is not a rvalue!
  /// For instance:
  /// \code
  ///   myNewton.setJacobianPtr(std::move(passedPointer));
  /// \endcode
  void
  setJacobianPtr(std::unique_ptr<JacobianBase> jacobianPtr)
  {
    Jacobian_ptr = std::move(jacobianPtr);
    // Connect Jacobian to the non-linear system
    Jacobian_ptr->setNonLinSys(&nonLinSys);
    // Reset the state so I can reuse the object
    this->state = NewtonState{};
  }

  /// You can set options
  void
  setOptions(const NewtonOptions &options)
  {
    this->options = options;
    this->state   = NewtonState{};
  }

  NewtonResult
  solve(const ArgumentType &x0);

  virtual ~Newton() = default;

protected:
  virtual void
  callback() const {};

  /// Internal structure providing the current state.
  /// Used for callback
  class NewtonState
  {
  public:
    /// The last computed solution
    NewtonTraits::ReturnType currentSolution;

    /// The current number of iterations
    unsigned int currentIteration{0u};

    /// The current residualNorm (set initially to a big number)
    double currentResidualNorm = std::numeric_limits<double>::max();

    /// current step length (set initially to a big number)
    double currentStepLength = std::numeric_limits<double>::max();
  };

  NonLinearSystemType           nonLinSys;
  std::unique_ptr<JacobianBase> Jacobian_ptr;
  NewtonOptions                 options;
  NewtonState                   state;
};

/// An example of  use of callback()
///
/// A verbose version of Newton
///
struct NewtonVerbose : public Newton
{
  /// Inherit constructors
  using Newton::Newton;

  /// Specialization of callback()
  void
  callback() const override;
};

#endif /* NEWTON_HPP_ */
