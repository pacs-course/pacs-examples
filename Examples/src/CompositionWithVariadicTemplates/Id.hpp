#ifndef __ID_HPP__
#define __ID_HPP__
#include<limits>
namespace Identifier
{
  //! Since c++17 I could have used std::optional 
  constexpr unsigned int notAnId=std::numeric_limits<unsigned int>::max();
  class Id
  {
  protected:
    unsigned int M_id;
  public:
    Id(unsigned int i=notAnId):M_id(i){};
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
