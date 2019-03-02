// Copyright (C) 2009 VZLU Prague, a.s., Czech Republic
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

// Author: Jaroslav Hajek <highegg@gmail.com>

#include <octave/oct.h>
#include <octave/mach-info.h>
#include <octave/ls-oct-binary.h>
#include <octave/oct-stream.h>

DEFUN_DLD (fload, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {@var{var} =} fload (@var{fid})\n\
Loads a single variable of any type from a binary stream, where it was previously\n\
saved with fsave.\n\
@end deftypefn")
{
  octave_value retval;
  int nargin = args.length ();

  if (nargin == 1)
    {
      int fid = octave_stream_list::get_file_number (args (0));

      octave_stream octs = octave_stream_list::lookup (fid, "fload");
      std::istream *is = octs.input_stream ();

      if (is)
        {
          // FIXME: these are dummies. It would be nice to get rid of them and call
          // a more low-level interface, but there doesn't seem to be a more suitable
          // one in Octave, that still does the automatic type lookup.
          std::string doc, filename;
          bool swap = false, global;

          std::string varname =
          read_binary_data (*is, swap, oct_mach_info::native_float_format (),
                            filename, global, retval, doc);

          std::cerr << varname << " = " << retval.double_value () << std::endl;

          if (retval.is_undefined ())
            error ("fload: failed to extract value");
        }
      else
        error ("fload: stream not opened for reading.");
    }
  else
    print_usage ();

  return retval;
}
