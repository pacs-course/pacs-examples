#A look to smart pointers# 

* `polyFactory:` a simple object factory for Polygons (the Polygon classes are defined in `Polygon.[h|c]pp`).

* `enable_shared`. A simple class only to show the enable_shared
utility of the standard library. It is a simple utility: if you
inherit a class from `enable_shared<Class>` (it uses the Curiously
Recurring Template technique) you anable the method
`shared_from_this()` which produces a shared pointer to the object. It
is a useful technique because it avoids to make mistakes. If you want
to share a resource, you can create the shared pointer using this
utility and you are sure you are doing the right thing.

* `main_smart_pointer` a code that shows the use of the shared pointers
using the classes defined above 

* `main_simple` another main code with a simple example showing the
working of smart pointers.  A Container class is composed
polymorphically with an object of a polymorphic family, selected using
a simple object factory .  A more complex example of composition with
polimorphyc object, which uses clonable classes, is in
`Utilities/test_clonable.cpp`: it employs a `Wrapper` class that
simplify the composition with polymorphic object (provided they are
clonable classes). Hve a look!

