// Copyright (C) 2009 VZLU Prague
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

DEFUN_DLD (fsave, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Loadable Function} {} fsave (@var{fid}, @var{var})\n\
Save a single variable to a binary stream, to be subsequently loaded with\n\
fload. Returns true if successful.\n\
@end deftypefn")
{
  octave_value retval;
  int nargin = args.length ();

  if (nargin == 2)
    {
      int fid = octave_stream_list::get_file_number (args(0));

      octave_stream octs = octave_stream_list::lookup (fid, "fsave");
      std::ostream *os = octs.output_stream ();

      octave_value val = args(1);

      if (os)
        {
          // FIXME: these are dummies. It would be nice to get rid of them and call
          // a more low-level interface, but there doesn't seem to be a more suitable
          // one in Octave, that still does the automatic type lookup.
          std::string doc, name = "tmp";
          bool floats = false, global = false;
          save_binary_data (*os, val, name, doc, global, floats);
        }
      else
        error ("fsave: stream not opened for writing.");
    }
  else
    print_usage ();

  return retval;
}
