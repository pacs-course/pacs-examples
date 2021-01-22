/*
 * joinVectors.hpp
 *
 *  Created on: Jan 21, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_UTILITIES_JOINVECTORS_HPP_
#define EXAMPLES_SRC_UTILITIES_JOINVECTORS_HPP_
#include <tuple>
#include <array>
#include <vector>
#include <exception>
#include <iterator>
namespace apsc
{
  /*
   * Forward declaration
   */
  template<class... Types>
      class JoinVectorIterator;

  /*!
   * This class imitates (only approximately) the python utility join.
   * It only provide read-only access. The extension to allow also
   * writing over the data is more complex an bejond the scope of this example.
   * Moreover it works to join std::vectors. See test_joinVector.cpp for an example of its
   * usage
   * @tparam Types The types stored in the std::vectors to be joined
   * @note The vectors must have the same size and the correct type
   * @todo Extend so that it can also write on the stored vectors
   */
  template<class... Types>
  class JoinVectors
  {
  public:
    //! The number of vectors joined
    static constexpr std::size_t numVectors=sizeof...(Types);
    //! The tuple type  containing the pointers to the joined vectore
    using ContainingTuple =std::tuple<std::vector<Types> const *...>;
    //! The type of a tuple containing the value types stored in the joined vectors
    using Values = std::tuple<Types...>;
    JoinVectors()=default;
    //! Constructor that takes the vectors to be joined
    JoinVectors(std::vector<Types> const &... args):
       data(&args...){}

    /*! Set the vectors to be joined
     * @param args The vectors
     */
    void set_Vectors(std::vector<Types> const &... args)
    {
      data=std::make_tuple(&args...);
    }
    /*!
     * Extracts the values at the i-th position of the joined vectors
     * @param i The index of the values to be extracted
     * @return A tuple of values corresponding to the i-th elements of the vrctors
     */
     Values operator[](std::size_t i)const
    {
      Values res;
      getElement<numVectors-1>(i,res);
      return res;
    }
     //! The size of the vectors
     std::size_t size()const {return std::get<0>(data)->size();}
     //!
     //! @return a const forward iterato
     auto begin() const {return jVICreator.createBegin();};
     //!
     //! @return a const forward iterator representing the end of the joined structure
     auto end() const   {return jVICreator.createEnd();}
   private:
    template <std::size_t N>
    void
    getElement(std::size_t i,Values & values)const
    {
      std::get<N>(values)=std::get<N>(data)->operator[](i);
      if constexpr (N==0u)
          return;
      else
        getElement<N-1u>(i,values);
    }
    ContainingTuple data;
    JoinVectorIterator<Types...> jVICreator{*this};
  };

  /*!
   * Utility function to save life. This is the one the user will normally adopt
   * @tparam Types Th etypes contained in the std::vectors to join
   * @param args The vectors to joine
   * @return A joinVectors object that joins the vectors
   */
  template<class... Types>
  auto join_vectors(std::vector<Types> const &... args)
  {
    return JoinVectors<Types...>{args...};
  }


  /*!
   * A ForwardInputIterator for JoinVectors<>
   *
   * This class is not so well designed since it is responsible of two things:
   * act as a simple constant Forward (only one-directional) iterator and as a class
   * that creates iterators.
   * @todo Make two separate classes for iterators and tool to create an iterator
   * @todo Compete the full specifications for an input iterator. We are missing -> and postfix increment
   * @tparam Types Should correspond to the types stores in the vectore joined in the joinVector class
   * of which this class is an iterator
   */
  template<class... Types>
    class JoinVectorIterator
    {
    public:
    /*! @defgroup type alias
     * These type alias are to conform the standard specifications for a LegacyInputIterator
     * @{
     */
    using difference_type = std::size_t;
    using value_type=typename JoinVectors<Types...>::Values;
    using pointer=value_type const *;
    using reference = value_type const &;
    using iterator_category = std::input_iterator_tag;
    //! !@}

    //! Constructor. It takes the joinVector object to which iterate
    JoinVectorIterator(JoinVectors<Types...> const & myV):myVector{myV}{}
    //! dereferencing
    //!
    //! @return the Values at the i-th position in the joined vector, where i is the current position stored in the iterator
    auto  operator * () const
    {
      return myVector[counter];
    }

    JoinVectors<Types...> const *operator ->() const
    {
      return &myVector;
    }

    /*!
     * Advancing.
     * @note no bounds check!
     * @return The advanced iterator
     */
    auto  & operator ++()
    {
      ++counter;
      return *this;
    }
    /*!
     * Advance by a positive integral value
     * @param i The value
     * @return The advanced iterator
     * @note No bounds check!
     */
    auto & operator +(std::size_t i)
    {
      counter +=i;
      return *this;
    }
    /*!
     * Decrement by a positive integral value
     * @param i The value
     * @return The decremented iterator
     * @note No bounds check! If i>counter result is undetermined (probably segmentation fault!)
     */
    auto & operator -(std::size_t i)
    {
      counter -=i;
      return *this;
    }
    /*!
     * Creates the iterator that represents the end
     * @return the end iterator
     */
    auto createEnd()const
    {
      JoinVectorIterator res(myVector);
      res.counter=myVector.size();
      return res;
    }
    //! Creates the iterator that represents begin
    auto createBegin()const
    {
      return JoinVectorIterator{myVector};
    }
    //! gives read only access to the stored joined vector.
    JoinVectors<Types...> const * theJoinVector()const {return &myVector;}
    //! The current position in the joined vector.Left public to simpfify life
      std::size_t counter=0u;
    private:
      JoinVectors<Types...> const &  myVector;
    };




  template <class... Types>
  bool operator < (const JoinVectorIterator<Types...> & a, const JoinVectorIterator<Types...> &b){
    if(a.theJoinVector() != b.theJoinVector())
      throw std::runtime_error("Cannot compare two iterators to different joined vectors" );
    return a.counter<b.counter;
  }

  template <class... Types>
    bool operator == (const JoinVectorIterator<Types...> & a, const JoinVectorIterator<Types...> &b){
    if(a.theJoinVector() != b.theJoinVector())
      throw std::runtime_error("Cannot compare two iterators to different joined vectors" );
      return a.counter==b.counter;
    }

  template <class... Types>
     bool operator != (const JoinVectorIterator<Types...> & a, const JoinVectorIterator<Types...> &b){
    if(a.theJoinVector() != b.theJoinVector())
      throw std::runtime_error("Cannot compare two iterators to different joined vectors" );
      return !(a==b);
     }

}




#endif /* EXAMPLES_SRC_UTILITIES_JOINVECTORS_HPP_ */
