#The optional class#

`std::optional<T>` is a new feature of C++ that allows to mark a value as "unset". Useful when you want to have a way of indicating missing or invalid data. 

In this example we show  the main features of this simple class template, and we read form a file where some data is missing.

We recalle thea an optional is convertible to bool, giving true if a valied data is present. The default constructor sets the optional as invalid. You can set it to a value just by using the assignment operator, while retrieving the value can be done by dereferencing the optional (as it were a pointer) or with the method `value()`. Dereferencing a invalid optional gives undefined behaviour, so always check for valid data before.

The value `std::nullopt` indicates the invalid data.

