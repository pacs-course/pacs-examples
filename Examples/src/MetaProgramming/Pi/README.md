# Computing pi as a partial sum of a series #

There are may series that produce pi. Here I am using one of them to approximate pi. Template recursion allow me to compute sum the various terms of the series, stopping at a given term, and let the compiler do the job (You may need optimization on: compile with `make DEBUG=yes` and see that `nm --demangle` does not conteain `computePi`.

Note that I have used template recursion also to define my own pow for integer exponents.
I have also put in a namespace some constexpr that contain the value of pi at different level of precision.

# What do I learn here? #
- Another use of recursion with templates.
- The use of a constexpr function to define useful constants


**A technical note:** 

This is only an example, if you want pi in your code there are simpler ways:

- include <numbers> (c++20) where a lot of numerical constants are provided 
```c++
#include <numbers>
inline constexpr double pi=std::numbers::pi;
```
Maybe you want it with higher precision, then you need `pi_v`:
```c++
#include <numbers>
inline constexpr long double pi=std::numbers::pi_v<long double>;
```
*Note* I am using `inline` here as I assume that the statement is in a ehader file included in several translation units. With `inline` I effectively make `pi` a global variable by relaxing the one definition rule.

- Using a trigonometric function
 
This is the "old way" (cannot use `constexpr`)

 ```
 #include<cmath>
 const double pi = 2*std::acos(0.0);
 ```
 
 if you want a long double
 
 ```
 #include<cmath>
 const long double pi = 2*std::acos(0.0L); // 
 ```
 Rmember the `L` to activate the overload for long double! Otherwise you call `acos` for double and convert the result to long double, eventually having just the precision of a double!
 
 

 You cannot declare it constexpr (if you do it it is ignored) since `acos` is (obviously) not a constexpr function
 
 - Using the constant provided by the `cmath` header


 ```
 #include<cmath>
 constexpr double pi = M_PI;
 ```
This is what you would have done before c++20. Now you have `<numbers>` and you  can avoid this technique. The fact is that `M_PI` is a preprocessor macro, and in modern c++ we tend to use cpp macros only when strictly necessary. So use the first technique shown here.
