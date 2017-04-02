#ifndef _HH_DOMAIN_HH
#define _HH_DOMAIN_HH

#include <string>
#include "GetPot"

namespace Geometry
{

  /*!
    @class Domain1D
    This class implement the comcept of 1D domain, i.e. an interval. It stores the
    left and right extreme of the interval. The values are readed using GetPot, or
    imposed using the set methods.
  */
  class Domain1D
  {

  public:

    // Constructors and destructors.
    //! @name Constructors and destructors.
    //@{

    /*!
      Constructor for the class.
      @param data Costant reference for a GetPot object.
      @param section Constant reference for the section to read the parameters in the datafile.
    */
    Domain1D ( const GetPot& data,
	       const std::string& section = "domain/" );

    //! Virtual destructor.
    virtual ~Domain1D ();

    //@}

    // Get methods.
    //! @name Get methods.
    //@{

    /*!
      Return the constant left value of the interval.
      @return Left value of the interval.
     */
    inline double left() const
    {
      return M_a;
    }

    /*!
      Return the left value of the interval.
      @return Left value of the interval.
     */
    inline double& left()
    {
      return M_a;
    }

    /*!
      Return the constant right value of the interval.
      @return Right value of the interval.
    */
    inline double right() const
    {
      return M_b;
    }

    /*!
      Return the right value of the interval.
      @return Right value of the interval.
    */
    inline double& right()
    {
      return M_b;
    }

    //@}

    // Set methods
    //! @name Set methods
    //@{

    /*!
      Set the value of left extreme.
      @param left Constant reference to the left extreme.
    */
    inline void setLeft( const double & left )
    {
      M_a = left;
    }

    /*!
      Set the value of right extreme.
      @param right Constant reference to the right extreme.
    */
    inline void setRight( const double & right )
    {
      M_b = right;
    }

    //@}

  private:

    // Data values for extreme points.
    //! @name Data values for extreme points.
    //@{

    //! Left extreme.
    double M_a;

    //! Right extreme.
    double M_b;

    //@}
  };
}
#endif
