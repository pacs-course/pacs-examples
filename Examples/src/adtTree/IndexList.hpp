/*
 * indexList.hpp
 *
 *  Created on: Mar 3, 2022
 *      Author: forma
 */

#ifndef ADTTREE_INDEXLIST_HPP_
#define ADTTREE_INDEXLIST_HPP_
#include <vector>
#include <exception>
#include <iostream>
namespace apsc
{
  namespace adt
  {
    /*! A basic node of the adt structure
     *
     * A node can be anything with two public unsigned integer data
     * member named l and r and one unsigned int member called id;
     * All initialised to zero.
     */
    struct BasicNode
    {
      std::size_t l{0u};//!< the left node
      std::size_t r{0u}; //!< the right node
      std::size_t id{0u};//!< the node id
      std::size_t up{0u};//!< the parent node
    };

    /*!
     * IndexList is a basic dynamic container for the nodes of an
     * adt structure. It provides the possibility of getting the index
     * of the node and the corresponding node,or return a node for a later use.
     *
     * The scope is to have a dynamic structure to implement dynamic adt trees where
     * nodes can be added or deleted.
     *
     * It works as a double linked list, but with the extra feature of using the
     * efficient data structure provided by a std::vector.
     *
     * @tparam Node A node complying the rule for adt Nodes
     */
    template<typename Node=BasicNode>
    class IndexList
    {
    public:
      /*!
       *  Constructor taking the size of the container.
       * @param N The size
       */
      IndexList(std::size_t N=0u):node_list(N+1u)
      {
        if (N==0)
          {
            node_list[head].l=0u;
            node_list[head].r=0u;
          }
        else
          {
            std::size_t counter=0u;
            for (auto &n :node_list)
              {
                auto next = (counter+1u) % (N+1u);
                n.r=next;
                node_list[next].l=counter;
                n.id=counter;
                ++counter;
              }
            node_list[head].l=N;
          }
        num_avail=N;
      }
/*!
 * Increase the size of the container.
 * @param N The size to add
 */
      void increaseSize(std::size_t N) noexcept;

      /*!
       * Get a new node index
       * @return a new node index
       */
      std::size_t getNodeIndex();
      /*!
       * Return an index to the structure. The corresponding node is made
       * available for a possible reuse.
       * @param index
       */
      void returnNodeIndex(std::size_t index) noexcept;
      //! Addressing operator returns the node
      auto const & operator[](std::size_t i)const noexcept {return node_list[i];}
      auto & operator[](std::size_t i)noexcept {return node_list[i];}
      /*!
       * Returns true if there are no more nodes available.
       *
       * It means that either you have to increase the size using increaseSize(N) or
       * the next request for a node will automatically increase the size, using
       * the static member addedBlockSize
       */
      bool empty(){return num_avail==0u;}

      template <class N>
      friend
      std::ostream & operator <<(std::ostream& out, IndexList<N> const &);
    private:
      std::size_t num_avail=0u;
      static constexpr std::size_t head=0u;
      std::vector<Node> node_list;
      static constexpr std::size_t addedBlockSize=1000u;
    };
/*!
 *  Pretty print
 * @tparam Node The node type
 * @param out An output stream
 * @param list An adt::indexList
 * @return The output stream given in input
 */
    template <typename Node>
    std::ostream & operator <<(std::ostream& out, IndexList<Node> const & list)
    {
      out<<"Forward\n";
      auto head = IndexList<Node>::head;
      auto first=list.node_list[head].r;
      while (first !=head)
        {
          out<<first<<"->";
          first=list.node_list[first].r;
        }
      out<<std::endl;
      out<<"Backward\n";
      auto last=list.node_list[head].l;
      while (last !=head)
        {
          out<<last<<"<-";
          last=list.node_list[last].l;
        }
      out<<std::endl;
      return out;
    }
  }
}
//*** Implementations

template<typename Node>
inline std::size_t
apsc::adt::IndexList<Node>::getNodeIndex ()
{
  if (num_avail==0u)
    {
      if(node_list[head].l!=node_list[head].l)
        {
          throw std::runtime_error("Node data structure is broken");
        }
      // increase vector size
      increaseSize(addedBlockSize);
    }
  auto next_avail=node_list[head].r;
  node_list[head].r=node_list[next_avail].r;
  node_list[node_list[next_avail].r].l=head;
  --num_avail;
  return next_avail;
}

template<typename Node>
inline void
apsc::adt::IndexList<Node>::increaseSize (std::size_t N) noexcept
{
  auto old_size=node_list.size();
  if (N==0)return;
  node_list.resize(old_size+N);
  num_avail+=N;
  for (auto i=old_size;i<node_list.size();++i)
    {
      node_list[i].l=i-1u;
      node_list[i].r=i+1u;
      node_list[i].id=i;
    }
  auto old_last=node_list[head].l;
  node_list[old_last].r=old_size;
  node_list[old_size].l=old_last;
  node_list.back().r=head;
  node_list[head].l=node_list.size()-1u;
}

template<typename Node>
inline void
apsc::adt::IndexList<Node>::returnNodeIndex (std::size_t index) noexcept
{
  /*
  auto last_node = node_list[head].l;
  node_list[head].l=index;
  node_list[index].r=head;
  node_list[last_node].r=index;
  node_list[index].l=last_node;
  */
  auto first_node = node_list[head].r;
  node_list[head].r=index;
  node_list[index].l=head;
  node_list[index].r=first_node;
  node_list[first_node].l=index;
  ++num_avail;
}

#endif /* ADTTREE_INDEXLIST_HPP_ */
