# Reading a vector as a stream #

An example on how to read a vector as if it were a stream. I have also overloaded the comma operator to enable filling the vector using a syntax similar to the one used in the Eigen library for linear algebra.

Note the use of variadic templates to have the maximum level of generality: std::vector has more than one template parameter, all but the first have a default value. But, since I want this tool to work for any std::vector, and I do not know how many template parameters we have (the standard just says that a std::vector has *at least* two template parameters), I have decided to use variadic templates and let the compiler deduce them (thanks to template argument deduction).

A different version may use a different template parameter for the input variable: in this version the input variable has exactly the same type of the one stored in the vector. Since implicit conversion is not allowed in template arguments, conversion must be done outside. Just as an example, if `v` is a `vector<double>`

`v<<1;`

will not compile since `1` is an int. But of course I can do `v<<1.;`.

This is an example of complex overloading of operators by using templates. In fact, there is no need of this tool. Parameter list (brace) initialization already allow to initialize a vector in a simple way. But in other cases it may be useful to have little tools of this type. For instance, the streaming operator can be used also run-time and when the size of the vector is not known beforehand.

# What do I lean here? #
- An unusual overload of the output stream operator;
- A curious usage of overloading the comma operator;
- A curious usage of variadic templates.
