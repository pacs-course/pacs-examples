/*
 * LockClasses.hpp
 *
 *  Created on: Aug 25, 2022
 *      Author: forma
 */

#ifndef AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_LOCKS_LOCKCLASSES_HPP_
#define AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_LOCKS_LOCKCLASSES_HPP_
#include <omp.h>

class NoLocked{
public:
  void update_c(){counter++;}
  int howMany()const {return counter;}
private:
  int counter{0};
};

class Locked{
public:
  Locked()
  {
    omp_init_lock(&myLock);
  }
  void update_c(){
    omp_set_lock(&myLock);
    counter++;
    omp_unset_lock(&myLock);
  }
  int howMany()const {return counter;}
private:
  int counter{0};
  omp_lock_t myLock;
};




#endif /* AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_LOCKS_LOCKCLASSES_HPP_ */
