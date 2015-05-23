/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012 and modified by
 * Luca Formaggia, 2012.
 *
 * (C) Copyright Nicolai M. Josuttis 2012, Luca Formaggia, 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
/*!
  @file dist1.cpp
  @brief An example on use of random number generation and distributions.
  @note  Additional comments and a few additions by Luca Formaggia.
 */
#include <random>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>

//! A helper function to print the distribution
template <typename Distr, typename Eng>
void distr (Distr d, Eng & e, const std::string& name)
{
  // The engine produce integer numbers whose width depends on the
  // engine (and possibly the chosen engine template argummnts)
  // But we can extract the type from the engine itself.
  typedef typename Eng::result_type eng_result_type;
  // The same for the result type of the distribution
  typedef typename Distr::result_type distr_result_type;


  // print min, max and four example values
  std::cout << name << ":" << std::endl;
  std::cout << "- min():  " << d.min() << std::endl; 
  std::cout << "- max():  " << d.max() << std::endl; 
  std::cout << "- values: " << d(e) << ' ' << d(e) << ' '
	    << d(e) << ' ' << d(e) << std::endl; 

  // Make an istogram of the generated values
  // For simplicity we convert them to integers
  // As key we use long long to account for big numbers
  std::map<long long,int> valuecounter;
  for (int i=0; i<200000; ++i) {
    valuecounter[d(e)]++;
  }
 
    // and print the resulting distribution
    std::cout << "====" << std::endl;
    for (auto elem : valuecounter) {
        std::cout << std::setw(3) << elem.first << ": "
                                  << elem.second << std::endl;
    }
    std::cout << "====" << std::endl;
    std::cout << std::endl;
}

//! Example printing several distributions.
/*!
  All distribution have a defualt constructor which sets the parameter
  distribution to default values. Look at a reference manual to have a
  complete list of the possible arguments.
 */
int main()
{
  std::random_device re;
  // We use the knuth engine
  auto myseed=re();
  std::cout<<"The seed is "<<myseed<<std::endl;	
  std::knuth_b e(myseed);
  //std::random_device e;
  
  // Try the random engine
  
  // Note the <>. It is compulsory, since it is a 
  // full specialization of a class template
  std::uniform_real_distribution<> ud(0, 10);
  distr(ud,e,"uniform_real_distribution");
  
  // Default is mean 0 and standard deviation 1
  std::normal_distribution<> nd;
  distr(nd,e,"normal_distribution, mean 0, stdev 1");

  // Mean 1  Std dev 2.0
  std::normal_distribution<> nd2(1.0,2.0);
  distr(nd2,e,"normal_distribution, mean 1, stdev 2");
  
  std::exponential_distribution<> ed;
  distr(ed,e,"exponential_distribution (lambda=1.0)");
  
  std::gamma_distribution<> gd;
  distr(gd,e,"gamma_distribution");

  std::lognormal_distribution<> lnd;
  distr(lnd,e,"lognormal_distribution");

  std::weibull_distribution<> wd;
  distr(wd,e,"weibull_distribution");

  //This produces a very "elongated" distribution
  // so I have commented it out
  //  std::student_t_distribution<> sd;
  //distr(sd,e,"student_t_distribution");
}
