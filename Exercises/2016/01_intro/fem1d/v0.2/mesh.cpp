#include "mesh.h"

mesh::mesh
(const double a, const double b, const unsigned int nnodes_)
  : L (b - a), nnodes (nnodes_),
    nels (nnodes - 1), h (L / double (nels))
  {
    
    nodes = new double [nnodes];
    for (unsigned int ii = 0; ii < nnodes; ++ii)
      nodes[ii] = static_cast<double>(ii) * h + a;
    
    elements = new unsigned int [nels][2];
    for (unsigned int ii = 0; ii < nels; ++ii)
      {
        elements[ii][0] = ii;
        elements[ii][1] = ii+1;
      }
  }

