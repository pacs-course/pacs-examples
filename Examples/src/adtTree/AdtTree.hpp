/*
 * AdtTree.hpp
 *
 *  Created on: Mar 11, 2022
 *      Author: forma
 */
#include "AdtBox.hpp"
#include "IndexList.hpp"
#include <algorithm>
#include <stack>
#include <tuple>
#ifndef ADTTREE_ADTTREE_HPP_
#define ADTTREE_ADTTREE_HPP_
namespace apsc
{
namespace adt
{
  //! The directions I can move on the adt-tree (binary tree)
  enum Direction
  {
    Left = 0,
    Right = 1
  };

  enum Action
  {
    Stop = 0,
    GoLeft = 1,
    GoRight = 2,
    GoAll = GoLeft | GoRight
  };

  /*!
   * @brief Class to test the position at a given level
   *
   * The role of the class is to verify if a coordinate falls "on the left" or
   * "on the right" with respect to the current level.
   *
   * Every time we advance along a given dimension the corresponding partition
   * level is adjorned as well as the point against which we need to make the
   * test to move along the tree.
   *
   * @tparam BOXDIMS The dimension of the box (that returned by BOX::size())
   * @note I use %BOXDIM for the dimension so that I can directly put the adt
   * tree level as argument corresponding to the parameter dim. A little
   * overhead but less headaches
   * @todo make sure that the dimension and not the level are passed and
   * eliminate %BOXDIMS to speed up code
   */
  template <std::size_t BOXDIMS> class NodeControl
  {
  public:
    //! Sets initial data
    NodeControl()
    {
      len_.fill(1. / 2.);
      center_.fill(1. / 2.);
      // partitionLevel_.fill(0u);
    }
    /*!
     * @brief The length associated to the current level
     *
     * @param dim the box dimension index
     * @return The lenght of the current sub-box for the given dimension
     */
    double
    len(std::size_t dim) const
    {
      return len_[dim % BOXDIMS];
    }
    /*!
     * @brief The point used for the test of at the current level
     *
     * @param dim The box dimension index
     * @return The point used for the test at the current level
     */
    double
    center(std::size_t dim) const
    {
      return center_[dim % BOXDIMS];
    }
    /*!
     * @brief advance the level along a given direction
     *
     * @param dim the coordinate
     * @param direction the diraction I am moving along the tree
     */
    void
    advance(std::size_t dim, Direction direction)
    {
      dim %= BOXDIMS;
      len_[dim] /= 2;
      switch(direction)
        {
        case Left:
          center_[dim] -= len_[dim];
          break;
        case Right:
          center_[dim] += len_[dim];
          break;
        }
      //++partitionLevel_[dim];
    }
    /*!
     * @brief go bak one level along a given direction
     *
     * @param dim the coordinate
     * @param direction the direction I am moving along the tree
     */
    void
    back(std::size_t dim, Direction direction)
    {
      dim %= BOXDIMS;
      switch(direction)
        {
        case Left:
          center_[dim] += len_[dim];
          break;
        case Right:
          center_[dim] -= len_[dim];
          break;
        }
      len_[dim] *= 2;
      //--partitionLevel_[dim];
    }

    /*!
     * @brief Test if the current point dimension is at the left or right
     *
     * Test made according to the current level
     * @param dim The dimension
     * @return Left or Right
     */
    Direction
    test(std::size_t dim, double x) const
    {
      return x < center_[dim % BOXDIMS] ? Left : Right;
    }

    /*!
     * @brief Test if the current point dimension is at the left or right and
     * advance
     *
     * Test made according to the current level
     * @param dim The dimension
     * @return Left or Right
     */
    Direction
    test_and_advance(std::size_t dim, double x)
    {
      dim %= BOXDIMS;
      Direction res = test(dim, x);
      this->advance(dim, res);
      return res;
    }
    //! reset to root state
    void
    reset()
    {
      len_.fill(1. / 2.);
      center_.fill(1. / 2.);
      // partitionLevel_.fill(0u);
    }
    /*!
     * @brief Return the current level
     *
     * @param dim The dimension
     * @return The current level along that direction
     */
    // std::size_t current_level(std::size_t dim)const {return
    // partitionLevel_[dim];}
  private:
    std::array<double, BOXDIMS> len_;
    std::array<double, BOXDIMS> center_;
    // std::array<std::size_t,BOXDIMS>partitionLevel_;
  };

