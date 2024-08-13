/*
 * AdtVisitors.hpp
 *
 *  Created on: Mar 11, 2022
 *      Author: forma
 */

#ifndef ADTTREE_ADTVISITORS_HPP_
#define ADTTREE_ADTVISITORS_HPP_
#include "AdtTree.hpp"
#include "AdtBox.hpp"
#include <iostream>
namespace apsc
{
  namespace adt
  {
    /*!
     * @brief Basic visitor to traverse the tree
     *
     * All visitor must have the method
     *
     * @code
     * template<typename ADTNODE>
     * Action operator()(ADTNODE const & node, NodeControl<ADTNODE::BOXDIMS> const & control, std::size_t level)
     * @endcode
     *
     * That returns the operation to be performed in terms of the enumerator
     *
     * @code
     * enum Action{Stop,GoLeft,GoRight,GoAll};
     * @endcode
     *
     *
     *
     */
    class BasicTrasversal
    {
    public:
      /*!
       * @brief  The actual visitor
       *
       * @tparam ADTNODE The type of the node of the AdtTree
       * @param node The node to be visited
       * @param control The control for the node
       * @param level The level in the tree
       * @return An Action for the traversal procedure: GoLeft, GoRight, Stop or GoAll
       */
      template<typename ADTNODE>
   Action operator()(ADTNODE const & node, NodeControl<ADTNODE::BOXDIMS> const & control, std::size_t level)
      {
        indexes_.push_back(node.id);
        return GoAll;
      }
      auto indexes()const {return indexes_;}
    private:
      std::vector<std::size_t> indexes_;
    };

/*!
 * @brief Visits the tree and prints some info on an output stream
 *
 */
    class PrintOutVisitor
     {
     public:
      //! Constructor takes a stream
      PrintOutVisitor(std::ostream & stream=std::cout,Action defAct=Action::GoAll):out{stream},defaultAction{defAct}{};
      //! Prints the content of the tree
       /*!
        * @brief  The actual visitor
        *
        * @tparam ADTNODE The type of the node of the AdtTree
        * @param node The node to be visited
        * @param control The control for the node
        * @param level The level in the tree
        * @return An Action for the traversal procedure: GoLeft, GoRight, Stop or GoAll
        */
      template<typename ADTNODE>
      Action operator()(ADTNODE const & node, NodeControl<ADTNODE::BOXDIMS> const & control, std::size_t level)
       {
         out<<"Node index "<<node.Id()<<":";
         out<< "Up, left and right:"<< node.up<<","<<node.l<<","<<node.r<<" Coord:";
         for (auto i=0u;i<node.cdim();++i)
           {
             auto const & point=node.corner(i);
             for (auto const & x:point)
               {
                 out<<x<<" ";
               }
             out<<" | ";
           }
         out<<"CENTER:"<<control.center(level)<<"level= "<<level;
         out<<"\n";
         return defaultAction;
       }
     private:
       std::ostream & out;
       Action defaultAction;
     };


 /*!
   * @brief This is the most important visitor: finds intersections and prunes the search in the tree
   *
   * Given a Box (of PointBox) the visitor collects all adtTree nodes that intersect the box.
  *
  * @tparam DIM  The dimension of the Box
  * @tparam BOXTYPE The type of the Box
  */
  template <std::size_t DIM=2u, adt::AdtType BOXTYPE=AdtType::Box>
  class IntersectionVisitor
  {
  public:
    /*!
     * @brief This constructor takes the box to intersect
     *
     * @param intersector the Box
     */
    IntersectionVisitor(Box<DIM,BOXTYPE> const & intersector):intersector_{intersector}{};
    IntersectionVisitor()=default;
    /*!
     * @brief Sets a new box to intersect.
     *
     * @param intersector The box
     */
    void setIntersector(Box<DIM,BOXTYPE> const & intersector)
    {
      intersector_=intersector;
      this->intersectingNodes.clear();
    }
    /*!
     * @brief  The actual visitor
     *
     * @tparam ADTNODE The type of the node of the AdtTree
     * @param node The node to be visited
     * @param control The control for the node
     * @param level The level in the tree
     * @return An Action for the traversal procedure: GoLeft, GoRight, Stop or GoAll
     */
    template<typename ADTNODE>
    Action operator()(ADTNODE const & node, NodeControl<ADTNODE::BOXDIMS> const & control, std::size_t level)
    {
      static_assert(ADTNODE::dim()==DIM,"Error: atd tree node must have the same dimension of the visitor");
      // The dimension we are testing at the current level
      const std::size_t phCoord=node.phCoord(level);
      // The extension of the node at this level
      std::array<double,2> const & nodeExtension=node.minmax(phCoord);
      // The extension of the potentially intersecting box
      std::array<double,2> const & intersectExtension=intersector_.minmax(phCoord);
      //std::cout<<counter++<<" "<<"nodeExtension:"<<nodeExtension[0]<<" "<<nodeExtension[1]<<"  ";
      //std::cout<<"intersectExtension:"<<intersectExtension[0]<<" "<<intersectExtension[1]<<"\n";

      if (!(nodeExtension[0]>intersectExtension[1] or intersectExtension[0]>nodeExtension[1]))
        {
          // look at the other coordinates
          bool intersect=true;
          for (std::size_t k=1u; k<DIM;++k)
            {
              std::array<double,2> const & nodeExtension=node.minmax(phCoord+k);
              std::array<double,2> const & intersectExtension=intersector_.minmax(phCoord+k);
              if (nodeExtension[0]>intersectExtension[1] or intersectExtension[0]>nodeExtension[1])
                {
                  intersect=false;
                }
            }
          //std::cout<<"intersect\n";
          // we have intersection
          if(intersect) intersectingNodes.emplace_back(node.Id());
        }
      // Now we need to decide where to go!
      double x = control.center(level);
      if constexpr (node.type()==AdtType::Point)
    {
          if(intersectExtension[1] <= x)
            return Action::GoLeft;
          else if(intersectExtension[0] > x)
            return Action::GoRight;
          else
            return Action::GoAll;
    }
      else
        {
          // adtTree is a box. Things are slightly more complex
          // The index of the corner we are using at this level
          const BoxLocation whichCorner=node.location(level);
          if(whichCorner==BoxLocation::LowerLeft and intersectExtension[1] <= x)
            return Action::GoLeft;
          else if(intersectExtension[0] > x)
            return Action::GoRight;
          else
            return Action::GoAll;
        }
    }
    /*!
     * @brief Returns the indexes of the intersecting nodes after the visit
     *
     * @return A vector of indexes.
     */
    auto intersectingIndexes()const {return intersectingNodes;}
  private:
    Box<DIM,BOXTYPE> intersector_;
    std::vector<std::size_t> intersectingNodes;
  };
  }// end namespace adt
}// end namespace apsc




#endif /* ADTTREE_ADTVISITORS_HPP_ */
