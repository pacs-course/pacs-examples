#ifndef HHH_LinearAlgebraTraits__HH
#define HHH_LinearAlgebraTraits__HH
#include <memory>

#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
namespace LinearAlgebra
{
  template <class Derived>
  inline double
  dot(const Eigen::SparseMatrixBase<Derived> &a,
      const Eigen::SparseMatrixBase<Derived> &b)
  {
    return a.dot(b);
  }

  template <class Derived>
  inline double
  dot(const Eigen::MatrixBase<Derived> &a, const Eigen::MatrixBase<Derived> &b)
  {
    return a.dot(b);
  }

  template <class Derived>
  inline double
  norm(const Eigen::SparseMatrixBase<Derived> &a)
  {
    return a.norm();
  }

  template <class Derived>
  inline double
  norm(const Eigen::MatrixBase<Derived> &a)
  {
    return a.norm();
  }


  // This part is experimental. It is not needed and it may be wrong.
  // Ignore it for the moment
  // Primary templates
  template <class Vector>
  class VectorView
  {
  public:
    using Vector_type  = Vector;
    using pointer_type = std::unique_ptr<Vector>;
    VectorView(pointer_type &&V)
      : M_vector(std::move(V)){};
    VectorView(VectorView &&) = default;
    VectorView(VectorView const &rhs)
      : M_vector(new Vector(*rhs.M_vector))
    {}
    VectorView &
    operator=(VectorView &&rhs)
    {
      M_vector = std::move(rhs);
      return *this;
    }
    VectorView &
    operator=(VectorView &rhs)
    {
      M_vector = new Vector(*rhs.M_vector);
      return *this;
    }
    operator Vector &()
    {
      return *M_vector;
    }
    operator Vector const &() const
    {
      return *M_vector;
    }
    // Interfaces
    VectorView(int i)
      : M_vector(new Vector{i}){};
    VectorView()
      : M_vector(new Vector()){};

  private:
    pointer_type M_vector;
  };

  template <class Vector, class... Args>
  VectorView<Vector>
  make_VectorView(Args &&... args)
  {
    using pointer_type = typename VectorView<Vector>::pointer_type;
    return VectorView<Vector>(
      pointer_type(new Vector(std::forward<Args>(args)...)));
  }

  template <class Matrix>
  class MatrixView
  {
  public:
    using Matrix_type  = Matrix;
    using pointer_type = std::unique_ptr<Matrix>;
    MatrixView(pointer_type &&M)
      : M_matrix(std::move(M)){};
    MatrixView(MatrixView &&) = default;
    MatrixView(MatrixView const &rhs)
      : M_matrix(new Matrix(*rhs.M_matrix))
    {}
    MatrixView &
    operator=(MatrixView &&rhs)
    {
      M_matrix = std::move(rhs);
      return *this;
    }
    MatrixView &
    operator=(MatrixView &rhs)
    {
      M_matrix = new Matrix(*rhs.M_matrix);
      return *this;
    }
    operator Matrix &()
    {
      return *M_matrix;
    }
    operator Matrix const &() const
    {
      return *M_matrix;
    }

  private:
    pointer_type M_matrix;
  };

  template <class Matrix, class... Args>
  MatrixView<Matrix>
  make_MatrixView(Args &&... args)
  {
    using pointer_type = typename MatrixView<Matrix>::pointer_type;
    return MatrixView<Matrix>(
      pointer_type(new Matrix(std::forward<Args>(args)...)));
  }


  template <class Precond>
  class PrecondView
  {
  public:
    using Precond_type = Precond;
    using pointer_type = std::unique_ptr<Precond>;
    PrecondView(pointer_type &&M)
      : M_Precond(std::move(M)){};
    PrecondView(PrecondView &&) = default;
    PrecondView(PrecondView const &rhs)
      : M_Precond(new Precond(*rhs.M_Precond))
    {}
    PrecondView &
    operator=(PrecondView &&rhs)
    {
      M_Precond = std::move(rhs);
      return *this;
    }
    PrecondView &
    operator=(PrecondView &rhs)
    {
      M_Precond = new Precond(*rhs.M_Precond);
      return *this;
    }
    operator Precond &()
    {
      return *M_Precond;
    }
    operator Precond const &() const
    {
      return *M_Precond;
    }

  private:
    pointer_type M_Precond;
  };

  template <class Precond, class... Args>
  PrecondView<Precond>
  make_PrecondView(Args &&... args)
  {
    using pointer_type = typename PrecondView<Precond>::pointer_type;
    return PrecondView<Precond>(
      pointer_type(new Precond(std::forward<Args>(args)...)));
  }

} // namespace LinearAlgebra
#endif
