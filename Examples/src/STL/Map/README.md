# std::map#

std::map is an associative container that reproduce the classic `dictionary`. In a `std::map` you store a pair *<Key,Value>*. The access of a value corresponding to a Key is fast (the opposite is not). In a `std::map` there must be an ordering relation for the keys. Indeed internally the map is usually stored as binary search tree.

In a `std::map` you cannot have repeated keys. If you need repeated keys you have to use `std::multimap`. The latter has and interface vesy similar to that of map, but but, for instance, the method `equal_range` returns now *all the elements* with a given key. For consistency, the method is present also in a `std::map`, but, of course, it will always return just one item (if the key is present).

In this example I show the main functionalities of maps. I also show the `extract` utility, that allows to change an element of a map (and in fact any non-sequential container) without the need of deleting the old element and inserting the new element. Using the `extract` utility you save memory operation, and this can be a substantial gain if the stored elements are of large size.

#What do I learn from here?#

- Some functionalities of the ordered map container of the Standard Library.      