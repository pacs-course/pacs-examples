#Composite Design Pattern#

A composite pattern allows to build a container (composite) of
components that may be either a container or basic components.
Indeed, it allows to build dynamically a tree structure that is then
traversed, and each component executed. If the component is in fact a
container, it will loop over its components, executing them.

To allow a safe handling of memory I use ``unique_ptr<>``, and I
implement deep copy/assigmements with the method ``clone()`` (virtual
construction).

This version of the example does not use the ``Wrapper`` class
contained in the ``Utility`` folder.


*Note how the use of the prototype pattern (clone) simplify
things when writing the copy constructor and assignement of COmposite.*
