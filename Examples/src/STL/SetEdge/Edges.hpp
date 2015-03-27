#ifndef HH__Edges_HPP
#define HH__Edges_HPP
#include <array>
//! A simple class storing the Ids of Points in an Edge
class Edge
{
public:
  //! Constructor taked the point Id
  /*!
    This class has NO default constructor
   */
  Edge(const unsigned & a, const unsigned & b):M_pointId{{a,b}}{}
  //!Estracts the id of the Ith point
  unsigned int operator[](unsigned int i)const
  {
    return M_pointId[i];
  }
  //!Estracts the id of the Ith point, non const version
  unsigned int & operator[](unsigned int i)
  {
    return M_pointId[i];
  }
  //! Returns both points ID, only const version
  std::array<unsigned,2> getPointId() const
  { 
    return M_pointId;
  }
private:
  //! Arrray storing the points
  std::array<unsigned,2> M_pointId;
};

struct yesOrient{
  bool operator()(Edge const& a, Edge const & b) const;
};

struct noOrient{
  bool operator()(Edge const& a, Edge const & b) const;
};
#endif
