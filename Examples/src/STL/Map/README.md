# std::map#

std::map is an associative container that reproduce the classic `dictionary`. In a `std::map` you store a pair *<Key,Value>*. The access of a value corresponding to a Key is fast (the opposite is not). In a `std::map` there must be an ordering relation for the keys. Indeed internally the map is usually stored as binary search tree.

In a `std::map` you cannot have repeated keys. If you need repeated keys you have to use `std::multimap`. The latter has and interface vesy similar to that of map, but but, for instance, the method `equal_range` returns all the elements with a given key. For consistency, the method is present olso in a `std::map`, but, of course, it will always return just one item (if the key is present).

In this example I show a few functionalities of maps, the principal one.

#What do I learn from here?#

- Some functionalities of the ordered map container of the Standard Library.      