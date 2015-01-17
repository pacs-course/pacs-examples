#ifndef __ID_HPP__
#define __ID_HPP__
#include<limits>
namespace Identifier
{
  const unsigned int notAnId=std::numeric_limits<unsigned int>::max();
  class Id
  {
    unsigned int M_id;
  public:
    Id(): M_id(notAnId){};
    Id(unsigned int i):M_id(i){};
    void setId(unsigned int const & i)
    {
      M_id=i;
    }
    unsigned int getId() const
    {
      return M_id;
    }
    bool isUnassigned() const
    {
      return M_id==notAnId;
    }
  };
}
#endif
