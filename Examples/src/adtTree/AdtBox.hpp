/*
 * AdtBox.hpp
 *
 *  Created on: Mar 9, 2022
 *      Author: forma
 */

#ifndef ADTTREE_ADTBOX_HPP_
#define ADTTREE_ADTBOX_HPP_
#include <array>
#include <functional>
#include <iostream>
#include <limits>
#include <type_traits>
namespace apsc
{
namespace adt
{
  /*!
   * @brief The node type
   *
   * An enumerator class to distinguish the two main types used in an alternate
   * digital tree node: points and boxes. A Box is in fact formed by two points
   * defining the lower left and upper-right corner (\f$ (x_min, y_min,
   * z_min)\f$ and \f$(x_max, y_max, z_max)\f$ in 3D)
   */
  enum class AdtType
  {
    Point, //!< Point
    Box   //!< Box
  };
  /*!
   * @brief Coordinate box location
   *
   * In a box we need to distinguish if a coordinate belongs to the lower left
   * of upper right corner.
   *
   */
  enum class BoxLocation
  {
    LowerLeft, //!< LowerLeft corner
    UpperRight //!< UpperRight corner
  };

  /*!
   * A point is just an array
   */
  template <std::size_t DIM> using Point = std::array<double, DIM>;

  /*!
   * @brief The class representing a Cartesian box
   *
   * An adt::BOX<DIM,AdtType> can be:
   *
   * -# a set of two Point<DIM> that defines the LowerLeft and UpperRight corner
   *    of a box.\n The LowerLeft corner is the corner whose coordinates are
   * equal to or smaller than the corresponding coordinates of the other box
   * corner.\n The UpperRight corner has, instead, coordinates greater or equal
   * than the corresponding one of the other corner.
   *
   * -# Just a wrapper around a Point<DIM> (we indicate this case as
   * pointBox).\n This happens when the adt tree stores points.
   *
   * The selection is made via the second template parameter
   *
   *
   * We distinguish three types of coordinate index
   *
   * -# The physical coordinate, that goes from 0 to DIM (excluded)
   * -# The box coordinate which  goes from 0 to size() (excluded).\n
   *    For a pointBox is size()=DIM for a normal box size()=2*DIM
   * -# The tree level.\n
   *    When the box is used inside the ADT tree a level l provides the box
   * coordinate given by (l % size())
   *
   *
   * @tparam DIM The physical dimension of the Points defining the box
   * @tparam Type either equal to AdtType::Box or AdtType::Point
   */
  template <std::size_t DIM, AdtType Type = AdtType::Box> class Box
  {
  public:
    /*!
     * @brief constructor taking the two corners defining a box
     *
     * @param lowerLeft lower-left corner
     * @param upperRight upper-right corner
     */
    template <AdtType T = Type, typename = std::enable_if_t<T == AdtType::Box>>
    constexpr Box(Point<DIM> lowerLeft, Point<DIM> upperRight)
      : corners{lowerLeft, upperRight} {};
    /*!
     * @brief Version of constructor for the point Box
     *
     * @param point The point to be contained in the point Box
     */
    template <AdtType T = Type,
              typename = std::enable_if_t<T == AdtType::Point>>
    constexpr Box(Point<DIM> point) : corners{point} {};
    //! Default constructor
    Box() = default;
    /*!
     * @brief Sets the two corners defining the box
     *
     * @param lowerLeft lower-left corner
     * @param upperRight upper-right corner
     */
    template <AdtType T = Type>
    std::enable_if_t<T == AdtType::Box, void>
    setCorners(Point<DIM> const &lowerLeft, Point<DIM> const &upperRight)
    {
      corners = {lowerLeft, upperRight};
    }

    /*!
     * @brief This version is for a point BOX
     *
     * The name is kept for uniformity with the other type of box.
     * @param point The point
     */
    template <AdtType T = Type>
    std::enable_if_t<T == AdtType::Point, void>
    setCorners(Point<DIM> const &point)
    {
      corners = {point};
    }

    /*!
     * @defgroup coordinates Returns box coordinates as if it where a 2*DIM
     * point
     *
     * The Box is seen in the adt structure as a 2*DIM dimensional point.
     * Here I have chosen to use the coordinates of the lowerleft corner before
     * the upperright one.
     *
     * For instance, for a 2 dimensional Box (DIM=2,CDIM=2, size()=4) we return
     *
     * i=0 -> corners[0][0] (\f$ x_min\f$)\n
     * i=1 -> corners[0][1] (\f$ y_min\f$)\n
     * i=2 -> corners[1][0] (\f$ x_max\f$)\n
     * i=3 -> corners[1][1] (\f$ y_max\f$)\n
     *
     * for a 2 dimensional PointBox (DIM=2,CDIM=1, size()=2)\n
     * i=0 -> corners[0][0]\n
     * i=1 -> corners[0][1]\n
     *
     * @{
     * @param i The box coordinate
     * @return The coordinate of the box as a 2*DIM Point
     */
    constexpr auto
    operator[](std::size_t i) const noexcept
    {
      return corners[whichCorner(i)][phCoord(i)];
    }
    /*!
     * @param i The index from 0 to size() (CDIM*DIM)
     * @return The coordinate of the box as a 2*DIM Point
     */
    constexpr auto &
    operator[](std::size_t i) noexcept
    {
      return corners[whichCorner(i)][phCoord(i)];
    }
    //! @}
    /*!
     * @brief Info about the coordinate returned by operator[]
     *
     * For instance, for a 2 dimensional Box (DIM=2,CDIM=2, size()=4) we return
     *
     * i=0 -> corners[0][0] LowerLeft
     * i=1 -> corners[0][1] LowerLeft
     * i=2 -> corners[1][0] UpperRight
     * i=3 -> corners[1][1] UpperRight
     * i=4 -> corners[0][0] LowerLeft
     * ...
     *
     * for PointBoxes we return always LowerLeft, but in this case the
     * distinction is irrelevant!
     *
     * @param i The index from 0 to size() (CDIM*DIM), or the tree level
     * @return The location of the coordinate
     */
    static constexpr BoxLocation
    location(std::size_t i)noexcept
    {
      if constexpr(Type == AdtType::Box)
        return (i / DIM) % 2u == 0 ? BoxLocation::LowerLeft
                                   : BoxLocation::UpperRight;
      else
        return BoxLocation::LowerLeft;
    }
    /*!
     * @brief Return the index of the corner assocaited to a given level
     *
     * @param i The level
     * @return 0 or 1.
     */
    static constexpr std::size_t
    whichCorner(std::size_t i)noexcept
    {
      return location(i) == BoxLocation::LowerLeft ? 0u : 1u;
    }
    /*!
     * @brief The physical coordinate associated to the box coordinate i
     *
     * @param i A box coordinate (or an Adttree level)
     * @return The corresponding physical coordinate
     */
    static constexpr std::size_t
    phCoord(std::size_t i) noexcept
    {
      return i % DIM;
    }
    /*!
     * @brief value of coordinate at the corners
     *
     * Give an box coordinate i, returns the value of the corresponding physical
     * coordinate in the two box corners. Those values represent the extension
     * of the box along that direction.
     *
     * @note: for a Point Box the two values are identical
     *
     * @param i The box coordinate (from 0 to size()) (or the tree level)
     * @return An array containing the 2 values
     */
    std::array<double, 2>
    minmax(std::size_t i) const noexcept
    {
      return {corners[0u % CDIM][phCoord(i)], corners[1u % CDIM][phCoord(i)]};
    }

    /*!
     *  @defgroup Corners The corners of the Box
     * @{
     */
    /*!
     * @brief Returns the i-th corner
     *
     * @param i Can be a number between 0 and CDIM
     * @return
     */
    auto const &
    corner(std::size_t i) const noexcept
    {
      return corners[i % CDIM];
    }

    auto &
    corner(std::size_t i) noexcept
    {
      return corners[i % CDIM];
    }
    //@}
    /*!
     * @brief The size of the Box, = physical size doubled unless it is a point
     * Box
     *
     * @return The size
     */
    static constexpr auto
    size() noexcept
    {
      return CDIM * DIM;
    }
    //! The space dimensions=the size of the space of the box corners
    static constexpr auto
    dim() noexcept
    {
      return DIM;
    }
    //! The Box type (Box or Point)
    static constexpr AdtType
    type() noexcept
    {
      return Type;
    }
    //! Another way to identify if point box or not
    static constexpr auto
    cdim() noexcept
    {
      return CDIM;
    }
    //! The inner dimension of the container storing the box point(s)
    static constexpr std::size_t CDIM = (Type == AdtType::Box ? 2u : 1u);

  protected:
    std::array<Point<DIM>, CDIM> corners;
  };