  /*!
   * @brief The node for the adt structure
   *
   * A node for the adt structire is composed by a BasicNode, a Box and possibly
   * other user defined types. It contains the indexes of the next Nodes on the
   * Left and on the Right. 0 indicates no nodes.
   *
   * @tparam BOX The Box type
   * @tparam Args Possible other classes that enrich the node interface
   */
  template <typename BOX, typename... Args>
  struct AdtNode : public BasicNode, public BOX, public Args...
  {
    /*!
      * I can also use the constructors of the BOX, so a AdtNode can be treated
      * as a adt::Box
      */
      using BOX::BOX;
    /*!
     * @brief A Constructor that takes a Box and elements of each additional
     * component
     *
     * @note: the Basic node is not initialised by this constructor. You may to
     * access the public members (l,r,id) directly, or use the provided methods
     * @tparam T The possible addisional types
     * @param box The box
     * @param ext The additional objects
     */
    template <typename... T>
    AdtNode(BOX const &box, T &&...ext)
      : BOX{box}, Args(std::forward<T>(ext))... {};
   /*!
     The box type
    */
    using BoxType = BOX;
    /*!
     * @brief copies a node but maintaining the id and the links!
     *
     * I can create a prototype node and copy it into this node
     * but keeping the id and links of this node unchanged!
     *
     * @param prototype A node to be copied/moved into
     */
    template <class N>
    void
    copy(N &&prototype)
    {
      auto const id = this->id;
      auto const l = this->l;
      auto const r = this->r;
      auto const up = this->up;
      // copy or move the whole lot
      *this = std::forward<N>(prototype);
      this->id = id;
      this->l = l;
      this->r = r;
      this->up = up;
    }
    //! Next node on the left or the right (read only)
    std::size_t
    next(Direction d) const
    {
      return d == Left ? l : r;
    }
    //! Next node on the left or the right
    std::size_t &
    next(Direction d)
    {
      return d == Left ? l : r;
    }
    //! The identifier of the node
    std::size_t
    Id() const
    {
      return id;
    }
    //! The identifier of the node (better avoid changing it)
    // std::size_t & Id(){return id;}
    //! Is this node a leaf?
    bool
    leaf()
    {
      return l == 0u and r == 0u;
    };
    //! Is branch in given direction empty?
    bool
    empty(Direction d)
    {
      return d == Left ? l == 0u : r == 0u;
    }
    //! I get the size of the Box
    static constexpr std::size_t BOXDIMS = BOX::size();
    //! I get the dimension of the space of box vertices
    static constexpr std::size_t DIM = BOX::dim();
  };

  template <typename NODE> class AdtTree
  {
  public:
    using BoxType = typename NODE::BoxType;
    using NodeType = NODE;
    AdtTree() = default;
    AdtTree(std::size_t N) : data_{N} {}

    static constexpr auto BOXDIMS = BoxType::size();
    static constexpr auto DIM = BoxType::dim();
    auto                  add(NodeType const &prototype) noexcept;
    /*!
     * @brief Erase node with id=index
     *
     * @param index
     * @return
     */
    auto erase(std::size_t index) noexcept;
    /*!
     * @brief An utility to get an empty node prototype for further use
     * @return an empty Node
     */
    auto
    getNodeEmptyPrototype() const
    {
      return NodeType{};
    }
    /*!
     * @brief Increase the size of the data buffer holding tree nodes
     *
     * Memory is handled dynamically, but it may be convenient
     * bypass the automatic memory handling if you know the needed size
     *
     * @param N The number of nodes the data baffer should be increased
     */
    void
    increaseSize(std::size_t N)
    {
      data_.increaseSize(N);
    }
    /*!
     * @brief Visit the adtTree
     *
     * We can visit the tree traversing it in inorder fashion and specify a visitor
     * that can indicate an action to be performed after the visit of a node.
     *
     * @tparam Visitor A class that implementss a Visitor concept
     * @param visitor A visitor object
     * @return A copy of the internal representation of the visitor object visitor
     */
    template <class Visitor> Visitor visit(Visitor visitor);
    /*!
     * @brief The total number of nodes
     *
     * @return The numner of nodes in the tree
     */
    auto
    numNodes() const noexcept
    {
      return num_elements;
    }

    /*!
     * @brief A tree node
     *
     * @param index The index of the node
     * @return The corresponding node as constant reference
     */
    NODE  operator[](std::size_t index) const noexcept
    {return data_[index];}
  protected:
    /*!
     * @brief Go to the bottom using the adt comparison
     *
     * A helper function member used when adding a new node.
     * @param b The box of the node
     * @return A tuple with the node index, the direction of last move and the
     * level
     */
    auto reach_bottom(BoxType const &b) noexcept;
    //! Find a leaf of a subtree rooted in start
    /*
     */
    /*!
     * @brief Find a leaf of a subtree rooted in start start
     *
     *  We try to find a deep leaf. To do that we use a bit of
     *  entropy trying to reduce tree sparsity
     *
     * @param start The index of the root of the subtree
     * @return The index of the found leaf
     */
    auto find_leaf(std::size_t start) noexcept;
    //! The linked list structure actually holding the nodes!
    IndexList<NodeType> data_;
    std::size_t         max_level = 0u;
    //! The tree root
    std::size_t root = 0u;
    std::size_t num_elements = 0u;
  };

  template <typename NODE>
  auto
  AdtTree<NODE>::reach_bottom(const BoxType &b) noexcept
  {
    NodeControl<BOXDIMS> nc;
    auto                 current_pos = root;
    auto                 prev_pos = root;
    std::size_t          level = 0u; // we start from root and level 0
    Direction            where;
    while(current_pos != 0u)
      {
        auto dim = level % BOXDIMS; // the coordinate index in the box
        auto x = b[dim];
        where = nc.test_and_advance(dim, x);
        prev_pos = current_pos;
        current_pos = data_[current_pos].next(where);
        ++level;
      }
    return std::make_tuple(prev_pos, where, level);
  }

