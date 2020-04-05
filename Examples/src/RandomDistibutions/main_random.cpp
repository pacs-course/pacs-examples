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
#include <fstream>
#include "sampling.hpp"

//! A helper function to print the distribution
/*!
 *  It prints a simplified histogram for distribution d using engine e
 *  if file=true it will also produce a file with name file.dat with the
 *  values.
 *  By default it does not produce any file
 *  \param d  a distribution
 *  \param name A name for output and file
 *  \param file true is you want a file with generated numbers (def. false)
 *  \param hist true if you want a poor man histogram on the screen (true)
 */
template <typename Distr, typename Eng>
void distr (Distr & d, Eng & e, const std::string& name,bool file=false, bool hist=true)
{
  // The engine produce integer numbers whose width depends on the
  // engine (and possibly the chosen engine template argummnts)
  // But we can extract the type from the engine itself if we need it with
  // typedef typename Eng::result_type eng_result_type;
  // The same for the result type of the distribution:
  // typedef typename Distr::result_type distr_result_type;
  // Remember to use typename since we have template parameter dependent types
  //
  // Number of samples
  constexpr int N=20000;
  std::ofstream outfile;
  if (file)
    {
      std::string filename=name + std::string(".dat");
      outfile.open(filename);
      outfile<<N<<"\n";
    }
  // print min, max and four example values
  std::cout << name << ":" << std::endl;
  std::cout << "- min():  " << d.min() << std::endl; 
  std::cout << "- max():  " << d.max() << std::endl; 
  std::cout << "- some values: " << d(e) << ' ' << d(e) << ' '
	    << d(e) << ' ' << d(e) << std::endl; 
  std::vector<typename Distr::result_type> values;
  values.reserve(N);
  for (int i=0; i<N; ++i)
    {
      values.emplace_back(d(e));
      if(file)outfile<<values.back()<<"\n";
    }
  if(file)outfile.close();
  // Make an istogram of the generated values
  // For simplicity we convert them to integers
  // As key we use long long to account for big numbers
  if (hist)
    {
      std::map<long long,unsigned int> valuecounter;
      for (int i=0; i<N; ++i)
        {
          auto value = values[i];
          valuecounter[value]++;// this works!
        }
      // print the resulting distribution on the screen
      std::cout << "====" << std::endl;
      for (auto elem : valuecounter) {
        std::cout << std::setw(3) << elem.first << ": "
                  << elem.second << std::endl;
      }
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
  // Set it to true if you also want the files
  bool file=true;
  std::random_device re;
  // Try the random engine
  auto myseed=re();
  //unsigned int myseed=123456789;
  std::cout<<"The seed is "<<myseed<<std::endl;	
  // We use the knuth engine
  std::knuth_b e(myseed);
  // uncomment if you want the random device
  //std::random_device e;
  
  
  // Note the <>. It is compulsory, since it is a 
  // full specialization of a class template
  std::uniform_real_distribution<> ud(0, 10);
  distr(ud,e,"uniform_real_distribution",file);
  
  // Default is mean 0 and standard deviation 1
  std::normal_distribution<> nd;
  distr(nd,e,"normal_distribution_mean_0_stdev_1",file);

  // Mean 1  Std dev 2.0
  std::normal_distribution<> nd2(1.0,2.0);
  distr(nd2,e,"normal_distribution_mean_1_stdev_2",file);
  
  std::exponential_distribution<> ed;
  distr(ed,e,"exponential_distribution_lambda=1",file);
  
  std::gamma_distribution<> gd;
  distr(gd,e,"gamma_distribution",file);

  std::lognormal_distribution<> lnd;
  distr(lnd,e,"lognormal_distribution",file);

  std::weibull_distribution<> wd;
  distr(wd,e,"weibull_distribution",file);

  //This produces a very "elongated" distribution
  //so I am not printing on the scrren
  std::student_t_distribution<> sd;
  distr(sd,e,"student_t_distribution",true,false);
  // Now a discrete distribution. The constructor
  // may take different forms, in this case we give the
  // weights for the values
  std::discrete_distribution<int> ddist{1.,3.,6.,1.,1.,10.};
  distr(ddist,e,"discrete_distribution",true,true);
  //
  // **************** THE SHUFFLING PART
  // To show it works on any container I usa a map
  std::map<int,std::string> dict={
      {1,"Mario"},
      {2,"Giulio"},
      {3,"Maria"},
      {4,"John"},
      {5,"Matilde"},
      {6,"Paola"},
      {7,"Anna"},
      {9,"Giovanni"},
      {10,"Beatrice"},
      {11,"Juliette"}
  };
  // Extract 5 at random
  auto sampled = apsc::sampleContainer(dict,5);
  std::cout<<"Extracted sample\n";
  for (auto & i: sampled) std::cout<<"("<<i.first<<", "<<i.second<<"); ";
  std::cout<<std::endl;
  // now shuffle
  // It is a map! Element are not swappable
  // Move to a vector
  std::vector<std::pair<int,std::string>> sampleVec{sampled.begin(),sampled.end()};
  //! We can move it, we do not need it afterwards
  auto shuffled = apsc::shuffleContainer(std::move(sampleVec));
  std::cout<<"Shuffled sample\n";
  for (auto & i: shuffled) std::cout<<"("<<i.first<<", "<<i.second<<"); ";
  std::cout<<std::endl;
  
}
