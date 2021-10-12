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
#include <iostream>

namespace Timings
{
void
Chrono::start()
{
  startTime = stopTime = MyClock::now();
}

void
Chrono::stop()
{
  stopTime = MyClock::now();
}

double
Chrono::wallTime() const
{
  using namespace std::chrono;
  auto time_span = duration_cast<nanoseconds>(stopTime - startTime);
  return time_span.count() / 1000.;
}

double
Chrono::wallTimeNow() const
{
  using namespace std::chrono;
  auto time_span = duration_cast<nanoseconds>(MyClock::now() - startTime);
  return time_span.count() / 1000.;
}

std::ostream &
operator<<(std::ostream &out, Chrono const &c)
{
  auto oldf = out.flags();
  out << "Elapsed Time= " << std::scientific << c.wallTime() << " microsec"
      << std::endl;
  out.flags(oldf);
  return out;
}

} // namespace Timings
