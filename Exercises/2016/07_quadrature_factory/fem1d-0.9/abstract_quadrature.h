#ifndef HAVE_ABSTRACT_QUADRATURE_H
#define HAVE_ABSTRACT_QUADRATURE_H 1

#include <string>
#include <memory>
#include <map>
#include <functional>


class
abstract_quadrature
{
public :
  virtual double
  operator() (std::function<double (double)> f, double a, double b) = 0;
};

typedef std::unique_ptr<abstract_quadrature> quadrature_handle;

class
quadrature_factory
{

public :
  
  typedef std::function<quadrature_handle ()>  builder_type;
  typedef std::map<std::string, builder_type> container_type;  

  static quadrature_factory & instance ();
  container_type storage;

  void
  add (std::string const & id, builder_type const & func);

  quadrature_handle
  create (std::string const & name) const;
  
private :
  
  quadrature_factory () = default;
  
  quadrature_factory (quadrature_factory const &) = delete;
  
  quadrature_factory &
  operator= (quadrature_factory const &) = delete;

};


template<typename factory, typename product>
class proxy
{
public :

  proxy (std::string const &);
  
  static quadrature_handle
  build ()
  {
    return quadrature_handle
      (new product ());
  };
  
private :
  
  proxy (proxy const &) = delete; 

  proxy & operator= (proxy const &) = delete;

};

template<typename F, typename C>
proxy<F,C>::proxy (std::string const & name)
{
  F & factory (F::instance ());  
  factory.add (name, &proxy<F,C>::build);
}

#endif
