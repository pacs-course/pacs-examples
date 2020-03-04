#Some examples of using the type trait utilities introduced since C++11:#

* `fselect`. A template class that returna a value even if the template parameter is a pointer. A use of std::remove_pointer and of std::is_pointer;
* `move_swap`. My implementation of the swap utility. Use the one given by the standard library, this is just an example of possible implementation
* `main_traits` Testing the previous utilities and also shows an example of usage of std::is_base_of<B,D>