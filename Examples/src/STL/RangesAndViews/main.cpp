/*
 * main.cpp
 *
 *  Created on: Jan 17, 2022
 *      Author: forma
 */
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <ranges>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>
struct Person
{
  std::string  name;
  std::string  surname;
  unsigned int age;
  // just to test the case of member function
  auto
  getAge() const
  {
    return age;
  }
};
auto &
operator<<(std::ostream &out, Person const &p)
{
  out << "Name:" << p.name << " Family Name:" << p.surname << " Age: " << p.age
      << " years" << std::endl;
  return out;
}

/*!
   std::span is a view of a contiguous range of elements
   Thos steaming operator works for array, vectors and fixed size C-arrays of
   doubles!
*/
auto &
operator<<(std::ostream &out, std::span<double> const &p)
{
  // the ranges::for_each is a for_each that works on ranges
  std::ranges::for_each(p, [&out](auto const &x) { out << x << " "; });
  return out;
}

/*!
  A constrained function that prints the first n elements of a range
  The concept std::ranges::range is a concept that is satisfied by all the
  ranges (i.e. containers, views, etc.)
  @param out the output stream
  @param r the range
  @param n the number of elements to print
  @return the output stream
*/
auto &
printn(std::ostream &out, std::ranges::range auto const &r, std::size_t n)
{
  std::ranges::for_each(r | std::ranges::views::take(n),
                        [&out](auto const &x) { out << x << " "; });
  return out;
}

/*!
  @brief A function that estracts the digits from a string
  "number1,number2,number3,..."
  @param csv a string containing a list of numbers separated by commas
  @return a vector containing the numbers joined together
*/
auto
join_numbers(std::string_view const &csv)
{
  // let csv="10,11,12"
  auto digits = csv | std::views::split(',') // [ [1, 0], [1, 1], [1, 2] ]
                | std::views::join;          // [ 1, 0, 1, 1, 1, 2 ]
  std::vector<int> numbers;
  for(auto const &d : digits)
    /*
    The expression d - '0' is converting the character d to its corresponding
    integer value. In ASCII, the characters '0' to '9' are represented by the
    values 48 to 57. So, subtracting the ASCII value of '0' (which is 48) from
    the ASCII value of a digit character gives you the integer value of that
    digit. For example, if d is the character '7', then d - '0' is 55 - 48 which
    equals 7.
    */
    numbers.emplace_back(d - '0');
  return numbers;
}

