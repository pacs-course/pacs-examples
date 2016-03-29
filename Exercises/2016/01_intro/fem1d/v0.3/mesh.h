#ifndef HAVE_MESH_H
#define HAVE_MESH_H

template<unsigned int nnodes>
class mesh
{

public:

  const double L;
  const unsigned int nels;
  const double h;

  std::array<double, nnodes> nodes;
  std::array<std::array<unsigned int, 2>, nnodes - 1> elements;

  mesh (const double a, const double b) : L (b - a),
                                          nels (nnodes - 1),
                                          h (L / double (nels))
  {
    for (unsigned int ii = 0; ii < nnodes; ++ii)
      nodes[ii] = static_cast<double>(ii) * h + a;

    for (unsigned int ii = 0; ii < nels; ++ii)
      {
        elements[ii][0] = ii;
        elements[ii][1] = ii+1;
      }
  }
  
};

#endif
