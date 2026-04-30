#ifndef HH_GENERATOR_HH
#define HH_GENERATOR_HH
/*!
 * @file meshGenerators.hpp
 * @brief Mesh-generation policies for one-dimensional meshes.
 */
#include "domain.hpp"
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>
namespace Geometry
{
//! Container type used to store mesh nodes.
using MeshNodes = std::vector<double>;
/*!
 * @brief Abstract interface for one-dimensional mesh generators.
 *
 * Concrete generators encapsulate the policy used to produce the node
 * coordinates for a `Mesh1D` object.
 */
class OneDMeshGenerator
{
public:
  /*!
   * @brief Build a generator associated with a domain.
   * @param d Domain where the mesh has to be generated.
   */
  explicit OneDMeshGenerator(Geometry::Domain1D const &d) : M_domain{d} {}
  /*!
   * @brief Generate the mesh nodes.
   * @return A vector containing the node coordinates.
   */
  virtual MeshNodes operator()() const = 0;
  /*!
   * @brief Access the domain handled by the generator.
   * @return The associated one-dimensional domain.
   */
  [[nodiscard]] Domain1D const &
  getDomain() const
  {
    return M_domain;
  }
  //! Virtual destructor for polymorphic use.
  virtual ~OneDMeshGenerator() = default;

protected:
  //! Domain on which the generator operates.
  Geometry::Domain1D M_domain;
};
/*!
 * @defgroup meshers Mesh Generators
 * @brief Functors implementing different one-dimensional mesh policies.
 * @{
 */
/*!
 * @brief Generator for uniformly spaced meshes.
 *
 * The interval is partitioned into a prescribed number of elements having the
 * same size.
 */
class Uniform : public OneDMeshGenerator
{
public:
  /*!
   * @brief Construct a uniform mesh generator.
   * @param domain Domain to be discretized.
   * @param num_elements Number of mesh elements.
   */
  Uniform(Geometry::Domain1D const &domain, std::size_t num_elements)
    : OneDMeshGenerator(domain), M_num_elements(num_elements)
  {}
  /*!
   * @brief Generate uniformly spaced nodes.
   * @return Vector of node coordinates with `num_elements + 1` entries.
   */
  MeshNodes operator()() const override;

private:
  //! Requested number of elements.
  std::size_t M_num_elements;
};

/*!
 * @brief Generator for meshes with spatially varying element size.
 *
 * The generator accepts a strictly positive spacing function `h(x)` and builds
 * a mesh whose local density follows that target spacing. Internally it
 * integrates an auxiliary ODE involving `1 / h(x)` and then interpolates the
 * inverse map to recover the node positions.
 *
 * @pre The spacing function must satisfy `h(x) > 0` on the whole domain.
 */
class VariableSize : public OneDMeshGenerator
{
public:
  //! Function type used to prescribe the local target spacing.
  using SpacingFunction = std::function<double(double)>;
  /*!
   * @brief Construct a variable-size mesh generator.
   * @param domain Domain to be discretized.
   * @param h Spacing function prescribing the desired local mesh size.
   * @param max_num_elements Upper bound on the number of elements.
   */
  VariableSize(const Geometry::Domain1D &domain, SpacingFunction h,
               std::size_t max_num_elements)
    : OneDMeshGenerator(domain), M_h(std::move(h)), M_num_elements(max_num_elements)
  {}
  /*!
   * @brief Generate the mesh nodes.
   * @return Vector of node coordinates.
   *
   * The actual number of elements is determined adaptively and may be smaller
   * than `max_num_elements`.
   */
  MeshNodes operator()() const override;

private:
  //! Prescribed local spacing function.
  SpacingFunction M_h;
  //! Maximum number of elements allowed during generation.
  std::size_t     M_num_elements;
};
/*! @} */
} // namespace Geometry
#endif
