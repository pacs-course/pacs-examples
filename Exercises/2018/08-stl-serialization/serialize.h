#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

namespace serialize
{
  //! Convert an std::vector<T>, where T is a trivial type,
  //!  to a vector of bytes that can be saved to a binary file
  //!  or transmitted through a channel such as a socket or
  //!  sent as an MPI message.
  template <class T>
  std::vector<unsigned char> 
  write (const std::vector<T>& container)
  {
    static_assert (std::is_trivial<T>::value,
                   "Cannot serialize vector of this type");
  
    size_t numel = container.size ();
    size_t size = sizeof (T) * numel;
    std::vector<unsigned char> res (size, 0);  

    std::copy (container.begin (), container.end (),
               reinterpret_cast<T*> (&(res[0])));
  
    return res;
  }

  //! Read in a std::vector<T>, where T is a trivial type,
  //!  from a vector of bytes.
  template <class T>
  void
  read (const std::vector<unsigned char>& data,
        std::vector<T> &container)
  {
    static_assert (std::is_trivial<T>::value,
                   "Cannot serialize vector of this type");
  
    size_t size = sizeof (T);
    size_t numel = data.size () * sizeof (unsigned char) / size;
    container.resize (numel);  
    
    std::copy (data.begin (), data.end (),
               reinterpret_cast<unsigned char*> (&(container[0])));
  }

}
