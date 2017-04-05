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
// sysconf(_SC_CLK_TCK);

namespace Timings{

  const long Chrono::_CPS=sysconf(_SC_CLK_TCK);
  
  Chrono::Chrono():_t1(0),_ct1(0),_wallTime(0),_cpuTime(0){}
  
  void Chrono::start(){
    using namespace std;
    tms temp;
    times(&temp);
    _t1 = (temp.tms_utime+temp.tms_stime);
    _ct1= temp.tms_cutime+temp.tms_cstime;
  }
  
  void Chrono::stop()
  {
    using namespace std;
    tms temp;
    times(&temp);
    _cpuTime= double((temp.tms_utime+temp.tms_stime)-_t1)/_CPS;
    _wallTime=_cpuTime+
      double(temp.tms_cutime+temp.tms_cstime-_ct1)/_CPS;
  }

std::ostream & operator <<(std::ostream & out,Chrono const &c)
{
  //out.setf(std::ios_base::scientific, std::ios_base::floatfield);
  out<<"Total CPU Time= "<<c._wallTime<<" s, CPU Time="<<c._cpuTime<<" s"<<std::endl;
  return out;
}

}


