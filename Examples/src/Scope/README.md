#An example about scoping rules#


A scope is nomally opened and closed by { and } and can be
nested. Names defined in an outer scope will be imported in the inner
scope.  But if they are redefined in the inner scope they hide the
imported ones. A scope is said to be a named scope if it has a name:
named scopes are those introduced by namespace or by a class/struct.
The most external scope (the one outside any {} ) is called global
scope.

A variable defined in an scope (a part the global scope) exixts only
within the scope, and destroyed when the code exit the scope in which
it is defined.  A name defined in a named scope and hidden in an
enclosing scope may still be accessed using the scope resolution operator
`::` (i.e. by using its full qualified name).  

Name in the global scope (i.e. autside any user-defined scope) may be
recalled inside an internal scopo again with the scope resolution
operator without specifying anyting on the left: for example `::g`
indicates a global variable named `g`, i.e. a variable in the global scope.
