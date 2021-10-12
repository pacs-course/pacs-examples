#include "bound_cond.hpp"
#include <map>
#include <sstream>
namespace apsc
{
namespace FEM
{
  BCName
  BCBase::name() const
  {
    BCName theName;
    for(auto [key, value] : FEM::stringToBCType)
      {
        if(value == this->type())
          {
            theName = key;
            break;
          }
      }
    return theName;
  }

  double
  BCBase::apply(double const t, Coord const &coord) const
  {
    return M_fun(t, coord);
  }

  std::ostream &
  BCBase::showMe(std::ostream &stream) const
  {
    stream << "BC Type= " << this->name() << "(" << this->type() << ")"
           << std::endl;
    stream << "Description: " << this->M_description << std::endl;
    stream << "Associated entities:" << std::endl;
    auto const entities = this->entities();
    int        count(0);
    for(auto k : entities)
      {
        stream << k;
        if(++count % 8 == 0)
          stream << std::endl;
        else
          stream << " ";
      }
    if(!(count % 8 == 0))
      stream << std::endl;
    return stream;
  }
} // namespace FEM
} // namespace apsc
