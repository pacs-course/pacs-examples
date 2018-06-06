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
#include <myobject.h>

void load_myobject_type ()
{
  myobject::register_type ();
  myobject_type_loaded = true;
  mlock ();
  std::cout << "myobject_type_loaded" << std::endl;
}

const std::string & myobject::get_str () const
{ return str; } 


void myobject::set_str (std::string & _str)
{ str = _str; }


// PKG_ADD: autoload ("myobject_get_str", "myobject.oct");
DEFUN_DLD (myobject_get_str, args, ,
           "get the str field") 
{
  octave_value retval;
  octave_idx_type nargin = args.length ();
  if (nargin < 1 || nargin > 1)
    print_usage ();
  else
    {

      if (! myobject_type_loaded)
        load_myobject_type ();

      if (args(0).type_id () == myobject::static_type_id ())
        {
          const myobject & mo = static_cast<const myobject&> (args(0).get_rep ());
          retval = octave_value (mo.get_str ());
        }
    }
  return retval;
}

// PKG_ADD: autoload ("myobject_init", "myobject.oct");
DEFUN_DLD (myobject_init, args, ,  "initialize a myobject") 
{
  octave_value retval = 0;

  octave_idx_type nargin = args.length ();
  if (nargin != 3)
    print_usage ();
  else
    {
      if (! myobject_type_loaded)
        load_myobject_type ();
        
      std::string str = args(0).string_value ();
      int i = args(1).int_value ();
      double dbl = args(2).double_value ();
      
      if (!error_state)
        {
          retval = new myobject (str, i, dbl);
        }
    }
  return retval;
}


// PKG_ADD: autoload ("myobject_set_str", "myobject.oct");
DEFUN_DLD (myobject_set_str, args, nargout,
           "set the str field") 
{
  octave_value retval;
  octave_idx_type nargin = args.length ();
  if (nargin < 2 || nargin > 2)
    print_usage ();
  else
    {

      if (! myobject_type_loaded)
        load_myobject_type ();
      
      std::string str = args(1).string_value ();
      if (args(0).type_id () == myobject::static_type_id ())
        {
          const myobject mo  (static_cast<const myobject&> (args(0).get_rep ()));
          *(mo.p_str) = str;
        }
    }
  return retval;
}
