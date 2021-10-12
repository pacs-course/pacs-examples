#ifndef __HH_MESHREADERFACTORY_HH
#define __HH_MESHREADERFACTORY_HH
#include "Factory.hpp"
#include "MeshReaders.hpp"
#include <string>
namespace Fem
{
using MeshReaderFactory = GenericFactory::Factory<MeshReader, std::string>;
//! Get the factory of mesh readers
/*
  To be called only once in the program.
 */
MeshReaderFactory const &
getMeshReaderFactory()
{
  MeshReaderFactory &mRF(MeshReaderFactory::instance());
  mRF.add(
    "Simple",
    auto []() { return std::unique_ptr<MeshReader>(new MeshReadSimple); });
  mRF.add(
    "Triangle",
    auto []() { return std::unique_ptr<MeshReader>(new MeshReadTriangle); });
  mRF.add(
    "Dummy", auto []() { return std::unique_ptr<MeshReader>(new DummyMesh); });
  return mRF;
}
} // namespace Fem

#endif
