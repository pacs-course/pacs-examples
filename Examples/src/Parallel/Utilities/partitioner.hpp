/*
 * partitioner.hpp
 *
 *  Created on: Sep 17, 2022
 *      Author: forma
 */

#ifndef AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_UTILITIES_PARTITIONER_HPP_
#define AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_UTILITIES_PARTITIONER_HPP_
#include <algorithm>
#include <array>
#include <type_traits>
#include <vector>
namespace apsc
{
/*!
 * Partitions num_elements elements into num_tasks chunks
 *
 * Ths partition is equilibrated, the difference in size
 * between the chunks is at most i
 *
 * It implements a grouped partitioner: chunks of larger size are the last ones
 */
class GroupedPartitioner
{
public:
  /*!
   * Constructor
   * @param num_tasks The number of tasks
   * @param num_elements The number of elements
   */
  GroupedPartitioner(unsigned int num_tasks, std::size_t num_elements)
    : num_tasks{num_tasks}, num_elements{num_elements},
      rest{num_elements % num_tasks}, chunk_size{num_elements / num_tasks} {};

  GroupedPartitioner() = default;
  /*!
   * Sets partitioner data if not given with constructor
   * @param num_t The number of tasks
   * @param num_e The number of elements
   */
  void
  setPartitioner(unsigned int num_t, std::size_t num_e)
  {
    num_tasks = num_t, num_elements = num_e;
    rest = num_elements % num_tasks;
    chunk_size = num_elements / num_tasks;
  }
  /*!
   * The first element of chunk t
   * @param t the chunk index
   * @return the first elements
   */
  auto
  first(std::size_t t) const
  {
    return t * chunk_size + std::min(t, rest);
  }
  /*!
   * The last element +1 of chunk t
   * @param t The chunk index
   * @return The lst elements
   */
  auto
  last(std::size_t t) const
  {
    return this->first(t + 1u);
  }
  /*!
   * The chunk index of element i in the partition
   * @param i the element index
   * @return the chunk index
   */
  auto
  loc(std::size_t i) const
  {
    return std::min(i / (chunk_size + 1u), (i - rest) / (chunk_size + 1u));
  }

  /*!
   * The number of tasks
   * @return The number of tasks
   */
  auto
  get_NumTasks() const
  {
    return num_tasks;
  }

private:
  unsigned int num_tasks;
  std::size_t  num_elements;
  std::size_t  rest;
  std::size_t  chunk_size;
};

/*!
 * Partitions num_elements elements into num_tasks chunks
 *
 * Ths partition is equilibrated, the difference in size
 * between the chunks is at most i
 *
 * It implements a distributed partitioner
 */
class DistributedPartitioner
{
public:
  /*!
   * Constructor
   * @param num_tasks The number of tasks
   * @param num_elements The number of elements
   */
  DistributedPartitioner(unsigned int num_tasks, std::size_t num_elements)
    : num_tasks{num_tasks}, num_elements{num_elements},
      rest{num_elements % num_tasks}, chunk_size{num_elements / num_tasks} {};
  DistributedPartitioner() = default;
  /*!
   * Sets partitioner data if not given with constructor
   * @param num_t The number of tasks
   * @param num_e The number of elements
   */
  void
  setPartitioner(unsigned int num_t, std::size_t num_e)
  {
    num_tasks = num_t, num_elements = num_e;
    rest = num_elements % num_tasks;
    chunk_size = num_elements / num_tasks;
  }
  /*!
   * The first element of chunk t
   * @param t the chunk index
   * @return the first elements
   */
  auto
  first(std::size_t t) const
  {
    return (t * num_elements) / num_tasks;
  }
  /*!
   * The last element +1 of chunk t
   * @param t The chunk index
   * @return The lst elements
   */
  auto
  last(std::size_t t) const
  {
    return this->first(t + 1u);
  }
  /*!
   * The chunk index of element i in the partition
   * @param i the element index
   * @return the chunk index
   */
  auto
  loc(std::size_t i) const
  {
    return (num_tasks * (i + 1u) - 1u) / num_elements;
  }
  /*!
   * The number of tasks
   * @return The number of tasks
   */
  auto
  get_NumTasks() const
  {
    return num_tasks;
  }

private:
  unsigned int num_tasks;
  std::size_t  num_elements;
  std::size_t  rest;
  std::size_t  chunk_size;
};

#if __cplusplus >= 202002L
/*!
 * The concept of a partitioner
 * @tparam T the type of the partitioner
 */
template <class T>
concept PartitionerType = requires(T t) {
                            {
                              t.get_NumTasks()
                              } -> std::convertible_to<unsigned int>;
                            {
                              t.first(0u)
                              } -> std::convertible_to<std::size_t>;
                            {
                              t.last(0u)
                              } -> std::convertible_to<std::size_t>;
                          };
#endif
/*!
 * This utility takes a partitioner object and produces counts and
 * displacements as needed for MPI gatherv or scatterv routines
 * @tparam Partitioner The type of a partitioner
 * @param partitioner The partitioner
 * @return An array with counts and displacements. in vectors of int because
 * this is what MPI wants!
 */
#if __cplusplus >= 202002L
template <PartitionerType Partitioner>
#else
template <class Partitioner>
#endif
auto
counts_and_displacements(Partitioner const &partitioner)
  -> std::array<std::vector<int>, 2>
{
  auto             num_tasks = partitioner.get_NumTasks();
  std::vector<int> counts;
  std::vector<int> displacements;
  counts.reserve(num_tasks);
  displacements.reserve(num_tasks);
  for(auto i = 0u; i < num_tasks; ++i)
    {
      counts.emplace_back(
        static_cast<int>(partitioner.last(i) - partitioner.first(i)));
    }
  displacements.emplace_back(0);
  for(auto i = 1u; i < num_tasks; ++i)
    {
      displacements.emplace_back(
        static_cast<int>(displacements.back() + counts[i - 1u]));
    }
  return {counts, displacements};
}

/*!
 * The possible orderings of a matrix. We assume that the matrix is stored as a
 * linear contiguous buffer where elements are ordered row-wise or column-wise.
 *
 */
enum class ORDERINGTYPE
{
  ROWWISE = 0,
  COLUMNWISE = 1
};

/*!
 * Class that provides useful tools for partitioning a matrix with a row-wise or
 * column-wise partitioning
 *
 * The MatrixPartitioner class satisfies the concept of a PartitionerType, where
 * the matrix data is in fact serialied in a linear buffer, according to the
 * ordering.
 *
 * @tparam P A partitioner, either GroupedPartitioner or DistributedPartitioner
 * @tparam O The ordering type
 */
template <typename P = DistributedPartitioner,
          ORDERINGTYPE O = ORDERINGTYPE::ROWWISE>
class MatrixPartitioner
{
public:
  inline static constexpr ORDERINGTYPE Ordering = O;
  /*!
   * Constructor taking matrix properties
   * @param num_rows Number of rows
   * @param num_cols Number of columns
   * @param num_tasks Number of tasks (processors r thread) for the partion
   */
  MatrixPartitioner(std::size_t num_rows, std::size_t num_cols,
                    unsigned int num_tasks)
    : num_rows{num_rows}, num_cols{num_cols}, num_tasks{num_tasks}
  {
    if constexpr(O == ORDERINGTYPE::ROWWISE)
      {
        Partitioner.setPartitioner(num_tasks, num_rows);
      }
    else
      {
        Partitioner.setPartitioner(num_tasks, num_cols);
      }
  }
  MatrixPartitioner() = default;
  /*!
   * Sets the partitioner
   * @param num_rows Number of rows
   * @param num_cols Number of columns
   * @param num_tasks Number of tasks (processors r thread) for the partion
   */
  void
  setPartitioner(std::size_t num_r, std::size_t num_c, unsigned int num_t)
  {
    num_rows = num_r;
    num_cols = num_c;
    num_tasks = num_t;
    if constexpr(O == ORDERINGTYPE::ROWWISE)
      {
        Partitioner.setPartitioner(num_tasks, num_rows);
      }
    else
      {
        Partitioner.setPartitioner(num_tasks, num_cols);
      }
  }

