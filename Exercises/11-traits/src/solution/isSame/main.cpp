#include <type_traits>

template <typename T>
struct B
{
    B ()
    { static_assert( std::is_same<T,int>::value || std::is_same<T,double>::value, "Bad value" ); }

};

int main()
{
    B<double> b;

    B<char> a;

    return 0;
}
