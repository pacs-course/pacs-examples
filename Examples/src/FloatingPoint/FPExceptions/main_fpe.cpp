#include <iostream>
#include <cmath>
#include <string>
#include "trapfpe.hpp"
#include "fpeUtilities.hpp"
double f1 (double);
double f2 (double);
int main()
{
    using namespace std;
    bool theEnd{false};
    double a, b, c;
    bool on{false};
#ifndef FPE_ABORT
    cout << "Type 1 if you want to launch exception if a FPEs occurs: ";
    int ans;
    cin >> ans;
    on = (ans == 1);
    if (on)
    {
        cout << "FPE TRAP ACTIVATED" << endl;
    }
    else
    {
        cout << "FPE TRAP NOT ACTIVATED" << endl;
    }
#else
    cout<<"IT WILL ABORT ON FPE"<<std::endl;
#endif
    do
    {
        std::cout << "Gimme 3 numbers a, b and c" << std::endl <<
                  "I will compute 1/a, log(b) and exp(c)" << std::endl;
        std::cin >> a >> b>>c;
        try
          {
            double z = f1 (a);
            std::cout << " 1/" << a << " is " << z << std::endl;
            double x = f2 (b);
            std::cout << " log(" << b << ") is " << x << std::endl;
            double y = std::exp(c);
            std::cout << " exp(" << c << ") is " << y << std::endl;
            std::cout.flush();
            apsc::test_fpe_exception (on);
          }
        catch (std::exception& x)
          {
            std::cout << x.what() << std::endl;
          }
        catch (...)
          {
            std::cout << "Another (unknown) exception occurred" << std::endl;
          }
	std::cout << "Do you want to exit? (Y or y for yes) anithing else=no" << std::endl;
	std::string input;
	cin.clear();
	cin>>input;
        theEnd = (input== "Y" || input=="y");
    }
    while (!theEnd);
}

double f1 (double a)
{
    return 1. / a;
}

double f2 (double c)
{
    return std::log (c);
}