  /*!
   * The first row associated to task t
   * @param t
   * @return The row index
   */
  auto
  first_row(std::size_t t) const
  {
    if constexpr(O == ORDERINGTYPE::ROWWISE)
      {
        return Partitioner.first(t);
      }
    else
      {
        return 0u;
      }
  }
  /*!
   * The last row associated to task t
   * @param t the task number
   * @return The row index
   */
  auto
  last_row(std::size_t t) const
  {
    if constexpr(O == ORDERINGTYPE::ROWWISE)
      return this->first_row(t + 1);
    else
      return num_rows;
  }
  /*!
   * The first column associated to task t
   * @param t the task number
   * @return The column index
   */

  auto
  first_col(std::size_t t) const
  {
    if constexpr(O == ORDERINGTYPE::ROWWISE)
      {
        return 0u;
      }
    else
      {
        return Partitioner.first(t);
      }
  }
  /*!
   *
   * The last column associated to task t
   * @param t
   * @return The column index
   */
  auto
  last_col(std::size_t t) const
  {
    if constexpr(O == ORDERINGTYPE::ROWWISE)
      {
        return num_cols;
      }
    else
      return this->first_col(t + 1);
  }

  /*!
   * The index of the first element in the data buffer associated
   * to task t
   * @param t
   * @return The index in the data buffer
   */
  auto
  first(std::size_t t) const
  {
    if constexpr(O == ORDERINGTYPE::ROWWISE)
      {
        return Partitioner.first(t) * num_cols;
      }
    else
      {
        return Partitioner.first(t) * num_rows;
      }
  }

  /*!
   * Returns the number of rows and columns for each task, given the number of
   * tasks.
   * @param num_procs The number of tasks
   * @return an array with the number of rows and columns stored in two vectors
   */
  std::array<std::vector<std::size_t>, 2>
  getLocalRowsAndCols(std::size_t num_procs) const
  {
    std::vector<std::size_t> rows(num_procs);
    std::vector<std::size_t> cols(num_procs);
    for(std::size_t i = 0u; i < num_procs; ++i)
      {
        rows[i] = last_row(i) - first_row(i);
        cols[i] = last_col(i) - first_col(i);
      }
    return {rows, cols};
  }

  /*!
   * The index of the last element in the data buffer associated
   * to task t
   * @param t The task number
   * @return The index in the data buffer
   */
  auto
  last(std::size_t t) const
  {
    return this->first(t + 1);
  }

  /*!
   * The task to which a given matrix element has been assigned
   * @param row The rwo indes
   * @param col The column index
   * @return the task number
   */
  auto
  loc(std::size_t row, std::size_t col) const
  {
    if constexpr(O == ORDERINGTYPE::ROWWISE)
      return Partitioner.loc(row);
    else
      return Partitioner.loc(col);
  }
  /*!
   * The number of tasks
   * @return
   */
  auto
  get_NumTasks() const
  {
    return num_tasks;
  }

private:
  std::size_t  num_rows;
  std::size_t  num_cols;
  unsigned int num_tasks;
  P            Partitioner;
};

} // namespace apsc

#endif /* AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_UTILITIES_PARTITIONER_HPP_ */
