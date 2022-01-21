#ifndef _HH_BC_HH_
#define _HH_BC_HH_
//! A class holding boundary conditions
namespace apsc::FEM
{
enum class BcType
{
  Neumann,
  Dirichlet,
  Robin
};
class Bc
{
protected:
  BcType M_type;

public:
  Bc(BcType type=BcType::Dirichlet):M_type{type}{};
  BcType
  getBcType() const
  {
    return M_type;
  }
  void
  setBcType(BcType type)
  {
    M_type = type;
  }
};
} // namespace apsc::Geometry
#endif
