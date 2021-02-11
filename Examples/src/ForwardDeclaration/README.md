#Use of forward declaration#

This is a very simple example that indicates when forward declarations
are needed. Here forward declaration is necessary because class `A` has
a method that takes `B&`, while `B` stores a member of type `A`!

Often this situation it's a sign of poor design, but there are occasions
where this type of "cross referencing" is necessary.

The type of the parameter of a function/method is an incomplete
type. In other words, in the declaration of a function/method (not in
the definition!) the compiler does not need to have all the details of
the type of its parameters or return value. He needs only to know that
they are proper types, whose full declaration however can be
postponed.

Note that in the case of class `B` we need instead to have at disposal
the full definition of `A`, so we have to include the corresponding
header file containing the full definition. Indeed, class `B` has a
variable of type `A` as its member, therefore the compiler needs to
know the size of `A` to deduce the size of `B`. Just a forward
declaration of `A` is here insufficient, and it would lead to a
compiler error.

#What do I learn here?#
That forward declaratiosn are sometimes necessary.