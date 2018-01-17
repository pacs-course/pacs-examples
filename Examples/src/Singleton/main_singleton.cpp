#include "Singleton.hpp"
class foo: private SingletonPrototype<foo>
{
public:
  //  foo()=default;
};
int main()
{
  foo & a=SingletonPrototype<foo>::Instance();
  foo b;
}