  template <typename NODE>
  inline auto
  AdtTree<NODE>::erase(std::size_t index) noexcept
  {
    NodeType const &thisNode = data_[index];
    // find a leaf
    auto const leafIndex = find_leaf(index);
    // get parent
    auto up = data_[leafIndex].up;
    if(up != 0u) // Tree contains more than 1 node
      {
        if(data_[up].l == leafIndex)
          {
            data_[up].l = 0u;
          }
        else
          {
            data_[up].r = 0u;
          }
      }
    if(index != leafIndex)
      {
        // This is simpler but invalidates leafIndex instead of index!
        //  thisNode.copy(data_[leafIndex]);
        //}
        up = data_[index].up;
        if(up != 0u)
          {
            if(data_[up].l == index)
              {
                data_[up].l = leafIndex;
              }
            else
              {
                data_[up].r = leafIndex;
              }
          }
        auto const il = data_[index].l;
        auto const ir = data_[index].r;
        data_[leafIndex].r = ir;
        data_[leafIndex].l = il;
        data_[leafIndex].up = data_[index].up;
        if(il != 0)
          data_[il].up = leafIndex;
        if(ir != 0)
          data_[ir].up = leafIndex;
      }
    // this for the cheaper version
    // data_.returnNodeIndex(leafIndex);
    data_.returnNodeIndex(index);
    if(index == root)
      root = leafIndex; // I'm deleting root!
    --num_elements;
    if(num_elements == 0u)
      root = 0u;
  }

  template <typename NODE>
  auto
  AdtTree<NODE>::find_leaf(std::size_t start) noexcept
  {
    if(root == 0u or num_elements == 0u)
      return root;
    auto current_pos = start;
    while(!data_[current_pos].leaf())
      {
        auto left = data_[current_pos].l;
        auto right = data_[current_pos].r;
        if(left == 0u)
          current_pos = right;
        else if(right == 0u)
          current_pos = left;
        else
          current_pos = (num_elements % 2 == 0) ? left : right;
      }
    return current_pos;
  }

  template <typename NODE>
  auto
  AdtTree<NODE>::add(NodeType const &prototype) noexcept
  {
    auto index = data_.getNodeIndex();
    if(root != 0u) // not first node
      {
        // go to the bottom
        auto [prev, where, level] = reach_bottom(prototype);
        if(where == Left)
          {
            data_[prev].l = index;
          }
        else
          {
            data_[prev].r = index;
          }
        data_[index].up = prev;
        max_level = std::max(max_level, level);
      }
    else
      {
        root = index;
        data_[index].up = 0u;
      }
    data_[index].l = 0u;
    data_[index].r = 0u;

    ++num_elements;
    // Copy the info into the actual node!
    data_[index].copy(std::move(prototype));
  }

  template <class NODE>
  template <class Visitor>
  Visitor
  AdtTree<NODE>::visit(Visitor visitor)
  {
    // a tuple storing all info about a tree node level
    using levelInfo =
      std::tuple<std::size_t, std::size_t, NodeControl<NODE::BOXDIMS>>;
    std::stack<levelInfo> levelStack;
    // We start at root
    auto start = root;
    // Check if tree is empty!
    if(start == 0u || num_elements == 0u)
      return visitor;
    auto                       level = 0u; // root level
    NodeControl<NODE::BOXDIMS> control;
    // @todo a bit cumbersome probably I can replace with a do-while
    // Iterate until stack
    while(start != 0 || (!levelStack.empty()))
      {
        //std::cout << "start=" << start << std::endl;
        // Visit current node
        auto const &thisNode = data_[start];
        Action      status = visitor(thisNode, control, level);
        std::size_t newStart = 0u;
        // Set move according to status
        if((status & GoRight) == GoRight)
          {
            // Keep track for later move to right
            levelStack.push({start, level, control});
          }
        if((status & GoLeft) == GoLeft)
          {
            // Move to left brach
            newStart = thisNode.l;
            //std::cout << "NewStart=" << newStart << std::endl;
            control.advance(level % NODE::BOXDIMS, Left);
          }
        else
          {
            // Stop here
            newStart = 0u;
          }
        start = newStart; // Adjourn node
        ++level;          // I have moved one level down
        if(start == 0u)
          {
            // I have traversed the left branches, now I have to go right!
            while(start == 0u && (!levelStack.empty()))
            {
              // Find the first not empty right branch
              // Get back stored node info
              std::tie(start, level, control) = levelStack.top();
              levelStack.pop();
              // Move right
              start = data_[start].r;
              ++level; // I am on the next level
            }
            if(start != 0u)// found a noon-leaf
              {
                // Adjust control since I have moved to the right
                control.advance(level, Right);
              }
          }
      }
    return visitor;
  }


} // end namespace adt
} // end namespace apsc

#endif /* ADTTREE_ADTTREE_HPP_ */
