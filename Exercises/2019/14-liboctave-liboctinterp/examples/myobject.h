// Copyright (C) 2012 Carlo de Falco <carlo.defalco@gmail.com>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#include <octave/oct.h>

class myobject : public octave_base_value
{

 public:

  // Constructor
  myobject (const std::string _str = "", const int _intgr = 0, const double _dbl = 0.0)
    : octave_base_value (), str (_str), intgr (_intgr), dbl (_dbl) { p_str = &str; }
  
  void print (std::ostream& os, bool pr_as_read_syntax = false) const
  { os << "str : " << str << std::endl
       << "intgr : " << intgr << std::endl
       << "dbl : " << dbl << std::endl; }

  ~myobject(void) {  };

  bool is_defined (void) const { return true; }

  const std::string  & get_str (void) const;
  const int          & get_intgr (void) const;
  const double       & get_dbl (void) const;

  std::string * p_str;

  void set_str   (std::string & str);
  void set_intgr (int & intgr);
  void set_dbl   (double & dbl);
 
 private:
  
  std::string str;
  int intgr;
  double dbl;
  DECLARE_OCTAVE_ALLOCATOR;
  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA;

};

DEFINE_OCTAVE_ALLOCATOR (myobject);
DEFINE_OV_TYPEID_FUNCTIONS_AND_DATA (myobject, "myobject", "myobject");

static bool myobject_type_loaded = false;
