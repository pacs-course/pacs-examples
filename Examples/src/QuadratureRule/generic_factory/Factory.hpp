#ifndef RULES_FACTORY_HPP_
#define RULES_FACTORY_HPP_
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
namespace GenericFactory{
  
  /*! A generic factory. 
    
    It is implemented as a Singleton. The compulsory way to 
    access a method is RuleFactory::Instance().method().
  */
  template
  < 
    typename AbstractProduct,
    typename Identifier,
    typename Builder=std::function<std::unique_ptr<AbstractProduct> ()>
  >
  class Factory{
  public:
    //! The container for the rules.
    typedef  AbstractProduct AbstractProduct_type;
    //! The identifier.
    /*  
	We must have an ordering since we use a map with 
	the identifier as key.
    */
    typedef  Identifier Identifier_type;
    //! The builder type.
    /*
      The default is a function. In C++98 I would have used
      a function pointer. Here I prefer to use a function wrapper instead.
      I return a unique_ptr (C++11). This can make the handling more complex
      but it is much safer with respect to memory leaks. A simpler version
      may use a bare pointer and return type.
    */
    typedef  Builder Builder_type;
    //! Method to access the only instance of the factory
    static Factory & Instance();
    //! Get the rule with given name . 
    /*!
      The pointer is null if no rule is present.
    */
    std::unique_ptr<AbstractProduct> create(Identifier const & name) const;
    //! Register the given rule.
    void add(Identifier const &, Builder_type const &);
    //! Returns a list of registered rules.
    std::vector<Identifier> registered()const;
    //! Unregister a rule.
    void unregister(Identifier const & name){ _storage.erase(name);}
    //! Destructor
    ~Factory()=default;
  private:
    typedef std::map<Identifier,Builder_type> Container_type;
    //! Made private since it is a Singleton
    Factory()=default;
    //! Made private since it is a Singleton
    Factory(Factory &&)=default;
    //! Made private since it is a Singleton
    Factory & operator =(Factory &&)=default;
    //! It contains the actual object factory.
    Container_type _storage;
  };

  
  //! We use the Meyer's trick to istantiate the factory.
  template
  <
    typename AbstractProduct,
    typename Identifier,
    typename Builder
    >
  Factory<AbstractProduct,Identifier,Builder> & 
  Factory<AbstractProduct,Identifier,Builder>::Instance() {
    static Factory theFactory;
    return theFactory;
  }
  
  template
  <
    typename AbstractProduct,
    typename Identifier,
    typename Builder
    >
  std::unique_ptr<AbstractProduct> 
  Factory<AbstractProduct,Identifier,Builder>::create(Identifier const & name) 
    const {
    auto f = _storage.find(name); //C++11
    return (f == _storage.end()) ? std::unique_ptr<AbstractProduct>(): 
      std::unique_ptr<AbstractProduct>(f->second());
  }

  template
  <
    typename AbstractProduct,
    typename Identifier,
    typename Builder
    >
  void  
  Factory<AbstractProduct,Identifier,Builder>::add(Identifier const & name, 
					   Builder_type const & func){
    auto f = 
      _storage.insert(std::make_pair(name, func));
    if (f.second == false)
      throw std::invalid_argument("Double registration in Factory");
  }
  
  
  template
  <
    typename AbstractProduct,
    typename Identifier,
    typename Builder
    >
  std::vector<Identifier>  Factory<AbstractProduct,Identifier,Builder>::registered() 
    const {
    std::vector<Identifier> tmp;
    tmp.reserve(_storage.size());
    for(auto i=_storage.begin(); i!=_storage.end();++i)
      tmp.push_back(i->first);
    return tmp;
  }
}


#endif /* BC_FACTORY1_HPP_ */
