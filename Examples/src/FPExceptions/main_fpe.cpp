#include <iostream>
#include <cmath>
#include <string>
#include "trapfpe.hpp"
double f1 (double);
double f2 (double);
int main()
{
    using namespace std;
    bool theEnd (false);
    bool isFpe (false);
    double a, b;
    bool on(false);
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
        std::cout << "Gimme 2 numbers a and b" << std::endl <<
                  "I will compute 1/a and log(b)" << std::endl;
        std::cin >> a >> b;
        try
        {
            double z = f1 (a);
            std::cout << " 1/" << a << " is " << z << std::endl;
            std::cout.flush();
            test_fpe_exception (on);
            double x = f2 (b);
            std::cout << " log(" << b << ") is " << x << std::endl;
            std::cout.flush();
            isFpe = test_fpe_exception (on);
            if (isFpe)
            {
	      std::cout << "Some floating point exception has occurred (undetected)" << std::endl;
            }
        }
        catch (std::runtime_error& x)
        {
            std::cout << x.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "Another (unknown) exception occurred" << std::endl;
        }
	std::cout << "Do you want to exit? (Y for yes) default=no" << std::endl;
	std::string input;
	cin.clear();
	cin>>input;
        theEnd = (input[0] == 'Y');
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

