      subroutine open_stream (num, name)
      implicit none

      integer num
      character(255) name
      
      open (unit=num, file=name, access="STREAM", form="FORMATTED")

      end
