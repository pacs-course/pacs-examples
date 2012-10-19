#include <iostream> 

class MVerr { };                         // a base class
class Overflow: public MVerr { };        // overflow

class IntOverflow: public Overflow { };     // integer overflow
class FloatOverflow: public Overflow { };   // floating number overflow

class SmallDivisor2: public MVerr { };    // small or zero divisor
class NoMatch: public MVerr { };          // matrix vector size not match

void f() {
  try {
    // ... try some code
    throw IntOverflow();

  } catch (Overflow) {
    // ... handle integer, floating number overflow and derived exceptions
    std::cout << "handling overflow" << std::endl;

  } catch (MVerr) {
    // ... handle MVerr errors that are not Overflow
  }
}


void g() {
  try {
    // ... try some code
    throw NoMatch();

  } catch (Overflow) {
    // ... handle integer, floating number overflow and derived exceptions
    std::cout << "handling overflow" << std::endl;

  } catch (MVerr) {
    // ... handle MVerr errors that are not Overflow
    std::cout << "handling non-overflow matrix-vector exceptions" << std::endl;
  }
}


int main() {
  f();
  g();
}
