#ifndef HAVE_MESH_H
#define HAVE_MESH_H

template<class prec = double> 
class mesh
{

public:

  const unsigned int nnodes;
  const prec L;
  const unsigned int nels;
  const prec h;

  prec *nodes;
  unsigned int (*elements)[2];

  mesh
  (const prec a, const prec b, const unsigned int nnodes_);

  ~mesh ()
  {
    delete [] nodes;
    delete [] elements;
  }
  
};

template<class prec>
mesh<prec>::mesh
(const prec a, const prec b, const unsigned int nnodes_)
  : L (b - a), nnodes (nnodes_),
    nels (nnodes - 1), h (L / double (nels))
  {
    
    nodes = new prec [nnodes];
    for (unsigned int ii = 0; ii < nnodes; ++ii)
      nodes[ii] = static_cast<prec>(ii) * h + a;
    
    elements = new unsigned int [nels][2];
    for (unsigned int ii = 0; ii < nels; ++ii)
      {
        elements[ii][0] = ii;
        elements[ii][1] = ii+1;
      }
  }


#endif

