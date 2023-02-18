# Integer overflow/underflow: is 0 - 1 = -1? #

Integer overflows and underflows produce unwanted results. **And they cannot be detected!  Beware!**

The nastier case is with unsigned integers. If you by mistake subtract
to an unsigned integer a value greater than that of the unsigned... well you get
an insigned integer whose value *is certainly not what you expected*.

A classic example of this mistake is when you want to
loop a vector in reverse direction and you do something like

```
for (std::size_t i=v.size()-1;i>=0u;--i){
    v[i]=...
    ..
    }
```

This loop **never ends**: `std::size_t` is an unsigned integer (normally an `unsigned long int`), when
it reaches `0` you still subtract `1` from it at the end of the loop. You think to get `-1`, and the end of the loop. But
instead.... run the program to see.

The probelm is that

````
0u - 1u = max representable unsigned integer!
````


It`s better using reverse iterators! They have been made for this purpose:

`````
for (auto i=v.rbegin(); i!=v.rend();--i){
    *i=...
    ..
    }
`````

**Note** To avoid this hassle often people use `int` instead of `unsigned int` even when the integer should never go negative. However, remember that
maximum value of an `int` is half that of an `unsigned int`. Often it doesn't matter, but it may if you need big integers...   

## A nice utility in `Utilities`##
Another situation where you can get in trouble when using unsigned integers is
when you have to compute the absolute difference of two integers and you are using unsigned int.

The following code **is wrong!**
```
#include<cmath>
unsigned i=10u;
unsigned j=20u;
unsigned result=std::abs(i-j);
```
Why? Think about it. In the `Utilities` folder you have `absdiff.hpp` that provide
a tool to compute the absolute difference safely also for unsigned.


## What do I learn here? ##
- That computer algebra is not equal to ordinary algebra, even for integers. You just have to be careful.