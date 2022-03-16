#include "rational.hpp"
#include <iostream>
#include <iomanip>
#include <set>
#include <unordered_set>
//===============================================
int
main()
{
  using namespace std;
  using namespace apsc; // my namespace
  cout << "Size of a Rational=" << sizeof(Rational) << " bytes" << endl;
  Rational a{-15, 6};
  Rational b{6,40};
  Rational c{b};
  Rational d;
  c = Rational(1);
  double aa = a;
  cout << a << endl;
  cout << ++a << endl;
  cout << b << endl;
  cout << c << endl;
  cout << aa << endl;
  cout << a + b << endl;
  cout << a + b << endl;
  cout << a + 1 << endl;           // it converts a to double
  cout << a + Rational(1) << endl; // explicit conversion!

  std::ratio<3,4> threeQuarter;
  Rational tq(threeQuarter);
  cout<< tq<<endl;
  std::cout<<std::boolalpha<<"is "<<tq<<" smaller than "<< b<<"? "<<(tq<b)<<std::endl;
  // Rational mu=std::micro();
  // cout<<" A micro is "<<mu<<endl;

  // Having rcomparison operators allow to have sets of Ratiomals
  // A set of 3 rationals built on the fly
  std::set<Rational> sr{Rational{3,4},Rational{-9,37},Rational{2,3}};
  // Having a hash function and == operator allows me to use unordered containers as well!
  std::unordered_set<Rational,apsc::RationalHash> usr{Rational{3,4},Rational{-9,37},Rational{2,3}};



  cout << " Give me a Rational in the form a/b (followed by a space):";
  cin >> a;
  cout << endl << "I have read " << a << endl;
}
