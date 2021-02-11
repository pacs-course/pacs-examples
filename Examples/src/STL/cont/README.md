# A few of examples on use of different containers#

Some taken from the code associated to the book *The C++ Standard Library - A Tutorial and Reference, 2nd Edition by Nicolai M. Josuttis, Addison-Wesley, 2012*, an excellent reference on the
Standard Library (a new edition has come out extended to
C++17). The full set of codes contained in the book are [available on the web](http://www.cppstdlib.com/) with many more things!.

  * **deque1** It'a an example of `deque` the double ended queue standard
    container. It is a linear data structure where you
    can extract and insert elements efficiently at both ends and it
    allows to address a specific element with the addressing (`[]`)
    operator. Useful if you indeed need the flexibility of adding elements
    at both ends. If not, it's better to use a std::vector
    
  * **multimap1** multimaps (here we show the ordered version, you
    have also `std::unordered_multimap`) are useful to create dictionaries,
    i.e. *key->value* maps, where keys can be repeated (if not, you
    have to use `std::map` instead).
    
  * **multiset1** Example of `std::multiset`. It is a set where
    equivalent elements can be repeated. This is indeed the only
    difference w.r.t. `std::set`. Here we show the ordered version,
    that requires an orderring relation among the elements (like in
    the multimap1 example). By default is `std::less`, which
    implements *less tan*. But in the example it is shown how you can
    change the ordering relation by adding a comparison operator as
    second templare argument.
    
  * **oddeven** An ordering relation introduces the concept of
    equivalence classes: if `R` denoted an ordering relations, two
    element of a well ordered set are eqivalent if `! (a R b) && ! ( b
    R a)` (I have used c+++ notation for logical operators). So if I
    use an ordering relations for integer elements using `modulo-2`
    arithmetic two integer will be equivalent if they are congruent
    modulo-2. The fact that I can specify the ordering relation when
    creating a `std::set` or a `std::multiset` con be exploited in
    that sense, as shown in the example, where a create a multiset
    specifying the ordering relation ` a (mod 2) < b (mod2)`.

  * **ostream_iterator** This example shows the use of a streaming
    iterator, in particular an output streaming iterator.
    
  * **set1** an example of `std::set` a very nice container for
    ordered sets (with no repetition). It means thet there are no
    equivalent elements in a `std::set`. If you take two elements of a
    `set::set` either `a R b` or `b R a`, being `R` the chosen
    ordering (the default is `<`).
    
  * **setcmp1** Another example of ser prescribed ordering. If you
      pass the comparison operator as argument in the constructor of a
      `std::set` (or `std::multiset`) you may also decide which
      comparison to make at run time. In thi example, when creating
      the set you pass a object that contains the comparison operator,
      whose behavior may be changed by selecting the proper value of
      an enumerator.
      
  * **unordset1** An example of `std::unordered_set`. This container
    implements an hash type structure. You need to have defined for
    the elements an equivalence relation (defaulted to
    `std::equal_to`, which normally implements `==`) and an *hash
    function*.  Luckily, for POD, strings and tuples of PODS the
    standard library already implements both, so you have to do
    nothing. For other types you have to define a proper equivalence
    operator and hash function yourself.
  

