#computing pi as a partial sum of a series#

There are may series that produce pi. Here I am using one of them to approximate pi. Template recursion allow me to compute sum the various terms of the series, stopping at a given term, and let the compiler do the job (You may need optimization on: compile with `make DEBUG=yes` and see that `nm --demangle` does not conteain `computePi`.

Note that I have used template recursion also to define my own pow for integer exponents.
I have also put in a namespace some constexpr that contain the value of pi at different level of precision.

#What do I learn here?#
- Another use of recursion with templates.
- The use of a constexpr function to define useful constants


**A technical note:** 

This is only an example, if you want pi in your code there are simpler ways:

- Using a trigonometric function
 
 ```
 #include<cmath>
 double pi = 2*std::acos(0.0);
 ```
 
 if you want a long double
 
 ```
 #include<cmath>
 long double pi = 2*std::acos(0.0L); // 
 ```
 Rmember the `L` to activate the overload for long double! Otherwise you calll `sin` for double and convert the result to long double, eventually having just the precision of a double!
 
 

 You cannot declare it constexpr (if you do it it is ignored) since `acos` is (obviously) not a constexpr function
 
 - Using the constant provided by the `cmath` header
 
 ```
 #include<cmath>
 constexpr double pi = M_PI;
 ```
 
 This approach alllows to define `pi` as a constant expression  (as it is logically), but is uses a cpp macro, legacy of the C-library `math.h`, from which `<cmath>` has been derived. It contains all digits for a long double, so you may use it also if you want pi with the precision of a long double.
 