  /*!
   * @brief Normalize points and boxes
   *
   * In an adt structure coordinates are normally normalised to be between 0.
   * and 1. This utility contains the tools to do this operation by defining a
   * bounding box.
   *
   * @tparam DIM The dimension
   */
  template <std::size_t DIM> struct Normaliser
  {
    /*!
     * @brief The constructor takes a bounding box, which will be used for the
     * normalization
     *
     * @param box A bounding box
     */
    Normaliser(Box<DIM, AdtType::Box> const &box) { setBoundingBox(box); }
    //! Default constructor
    Normaliser()
    {
    	// By default the box is the unitary box
    	length.fill(1.0);
    	invLength.fill(1.0);
    	origin.fill(0.0);
    };

    /*!
     * @brief Computes the normalization operator from a bounding box
     *
     * @param box The box containing the two corners of a bounding box
     */
    void
    setBoundingBox(Box<DIM, AdtType::Box> const &box)
    {
      origin = box.corner(0);
      for(auto i = 0u; i < DIM; ++i)
        {
          length[i] = (box.corner(1)[i] - box.corner(0)[i]);
          invLength[i] = 1. / length[i];
        }
    }

    /*!
     * @brief Normalise a Point<DIM>
     *
     * @param p The Point that will be normalized
     */
    void
    operator()(Point<DIM> &p) const
    {
      for(auto i = 0u; i < DIM; ++i)
        {
          p[i] = invLength[i] * (p[i] - origin[i]);
        }
    }
    /*!
     * @brief Normalize a Box<DIM>
     *
     * @param b A Box. It well return normalised
     */
    template <AdtType Type>
    void
    operator()(Box<DIM, Type> &b)
    {
      for(auto j = 0u; j < b.cdim(); ++j)
        this->operator()(b.corner(j));
    }

    /*!
     * @brief Inverse operation of normalise()
     *
     * @param p A normalised Point<DIM>
     * @return A un-normalised point
     */
    void
    denormalise(Point<DIM> &p) const
    {
      for(auto i = 0u; i < DIM; ++i)
        {
          p[i] = origin[i] + length[i] * p[i];
        }
    }

    /*!
     * @brief Inverse operation than normalise()
     *
     * @param b  A normalised Box<DIM>
     * @return  The un-normalised Box
     */
    template <AdtType Type>
    void
    denormalise(Box<DIM, Type> &b)
    {
      for(auto j = 0u; j < b.cdim(); ++j)
        this->denormalise(b.corner(j));
    }

    /*!
     * @brief Sets the normalizer function for a set of boxes
     *
     * @tparam BOXContainer The type of a container of boxes
     * @param boxes A container of boxes
     * @return The corresponding bounding box
     */
    template <typename BoxContainer>
    Box<DIM, AdtType::Box>
    setBoundingBox(BoxContainer const & boxes)
    {
      Box<DIM, AdtType::Box> box;
      for(auto i = 0u; i < DIM; ++i)
        {
          box.corner(0)[i] = std::numeric_limits<double>::max();
          box.corner(1)[i] = -std::numeric_limits<double>::max();
        }
      for(auto const &b : boxes)
        {
          for(auto i = 0u; i < DIM; ++i)
            {
              box.corner(0)[i] = std::min(box.corner(0)[i], b.corner(0)[i]);
              box.corner(1)[i] = std::max(box.corner(1)[i], b.corner(1)[i]);
            }
        }
      this->setBoundingBox(box);
      return box;
    }

  private:
    std::array<double, DIM> length;
    std::array<double, DIM> invLength;
    Point<DIM>              origin;
  };

  template <std::size_t DIM, AdtType Type>
  std::ostream &
  operator<<(std::ostream &out, Box<DIM, Type> const &box)
  {
    out << "BOX:\n";
    for(std::size_t i = 0u; i < box.cdim(); ++i)
      {
        Point<DIM> p = box.corner(i);
        for(double x : p)
          out << x << ", ";
        out << "\n";
      }
    out << "Min, Max coordinates:\n";
    for(std::size_t i = 0u; i < box.size(); ++i)
      {
        auto [minc, maxc] = box.minmax(i);
        out << i << "-Boxcoord: " << minc << ", " << maxc << "\n";
      }
    out << std::endl;
    return out;
  }

} // namespace adt
} // namespace apsc

#endif /* ADTTREE_ADTBOX_HPP_ */
