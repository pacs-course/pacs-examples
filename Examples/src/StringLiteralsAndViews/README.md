#Strings and string literals#

Strings are only apparently *simple stuff*. First of all, in C++ we have the C++ style strings `std::string`, whose use is highly encouraged, and the C-style, null terminated, strings, which are in fact `char*`.

The fact that C-style strings are implicitly convertible to C++ strings may caus confusing. For instance

	std::string s="Hello World";
	
works fine since the null-terminated string literal "Hellp World" is converted (by constructor) to the C++ string. However, in

	auto s="Hello World";

variable `s` is a `char *`!

To indicate that a string literal should be interpreted as a C++ string you need to use `std::string_literals` and write

	auto s = "Hellp World"s;
	
Inside the belly of a C++ string we still have (as private member) a null-terminated C-type string, aka `char *`, that we can recall with the method `str()`. This fact implies that all the special characters that governs how a C string is interpreted are valid also in C++ strings. For instance `\n` indicates newline and when sent to a terminal or a file will cause moving to the next line. 
A complete list of special character may be found easily on the web.

A notable special character is the *null character* indicated by `\0`. The little program in this example shows that *null terminated string* means exactly what it says.

Sometimes there is the need to interpret character *verbatim*, for instance we want `\n` be printed as **\n** and not be interpreted as *move to the next line*.

To this purpose, C++ introduce the *raw string* specification, 

	R"c(string)c"
where `string` is the string you want to print verbatim, `c` is any character that is not present in `string`. For instance 

	R"!(Hello\0World)!";
	
prints the string **Hello\0World** verbatim, the `\0` is not interpreted as string terminator.

Many more things can be said about strings. For instance, you should remember that a C++ string is in fact a class template. It has at least one template paramater that indicates the type of character used by the string. It has a default, however, equal to `char`. So everytime you write `std::string` you are actually saying `std::string<char>`.  If you have to write in certain languages, like Mandarin Chinese, which require a large number of symbols to represent a sentence, maybe you need `std::string<widechar>`.

Another important question if you go multilingual is that of the encoding system, i.e. the map between the printable character and the number(s) used to represent it in memory (remember, in a computer we only have numbers..). But this will be too much for this example.

In this example we show also a possible use of string view. `std::string_view` is a non-owning reference to a sequence of characters. It was introduced with C++17 and serves several useful purposes:

1. **Performance Optimization:** It allows the creation of lightweight, non-owning references to strings without the need to copy the underlying string data. This can lead to performance improvements, as it avoids potentially expensive copy operations.

2. **Safer C-String Handling:** It provides a safer, more convenient way to work with C-style strings (`const char*`), by adding bounds-checked access and other features while still avoiding string copies.

3. **Function Parameter Type:** `std::string_view` can be used as a function parameter type when you want to accept any kind of string-like object without committing to a particular string type. This can enable functions to accept literals, `std::string`, `const char*`, and other types of strings in a very efficient way.

4. **String Slicing:** It can be used to refer to sub-strings or "slices" of a larger string without copying them. This is useful for parsing and tokenizing strings.

1. **Temporary String Views:** It allows the creation of temporary views over string literals or other temporary string expressions, which can be more efficient than creating temporary `std::string` objects.

It's important to note that since `std::string_view` does not own the string it references, care must be taken that the underlying string outlives the `string_view`. Otherwise, you could end up with a dangling reference, which could lead to undefined behavior. Sting views are useful in code that operates heavily on strings, such as parsers, compilers, and other text-processing applications.



#What do I learn here#
- Some pieces of information and curiosities about strings;
- How to interpret a string verbatim;
- The difference between C++ strings and C strings.



