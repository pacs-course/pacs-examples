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

#include "chrono.hpp"
#include<iostream>

namespace Timings{
  
  Chrono::Chrono():
    startTime(std::chrono::steady_clock::now()),
    stopTime(startTime)
  {}
  
  void Chrono::start()
  {
    startTime=stopTime=std::chrono::steady_clock::now();
      }
  
  void Chrono::stop()
  {
    stopTime=std::chrono::steady_clock::now();
  }
  
  double Chrono::wallTime() const
  {
    using namespace std::chrono;
    using duration = std::chrono::duration<double>;
    duration time_span=duration_cast<duration>(stopTime-startTime);
    return time_span.count();
  }

  double Chrono::wallTimeNow() const
  {
    using namespace std::chrono;
    using duration = std::chrono::duration<double>;
    duration time_span=duration_cast<duration>(stopTime-steady_clock::now());
    return time_span.count();
  }

  std::ostream & operator <<(std::ostream & out,Chrono const &c)
  {
    out<<"Elapsed Time= "<<c.wallTime()<<" s"<<std::endl;
    return out;
  }
  
}


