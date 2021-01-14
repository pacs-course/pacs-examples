#include<array>
#include<iostream>
/*!
 * This is an aggregate
 */
struct aggr
{
    double a;
    int b;
};
/*!
 * aggregates can have base classes (since c==17)
 */
struct aggr2 : aggr
{
    int c;
};
/*!
 * We can use brace initialization to return an aggregate
 * @return an aggregate of type aggre
 */
aggr anAggregate()
{
    return {3.1415, 7};
}

/*!
 * an array is an aggregate!
 * @return an array of 2 ints
 */
std::array<int,2> anArray2()
{
    return {1, 2};
}

/*!
 * Overloading streming operator to print arrays (it can be made better)
 * @tparam T The type contained in the array
 * @tparam N The size of the array
 * @param out the output stream
 * @param myarray The array
 * @return the output stream
 */
template <class T, std::size_t N>
std::ostream& operator << (std::ostream& out, std::array<T, N> const& myarray)
{
    out << "Array dimension:" << N << "\n";
    out << "Array content:  :" << std::endl;
    for (auto i : myarray)
    {
        out << i << " ";
    }
    out << std::endl;
    return out;
}



/*!
 * It is only to show the capabilities of tuple_size!
 * In this case the size is computed at compile time, differently
 * than using size().
 *
 * @tparam Array An array type
 * @param a The array
 * @param out An output stream
 * @return The output stream
 */
template <class Array>
std::ostream & printSize (const Array& a, std::ostream& out = std::cout)
{
    out << "Array dimension:" << std::tuple_size<Array>::value << "\n";
    return out;
}

/*!
 * A function that takes a pointer to an array of double
 * Old C and C++98 style. To show how you can use C++ standard
 * and vectors with legacy code
 * @param a a pointer to an array
 * @param n The size of the array
 */
void fun (double const* a, int n)
{
    std::cout << "Array dimension:" << n << "\n";
    std::cout << "Array content: ";
    for (int i = 0; i < n; ++i)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    using namespace std;
    array<int, 3> b{1, 2, 3}; //brace initialization (array is an aggregate!!)
    array<double, 3> c = {1., 2., 3.}; //brace-equal initialization (internal braces may be omitted)
    // You can have an aggregate of aggregates, and it is an aggregate!
    array<aggr, 3> bb{ 5.0, 3, 6.3, 4,  -1.0, 7};
    array<double, 4> d; // Default initialization
    cout << "Array b:" << b <<
         "Array c:" << c <<
         "Array d:" << d; // Note that d is NOT initialised to zero!
    // you can also do this
    std::array<aggr2, 3> ss {bb[0], 10, bb[1], 20, bb[2], 30};
    printSize(ss);
    std::cout<<" of aggregates aggr2\n";
    // structured bindings may help to retrieve the elements of an aggregate in one go
    auto [a1, a2] = anAggregate();
    std::cout << "The aggregate contains:" << a1 << " " << a2 << std::endl;
    auto [v1, v2] = anArray2();
    std::cout << "The array contains:" << v1 << " " << v2 << std::endl;

}
