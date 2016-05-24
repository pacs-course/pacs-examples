#include "abstract_quadrature.h"

quadrature_factory &
quadrature_factory::instance ()
{
  static quadrature_factory the_quadrature_factory;
  return the_quadrature_factory;
};

void
quadrature_factory::add (std::string const & name, builder_type const & func)
{
  auto f = storage.insert (std::make_pair (name, func));
  if (f.second == false)
    throw std::invalid_argument ("Double registration in Factory");
};

quadrature_handle
quadrature_factory::create (std::string const & name) const
{
  auto f = storage.find (name);
  return (f == storage.end ()) ?
    quadrature_handle () :
    quadrature_handle (f -> second ());    
};


