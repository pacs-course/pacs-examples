#ifndef HAVE_MESH_H
#define HAVE_MESH_H

class mesh
{

public:

  const unsigned int nnodes;
  const double L;
  const unsigned int nels;
  const double h;

  double *nodes;
  unsigned int (*elements)[2];

  mesh
  (const double a, const double b, const unsigned int nnodes_);

  ~mesh ()
  {
    delete [] nodes;
    delete [] elements;
  }
  
};

#endif
