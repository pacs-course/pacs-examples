#An example of a view (called also adaptor, or proxy)#

A view provides a different semantic to an *existing object*. It is an examle of an adaptor design pattern. It does not change the public interface of the viewd object, but it gives it a different meaning.


For instance, in this example we take an object of type Matrix
which can be **any Matrix type whose elements can be accesses with a call operator with two integer arguments representing the row and column indexes** (i.e.  `m(i,j)` is the value in location `i,j`).

The view allow to access the matrix with the index exchanged, so it provides its transpose.

The object in which a view operates is normally stores as a (lvalue) reference, so a view is normally not default constructible: it needs an object to be "attached to". 
In other words, we use aggregation via a reference. Remember that a reference must always bind to an object, we cannot have "dangling references".

The basic set up of this view is very simple (and sometimes enough!!). Here a sketch

```
template <typename M>
transposeView
{
public:
	transposeView(M& m):myM{m}{};
    double operator()(int i, int j)const {return myM(j,i);}
    double & operator()(int i, int j){return myM(j,i);}
private:
	M& myM;
} 
```

A main difficulty arises if you wish to be very general and treat const object properly. You may want that a transposeView of a constant Matrix be possible (with the solution above it is not!) but it should not allow to modify the matrix: if the viewd matrx is const I want that only the const version of `operator()(i,j)` be available!


Here we show some rather sophisticated way of achieving this using the `enable_if` and `is_const` type traits. Moreover, we provide a helper function to simplify the creation of the view. 

#What do I learn here?#
- That a simple solution, maybe sufficient for your problem at hand, becomes less simple if you wish to increase generality and be able to treat different situations;
- The use of some sophisticated type traits of the standard library;
- The use of traits to allow the user to change template parameter dependent types via specialization;
- *An example of the use of deduction guides (since c++17)*



