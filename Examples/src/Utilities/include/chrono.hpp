/*
This file illustrate the use of the ctime library,

Luca Formaggia 2005.

This file is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This file is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this file; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef _CHRONO_H_INCLUDE_
#define _CHRONO_H_INCLUDE_

#include <sys/times.h> // the standard ctime funcions
#include <iostream> // used because of operator<<
#include <unistd.h>

namespace Timings{
  
  class Chrono
  { 
    clock_t _t1;
    clock_t _ct1;
    double _wallTime;
    double _cpuTime;
    static const long _CPS;
  public:

    friend std::ostream & operator <<(std::ostream &,Chrono const &);

    Chrono();
    //! Starts counting time
    void start();
    //! Stops counting time
    void stop();
    //!Outputs wall time
    inline double wallTime() const{return _wallTime;}
    //!Outputs CPU time
    inline double cpuTime() const {return _cpuTime;}
    //! Pretty print
  };
}

#endif

