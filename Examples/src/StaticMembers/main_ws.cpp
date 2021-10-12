#include "class_ws.hpp"
#include <iostream>
#include <vector>

int
main()
{
  // static functions may be called without an object
  // using the scope resolution operator :: and the name
  // of the class
  std::cout << " N. of WS: " << WS::howmany() << std::endl;
  std::cout << " Average value: " << WS::average() << std::endl;

  WS aWS(3);
  std::cout << " N. of WS: " << WS::howmany() << std::endl;
  std::cout << " Average value: " << WS::average() << std::endl;
  // I create a scope
  {
    //! 10 object of type WS with value 5.0
    std::vector<WS> pp(10, 5.0);
    std::cout << " N. of WS: " << WS::howmany() << std::endl;
    std::cout << " Average value: " << WS::average() << std::endl;
    // pp is now destroyed!
  }
  std::cout << " N. of WS: " << WS::howmany() << std::endl;
  // since now we have objects I may also use it as a normal method
  std::cout << " Average value: " << aWS.average() << std::endl;
  //! I destroy the last one explicitely. Only to see what happens
  aWS.~WS();
  std::cout << " N. of WS: " << WS::howmany() << std::endl;
  std::cout << " Average value: " << WS::average() << std::endl;
}
