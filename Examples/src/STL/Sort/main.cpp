#include "compare.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

int
main()
{
  using namespace std;

  // ************** compare
  vector<std::string> s = {"Apple",     "applejuice", "milk",
                           "milKshake", "APPLE",      "Milk"};

  // useless : it is the default!!
  cout << " Normal sort (case sensitive)" << endl;
  std::sort(s.begin(), s.end(), CaseCompare());
  for(auto const &i : s)
    cout << i << " ";
  cout << endl;

  cout << " Sorted ignoring case" << endl;
  //  std::sort(s.begin(),s.end(),NocaseCompare());
  std::sort(s.begin(), s.end(), Nocase_locale());
  for(auto const &i : s)
    cout << i << " ";
  cout << endl;

  cout << " Now using lambdas" << endl;
  s = {"Apple", "applejuice", "milk", "milKshake", "APPLE", "Milk"};

  // useless : it is the default!!
  std::sort(s.begin(), s.end(),
            [](string const &a, string const &b) { return a < b; });
  cout << " Normal sort (case sensitive)" << endl;
  for(auto const &i : s)
    cout << i << " ";

  cout << endl;

  auto nocomp = [](std::string const &a, std::string const &b) {
    // Lexycografic comparison
    for(std::size_t i = 0; i < std::min(a.size(), b.size()); ++i)
      {
        if(std::toupper(a[i]) != std::toupper(b[i]))
          return std::toupper(a[i]) < std::toupper(b[i]);
      }
    return (a.size() < b.size());
  };
  std::sort(s.begin(), s.end(), nocomp);
  cout << " Sorted ignoring case" << endl;
  for(auto const &i : s)
    cout << i << " ";
  cout << endl;

// Here the new constrained algorithm
#if __cplusplus >= 202002L
  cout << " Now using the new constrained algorithm, ignoring cases" << endl;
  s = {"Apple", "applejuice", "milk", "milKshake", "APPLE", "Milk"};
  std::ranges::sort(s, nocomp); // much simpler!
  for(auto const &i : s)
    cout << i << " ";
  cout << endl;
  // a different possibility, using projectors
  // I sort the string by the first letter only
  cout << " Now using the new constrained algorithm, sorting by first letter "
          "and not ignoring case"
       << endl;
  std::ranges::sort(s, {}, [](std::string const &a) { return a[0]; });
  // The {} indicate to use the default comparison, which is here less<char
  // I show the  std::ranges::copy to print the vector
  std::ranges::copy(s, std::ostream_iterator<std::string>(std::cout, " "));
  // but you can also use for_each
  // std::ranges::for_each(s, [](auto const &i) {std::cout << i << " "; });
  /* the same with a for loop
  for(auto const & i :s)
    cout << i << " ";
*/
  cout << endl;
  std::cout << "The result may look strange... but remember that upper case "
               "letters are \"smaller\" than lower case ones\n";

#endif
}
