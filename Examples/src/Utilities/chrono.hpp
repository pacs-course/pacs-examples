//! An example of use of the C++11 chrono library.

/*
This file illustrate the use of the ctime library,

Luca Formaggia 2005. New version using C++11 (2015)

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
#include<chrono>
#include<iosfwd>

namespace Timings{
  
  class Chrono
  { 
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> stopTime;
  public:
    //! Outputs time from last start and stop
    friend std::ostream & operator <<(std::ostream &,Chrono const &);
    Chrono();
    //!Explicitly defaulted automatic consts/assignements
    // Not needed, just to show the use of default!
    Chrono(const Chrono &)=default;
    Chrono(Chrono&&)=default;
    Chrono & operator=(Chrono&&)=default;
    Chrono & operator=(const Chrono&)=default;
    //! Starts/reset  counting time
    void start();
    //! Stops counting time
    void stop();
    //!Outputs wall time between last start and stop (in microseconds)
    double wallTime() const;
    //!Outputs wall time between last start and now! (in microseconds)
    double wallTimeNow() const;
  };
}

#endif
