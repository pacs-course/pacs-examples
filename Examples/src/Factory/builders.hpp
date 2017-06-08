#ifndef HH_BUILDERS__
#define HH_BUILDERS__
#include<type_traits>
#include <memory>



//! An example of abstract builder base class
/*!
  It forms the base for builders to be used with 
  object factories.

  It uses the default constructor. It would be nice to have
  a generic builder able to pass also arguments to the constructor.

  However it is veri difficult to build a generic one of that sort. Of
  course for a specific hyerarchy of classes you may construct a
  specific builder that hase several create() methods each taking
  different arguments, to be paseed to the constructor.

  Unfortunately, template methods cannot be virtual, otherwise the solution
  would be simple indeed!
 */
template<class AbstractProduct>
class
BaseBuilder
{
 public:

  virtual std::unique_ptr<AbstractProduct>  create()=0;
  virtual ~BaseBuilder(){};
};
//! Abstract builder
/*!
  It relies on the defualt constructor
  \pre ConcreteProduct must be defualt constructible
  \pre ConcreteProduct must derive from AbstractProduct
 */
template<class ConcreteProduct, class AbstractProduct>
class
Builder : public BaseBuilder<AbstractProduct>
{
public:
 static_assert(
		    std::is_base_of<AbstractProduct, ConcreteProduct>::value,
		    "Builder requires that AbstractProduct be a base of ConcreteProduct");
  std::unique_ptr<AbstractProduct> create(){return
      std::unique_ptr<AbstractProduct>(new ConcreteProduct());
  } 
};
#endif
