#ifndef __ID_HPP__
#define __ID_HPP__
#include <optional>
#include <stdexcept>
namespace apsc::Geometry
{
class Id
{
protected:
  std::optional<unsigned int> M_id;

public:
  Id()=default;

  Id(unsigned int i) : M_id{i}{};

  /*!
     * @fn void setId(unsigned int)
   * @brief Set the id
   *
   * @param i The id
   */
  void
  setId(unsigned int i)
  {
    M_id = i;
  }

  /*!
   * @fn unsigned int getId()const
   * @brief get the stored id
   * @throw throws if id is unassigned
   * @return the id
   */
  unsigned int
  getId() const
  {
    if (M_id)
      return M_id.value();
    else
      throw std::runtime_error("ERROR. Cannot access unassigned IDs");
  }
  /*!
   * @fn bool isUnassigned()const
   * @brief Test if Id has been assigned
   *
   * @return true if not assigned
   */
  bool
  isUnassigned() const
  {
    return ! (M_id.has_value());
  }
};
} // namespace Geometry
#endif