int
main()
{
  {
    std::vector a{3., 4., 5., -9., -3.9};
    // the old way of sorting vector from smallest to largest element
    std::sort(a.begin(), a.end());
  }

  {
    std::vector a{3., 4., 5., -9., -3.9};
    std::cout << "Original vector: " << a << std::endl;
    // The ranges way. Less hassle with iterators when you operate on the whole
    // container
    std::ranges::sort(a);
    // but you could use also std::ranges::sort(a.begin(),a.end())
    // and you also have the parallel version
    // std::ranges::sort(std::execution::par,a)
    std::cout << "Sorted vector: " << a << std::endl;
  }

  {
    // Projections are unary functions that are meant to operate on elements of
    // a range before performing a standard algorithm. But they may be also
    // pointers to member function or members. ranges::sort takes a range (or a
    // couple of iterators, a comparison operator and a projection! Let's see
    // how it works.
    std::vector<Person> friends{{"John", "Waine", 30},
                                {"Julia", "Roberts", 50},
                                {"Mark", "Twain", 90},
                                {"Harry", "Potter", 18},
                                {"Paola", "Cortellesi", 45}};

    {
      std::cout << "Original vector of persons\n";
      for(auto const &p : friends)
        std::cout << p;
      std::cout << std::endl;
      // sort by name using default comparison operator
      // The {} in the second argument is the default comparison operator
      std::ranges::sort(friends, {}, &Person::name);
      std::cout << "\nPersons sorted by name\n";
      for(auto const &p : friends)
        std::cout << p;
      std::cout << std::endl;
    }
    {
      // sort by surname using default comparison operator
      std::ranges::sort(friends, {}, &Person::surname);
      std::cout << "\nSorted by surname\n";
      for(auto const &p : friends)
        std::cout << p;
    }

    {
      // sort by age using the function member and greater than comparison
      // operator
      std::ranges::sort(friends, std::greater<unsigned>{}, &Person::getAge);
      std::cout << "\nSorted by age (oldest to youngest\n";
      for(auto const &p : friends)
        std::cout << p;
    }
  }
  std::cout << std::endl;
  {
    // Partition a vector into odd end even
    std::ostream_iterator<int> out{
      std::cout, ", "}; // an output iterator with a space as separator
    std::vector<int> v{-3, 7, 9, 0, 2, 1, 4, 3, 7, 6, 5, 8, 9};
    std::cout << "Original vector:  \t";
    std::ranges::copy(v, out); // the use of copy to print!
    // first the elements for which the predicate returns true
    auto tail = std::ranges::partition(v, [](int i) { return i % 2 == 0; });

    std::cout << "\nPartitioned vector: \t";
    std::ranges::copy(std::ranges::begin(v), std::ranges::begin(tail), out);
    std::cout << "│ ";
    std::ranges::copy(tail, out);
    std::cout << std::endl;
  }

  {
    // Sort a vector (the simplest case!)
    std::ostream_iterator<int> out{std::cout, " "};
    std::vector<int>           v{-3, 7, 9, 0, 2, 1, 4, 3, 7, 6, 5, 8, 9};
    std::cout << "Original vector:  \t";
    std::ranges::copy(v, out);

    std::ranges::sort(v);

    std::cout << "\nSorted vector: \t";
    std::ranges::copy(v, out);
    std::cout << std::endl;
  }

  {
    // Sort a vector (with a projector)
    std::ostream_iterator<int> out{std::cout, " "};
    std::vector<int>           v{-3, 7, 9, 0, 2, 1, 4, 3, 7, 6, 5, 8, 9};
    std::cout << "Original vector:  \t";
    std::ranges::copy(v, out);

    // default comparison, squared
    std::ranges::sort(v, {}, [](int x) { return x * x; });

    std::cout << "\nSorted vector^2: \t";
    std::ranges::copy(v, out);
    std::cout << std::endl;
  }

  {
    // Another more complex example of projections
    // and a use of a view. I compute v^2_i - v^2_{n-i}
    std::ostream_iterator<int> out{std::cout, " "};
    std::vector<int>           v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "Original vector:  \t";
    std::ranges::copy(v, out);
    std::vector<int> result;
    std::ranges::transform(
      v,                          // first range
      v | std::views::reverse,    // second range
      std::back_inserter(result), // insert in the result
      std::minus<int>(), // operation on the elements of the two projected
                         // values
      [](int v) { return v * v; },  // projection on the first range
      [](int v) { return v * v; }); // projection on the II range
    std::cout << "v^2_i - v^2_{n-i}\t";
    std::ranges::copy(result, std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;
  }

  {
    // also a list is a range...
    std::ostream_iterator<int> out{std::cout, " "};
    std::list<double>          l{3., 4., -9.5, -6.};
    std::cout << "Original list:  \t";
    std::ranges::copy(l, out);
    // extract the sum of all positive numbers (using a loop)
    auto   gt0 = [](double const &x) { return x > 0; };
    double sum{0};
    for(auto i : l | std::views::filter(gt0))
      sum += i;
    std::cout << "Sum of positive numbers is " << sum << std::endl;
    //
  }
  {
    // Now we sum the square of the values greater than 0
    std::ostream_iterator<int> out{std::cout, " "};
    std::list<double>          l{3., 4., -9.5, -6.};
    std::cout << "Original list:  \t";
    std::ranges::copy(l, out);

    // extract the sum of the squares of just the positive numbers (using a
    // loop)
    auto   gt0 = [](double const &x) { return x > 0; };
    double sum{0};
    auto   v = l | std::views::filter(gt0) |
             std::views::transform([](double x) { return x * x; });
    // This works since we have a common range
    // sum=std::reduce(v.begin(),v.end());
    // in general you should do this
    auto vc = v | std::views::common; // create a common view
    sum = std::reduce(vc.begin(), vc.end());
    // for (auto i: v)
    //   sum+=i;
    std::cout << "Sum of squares of positive numbers is " << sum << std::endl;
    //
  }

  {
    // Joining strings
    using namespace std::literals;
    std::vector<std::string> course{"Advanced "s, "Programming "s, "for "s,
                                    "Scientific "s, "Computing"s};
    auto                     joined = course | std::views::join;
    std::string              joinedString{joined.begin(),
                             joined.end()}; // you hace to copy the string
    std::cout << joinedString;
    std::cout << std::endl;
  }

  {
    // Extracting the digits from a string of integer numbers
    std::string csv{"10,11,12,7,8,908"};
    auto        digits = join_numbers(csv);
    std::cout << "The digits are: ";
    for(auto i : digits)
      std::cout << i << " ";
    std::cout << std::endl;
  }

  {
    // Some examples of conditional views
    const auto v = std::vector{3, 2, 2, 1, 0, 2, 1};
    const auto is_negative = [](int i) { return i < 0; };

    std::cout << "The vector  ";
    if(std::ranges::none_of(v, is_negative))
      {
        std::cout << "contains only positive numbers\n";
      }
    else if(std::ranges::all_of(v, is_negative))
      {
        std::cout << "contains only negative numbers\n";
      }
    else if(std::ranges::any_of(v, is_negative))
      {
        std::cout << "contains at least one negative number\n";
      }
  }
  { // A new way to loop map values isung the values range adapter (it applies
    // also to vectors of pairs)
    std::unordered_multimap<int, std::string> ages_of_friends{
      {30, "John"}, {30, "Marc"},  {50, "Julia"},
      {90, "Mark"}, {18, "Harry"}, {45, "Paola"}};
    using namespace std::views; // to simplify a little bit the code
    std::cout << "The names of my friends older than 45 years are: ";
    for(std::string const &name :
        ages_of_friends | filter([](auto const &p) { return p.first > 45; }) |
          values)
      std::cout << name << " ";
    std::cout << std::endl;
  }
  {
    // Materializing a vector from a view
    std::vector<double> v{1., 2., 3.e-15, -4.e-18, -5., 6.3, -7.2e-20, 8., 2.7};
    using namespace std::views;
    constexpr double cut = 1.e-10;
    auto             large = [cut](double x) { return std::abs(x) > cut; };
    auto             square = [](double x) { return x * x; };
    auto conv = v | filter(large) | transform(square); // here common non needed
    std::vector<double> conv_vector{conv.begin(), conv.end()};
    std::cout << "The square of elements greater than tolerance is: ";
    for(auto i : conv_vector)
      std::cout << i << " ";
    std::cout << std::endl;
    // Now I take the first two elements and insert them in another vector
    auto first_two = conv | take(2) | common; // common is needed here
    std::vector<double> first_two_vector{first_two.begin(), first_two.end()};
    std::cout << "The first two elements are: ";
    for(auto i : first_two_vector)
      std::cout << i << " ";
    std::cout << std::endl;
  }
  {
    // How to create a view from a pair of iterators
    /*
    You have a pair of iterators that define a sequence and you vant to create a
    vew in order to use and standard contrained algorithms or any user defined
    algorithm that may take a view as input. For example I want to all values
    greater then five associated to a key of a multimap
    */
    std::multimap<int, double> m{{1, 3.}, {1, 4.}, {3, 5.}, {4, 6.},
                                 {1, 7.}, {3, 8.}, {1, 9.}};
    auto interval = m.equal_range(1); // interval is a pair of iterators
    auto maprange = std::ranges::subrange(interval.first, interval.second);
    // mapview is a view that over the sequence identifies by the iterators in
    // interval
    std::cout << "The values associated to key 1 greater than 5 are: ";
    using namespace std::views;
    // I compose the view with the values view that extracts just the values
    // frem the elements of the map and the filter view that selects the values
    // greater than 5
    for(auto const v :
        maprange | values | filter([](double x) { return x > 5.; }))
      {
        std::cout << v << " ";
      }
    std::cout << std::endl;
  }
}
