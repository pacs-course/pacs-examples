#include <functional>
#include <iostream>
using Funct=std::function<double (double const &)>;
void fun(Funct const & f)
{
  std::cout<<f(5.0)<<std::endl;
}
struct FunHandler
{
  FunHandler(Funct const & f):M_f(f){}
  void operator()()
  {
    fun(this->M_f);
  }
private:
  Funct const & M_f;
};


int main()
{
  const double a=10.;
  auto f=[a](double const & y){return a*y;};
  FunHandler fh(f);
  fh();
}
