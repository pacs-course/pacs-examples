#An example of serialization and binary i/o#

**Serialization** is a term used to indicate a procedure that maps an object to a buffer so that it can be saved on disk or other storage device and retrieved. 
 For simple object this is trivial. For instance, serialization of a `std::vector` is simple since data in a vector is strored in a contigous buffer, so what I need it to save the lenght and the buffer, and I am able to recover the vector. If I have a `std::set` instead things is a bit more involved. The simplest thing is to copy all set elements into a vector and then store the vector. When reading, you do the opposit operation.
 
In this example we indeed show how to write and read a binary file that serializes a vector and a set of the standard library. We show also that trivially copy-constructible objects can be also trivially serialized.

Serialization is a big issue. Here I show only the simplest cases. In general, you want to serialize in order to recover the state of your objects after a save. And your objects can be quite complex. If you want to be able to serialize your classes you have to think about it beforehand and use tools like [Boost serialize]("https://www.boost.org/doc/libs/1_77_0/libs/serialization/doc/tutorial.html") or [cereal]("https://uscilab.github.io/cereal/") (now with less bits :). They can greatly simplify things when dealing with complex objects.
