#include <type_traits>

int main()
{
    unsigned int i;

    const bool check = std::is_signed< decltype(i) >::value;

    typedef std::conditional< check, float, int>::type new_type;

    static_assert ( !std::is_same< new_type, float >::value, "Bad value" );

    return 0;
}
