/*
 * main.cpp
 *
 *  Created on: Jan 17, 2022
 *      Author: forma
 */
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <ranges>
struct Person
{
  std::string name;
  std::string surname;
  unsigned int age;
  // just to test the case of member function
  auto getAge()const {return age;}
};
auto & operator<<(std::ostream & out, Person const & p)
{
  out<<p.name<<" "<<p.surname<<" "<<p.age<<std::endl;
  return out;
}
int main()
{
  {
  std::vector a{3.,4.,5.,-9.,-3.9};
  // old way of sorting vector from smallest to largest element
  std::sort(a.begin(), a.end());
  }

  {
  std::vector a{3.,4.,5.,-9.,-3.9};
  // The ranges way. Less hassle with iterators when you operate on the whole container
  std::ranges::sort(a);
  }


  // Projections are unary functions that are meant to operate on elements of a range before
  // performing a standard algorithm. But they may be also pointers to member function or members.
  // ranges::sort takes a range (or a couple of iterators, a comparison operator and a projection!
  // Let's see how it works.
  std::vector<Person> friends{
    {"John","Waine",30},
    {"Julia","Roberts",50},
    {"Mark","Twain",90},
    {"Harry","Potter",18},
    {"Paola","Cortellesi",45}
  };

  {
    // sort by name using default comparison operator
    std::ranges::sort(friends,{},&Person::name);
    std::cout<<"\nSorted by name\n";
    for (auto const & p:friends)
      std::cout<<p;
    }

  {
    // sort by surname using default comparison operator
    std::ranges::sort(friends,{},&Person::surname);
    std::cout<<"\nSorted by surname\n";
    for (auto const & p:friends)
      std::cout<<p;
    }
  {
    // sort by age using the function member and greater than comparison operator
    std::ranges::sort(friends,std::greater<unsigned>(),&Person::getAge);
    std::cout<<"\nSorted by age (oldest to youngest\n";
    for (auto const & p:friends)
      std::cout<<p;
    }

  {
    // Anather more complex example of projections
    // and a use of a view. I compute v^2_i - v^2_{n-i}
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> result;
    std::ranges::transform(v,                       // first range
                           v | std::views::reverse, // second range
                           std::back_inserter(result), // insert in the result
                           std::minus<int>(), // operation on the elements of the two projected values
                           [](int v) { return v*v; }, // projection on the first range
                           [](int v) { return v*v; }); // projection on the II range
    std::ranges::copy(result,
                      std::ostream_iterator<int>(std::cout, ","));
    std::cout<<std::endl;
  }

  {
    // also a list is a range...
    std::list<double> l{3.,4.,-9.5,-6.};
    // extract the sum of all positive numbers (using a loop)
    auto gt0=[](double const & x){return x>0;};
    double sum{0};
    for (auto i: l|std::views::filter(gt0))
      sum+=i;
    std::cout<<"Sum is "<<sum<<std::endl;
    //
  }
  {
    // Now we sum the square of the values greater than 0
    std::list<double> l{3.,4.,-9.5,-6.};
    // extract the sum of all positive numbers (using a loop)
    auto gt0=[](double const & x){return x>0;};
    double sum{0};
    for (auto i: l |
        std::views::filter(gt0) |
        std::views::transform([](double x){return x*x;})
        )
      sum+=i;
    std::cout<<"Sum of sqares is "<<sum<<std::endl;
    //
  }

  {
    // Joining strings
   std::vector<std::string> course{"Advanced ","Programming ", "for ", "Scientific ", "Computing"};
   auto joined=std::ranges::join_view(course);
   for (auto const & c:joined)
     std::cout<<c;
   std::cout<<std::endl;
  }




  }





