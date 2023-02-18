# An example about scoping rules #


Scopes are normally (but not always) opened and closed by `{` and `}`,
respectively, and can be nested.  A scope just introduced by `{` (and
closed by `}`) is called *block scope*, and the code contained within
a *block*.

A scope initiated by `namespace` is called a namespace scope. A class
also defines a scope (sometimes called class scope) for its members.

Names in namespace scope and static class members may be addressed
with their full qualified name: `std::vector`, for instance, indicates
that the name `vector` is in the namespace `std`.  The scope
resolution operator alone may be used to fully qualify a name in the
global scope. For instance, `::foo` indicates a name (of a variable or
of a function) in the global scope.

The use of qualified name may help to disambiguate names and avoid
name clashes.


Names defined in an outer scope will be imported in the inner scope.
But if they are redefined in the inner scope the new definition hides the imported
one. A scope is said to be a named scope if it has a name: named
scopes are those introduced by namespace or by a class.  The most
external scope (the one outside any {} ) is called the *global scope*.

A variable defined in an scope  exists only
within that scope, and it's destroyed when the code exit the scope in which
it has been defined. 

To avoid unwanted hiding of variables becouse of the scoping rules
it's better *using long and significant name* and *use namespaces* to
group names that refer to the same logical units or. For instance
`std` is the namespace used for all utilities of the Standard
Library. However, sometimes for more special objects, we have a nested
namespace. For instance `std::execution` is the namespace for all
execution policies of the Standard Library. `execution` is nested in
`std`.

Follow the same technique also in your code. Choose a namespace for
all your stuff, and possibly use nested namespaces to isolate more
specific or special parts.  This way you make your code clearer and
reduce the risk of name clashes.

Since C++17 we can replace

    namespace apsc{
        namespace Utilities{
with simply

    namespace apsc::Utilities{


# What do I learn here? #
- Some basics about scoping rules.
  
