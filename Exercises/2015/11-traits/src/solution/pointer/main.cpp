#include <type_traits>
#include <iostream>

template <typename T >
void printValue ( const T& t )
{
    static_assert ( !std::is_pointer<T>::value, "Bad value" );
    std::cout << t;
}

int main()
{
    int a = 9;
    printValue(a);

    int * b = new int(7);
    printValue(b);

    return 0;
}
