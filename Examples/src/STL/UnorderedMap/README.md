# std::unordered_map#

std::map is an associative container that reproduce the classic `dictionary`. In a `std::unordered_map` you store a pair *<Key,Value>*. The access of a value corresponding to a Key is fast (the opposite is not). In a `unordered_std::map` there must be an equivalence relation for the keys and a hash function. Indeed internally the map is usually stored as an hash.

In a `std::unordered_map` you cannot have repeated keys. If you need repeated keys you have to use `std::unordered_multimap`. The latter has and interface vesy similar to that of map, but but, for instance, the method `equal_range` returns all the elements with a given key. For consistency, the method is present olso in a `std::unordered_map`, but, of course, it will always return just one item (if the key is present).

In this example I show a few functionalities of maps, the principal one.

#What do I learn from here?#

- Some functionalities of the unordered map container of the Standard Library.      