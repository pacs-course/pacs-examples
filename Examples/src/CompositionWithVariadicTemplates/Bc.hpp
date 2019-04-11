#ifndef _HH_BC_HH_
#define _HH_BC_HH_
//! A class holding boundary conditions
namespace BoundaryConditions
{
  enum class BcType{Neumann,Dirichlet,Robin};
  class Bc
  {
  protected:
    BcType M_type;
  public:
    BcType getBcType() const
    {
      return M_type;
    }
    void setBcType(BcType type)
    {
      M_type=type;
    }
  };
}// end namespace
#endif
