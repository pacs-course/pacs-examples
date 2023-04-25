# A look to smart pointers # 

* `polyFactory_simple(Shape):` a simple object factory for Polygons (the Polygon classes are defined in `Polygon.[h|c]pp`). By selecting an enumerator that indicates which concrete polygon you want, the function returns the pointer to the base class pointing to the concrete object of the wanted type. The object is default constructed.

* `PolyFactory(Stape, Args...)` This more sophisticated factory uses variadic tempalte to enable constructing the wanted concrete object possibly passing constructor arguments. See **Note** below.

* `enable_shared`. A simple class only to show the enable_shared
utility of the standard library. It is a simple utility: if you
inherit a class from `enable_shared<Class>` (it uses the Curiously
Recurring Template technique) you enable the method
`shared_from_this()` which produces a shared pointer to the object. It
is a useful technique because it avoids to make mistakes. If you want
to share a resource, you can create the shared pointer using this
utility and you are sure you are doing the right thing.

* `main_smart_pointer` a code that shows the use of the shared pointers
using the classes defined above 

* `main_simple` another main code with a simple example showing the
working of smart pointers.  A Container class is composed
polymorphically with an object of a polymorphic family, selected using
a simple object factory .  

A much more complex example of composition with
polymorphyc object, which uses cloneable classes, is in
`Utilities/CliningUtilities.hpp`: it employs a `PointerWrapper` class that  simplifies the composition with polymorphic object (provided they are  clonable classes). Have a look!

# A note #
The more sophisticated factory is much flexible since it allows to construct the object using one of its constructors, not necessarily the default one. However, you need to have care to avoid syntax errors. 
The base class (`AbstractPolygon` in this case) must provide the interface **to all possible constructors of the derived class**, mybe an empty interface. All derived classes should then use
```````
using AbstractPolygon::AbstractPolygon
```````
to bring all constructor interface in scope, maybe then redefining some of them.

This is necessary due to the way compiler checks code. It is a nuisance, I know, but I have not found a workaround. If you want furhet details look at the bottom


## What do I get from this example? ## 
- An overview of smart pointer usage.
- A (not so) simple design of a factory of polygons
- A usage of enable_shared utility
- A usage of "inherited constructors"


# Details on delegating constructors, why is needed here? #

It is not against the 'is-a' principle if a derived class provides additional constructors, specialised for the derived class. Actually it is often the case. 

 For instance, in my polymorphic family of Polygons, `Square` can be constructed also passing a `Point2D` as origin , and two doubles for the length and the rotation angle,

``````
  Square(Point2D const &origin, double length, double angle = 0.0);
``````
a constructor not present, for instance in `Triangle`.

So far so good. However, I here wanted to exploit variadic templates in order to design an object factory where I can pass arguments to the constructor:

``````c++
template <class... Args>
std::unique_ptr<AbstractPolygon>
polyFactory(Shape s, Args&&... args)
{
  switch(s)
    {
    case Shape::Triangle:
        return std::make_unique<Triangle>(std::forward<Args>(args)...);
    case Shape::Square:
        return std::make_unique<Square>(std::forward<Args>(args)...);
    case Shape::Polygon:
        return std::make_unique<Polygon>(std::forward<Args>(args)...);
    default:
        return nullptr;
    }
}
``````
It looks nice; indeed I can now do 
````````c++
auto s = polyFactory(Shape::Square,origin, length,angle);
````````
to generate a square with 
``````c++
std::make_unique<Square>(origin, length,angle);
``````
which, in turn, calls the corresponding constructor of `Square`.

Unfortunately however, there is a problem! When the compiler instantiate
`polyFactory(Shape::Square,origin, length,angle)` it has to check if the code of the generated function template is correct.

Unfortunately, that instance of the `polyFactory` template will contain

````````
make_unique<Triangle>(origin,length,angle);
````````
which require a constructor which is not available in `Triangle`.

It is irrelevant the fact that that line is not called, because of the `switch` statement: **it is incorrect if `Triangle` has not a constructor
taking a `Point2D` and two doubles!** 

The only solution is then to add a dummy constructor of that sort in the base class `AbstractPolygon`, and let all derived class `inherit` the constructor defined in `AbstractPolygon` with

````````c++
using AbstractPolygon::AbstractPolygon;
````````

Of course, `Square` will redefine the constructor for the square

````c++
  Square(Point2D const &origin, double length, double angle = 0.0);
````

while the other derived classes will not.

The problem is that now I can in principle call
``````c++
Triangle t{origin,length,angle}
``````
which does not make sense. I need to rely on the good usage of the code by part of the user.

**Note:** If you find a workaround to this problem, please let me know.
