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

  //! Convert an std::map<F,S>, where F, S are trivial types,
  //!  to a vector of bytes that can be saved to a binary file
  //!  or transmitted through a channel such as a socket or
  //!  sent as an MPI message.
  template <class F, class S>
  std::vector<unsigned char> 
  write (const std::map<F,S>& container)
  {
    static_assert (std::is_trivial<F>::value
                   && std::is_trivial<S>::value,
                   "Cannot serialize map of this type");
    size_t numel = container.size ();
    size_t size = (sizeof (F) + sizeof (S)) * numel;
    std::vector<unsigned char> res (size, 0);  

    auto destf = reinterpret_cast<F*> (&(res[0]));
    for (auto ii = container.begin ();
         ii != container.end (); ++ii, ++destf)
      *destf = ii->first;

    auto dests = reinterpret_cast<S*> (destf);
    for (auto ii = container.begin ();
         ii != container.end (); ++ii, ++dests)
      *dests = ii->second;
        
    return res;
  }

  //! Read in an std::map<F,S>, where F, S are trivial types,
  //!  from a vector of bytes.
  template <class F, class S>
  void
  read (const std::vector<unsigned char>& data,
        std::map<F,S> &container)
  {
    static_assert (std::is_trivial<F>::value
                   && std::is_trivial<S>::value,
                   "Cannot serialize map of this type");
  
    size_t size = (sizeof (F) + sizeof (S));
    size_t numel = data.size () * sizeof (unsigned char) / size;

    auto destf = reinterpret_cast<const F*> (&(data[0]));
    for (auto ii = 0; ii < numel; ++ii, ++destf)
      container[*destf];

    auto dests = reinterpret_cast<const S*> (destf);
    for (auto ii = container.begin ();
         ii != container.end (); ++ii, ++dests)
      ii->second = *dests;
  }

  //! Convert an std::vector<std::map<F,S>>, where F, S
  //!  are trivial types, to a vector of bytes that can
  //!  be saved to a binary file or transmitted through
  //!  a channel such as a socket or sent as an MPI message.
  template <class F, class S>
  std::vector<unsigned char> 
  write (const std::vector<std::map<F, S>>& container)
  {

    // sizeof (F) = nF
    // sizeof (S) = nS
    // sizeof (size_t) = nst
    // container.size () = N
    // container[i].size () = Ni
    //
    // memory layout:
    //   nst        N x nst       sum (Ni * (nF + nS),   i=0..N)
    // -------   ------------   ---------------------------------
    //    N       Ni, i=0..N          container[i],  i=0..N 
    //
    static_assert (std::is_trivial<F>::value
                   && std::is_trivial<S>::value,
                   "Cannot serialize map of this type");

    size_t N = container.size ();
    std::vector<size_t> tmp (N + 1);
    tmp[0] = (N);                                  // N
  
    for (int ii = 1; ii < N + 1; ++ii)
      tmp[ii]= container[ii-1].size ();            // Ni

    size_t ntotel = std::accumulate (tmp.begin () + 1,
                                     tmp.end (), 0);
    
    std::vector<unsigned char> res = write (tmp);
    res.reserve (ntotel * (sizeof (F) + sizeof (S)));
    for (int ii = 1; ii < N + 1; ++ii)
      {
        std::vector<unsigned char> tmp2 = write (container[ii-1]);
        res.insert (res.end (), tmp2.begin(), tmp2.end());
      }
  
    return res;
  }

  //! Read in an std::vector<std::map<F,S>>, where F, S
  //!  are trivial types, from a vector of bytes.
  template <class F, class S>
  void
  read (const std::vector<unsigned char>& data,
        std::vector<std::map<F,S>> &container)
  {
    static_assert (std::is_trivial<F>::value
                   && std::is_trivial<S>::value,
                   "Cannot serialize map of this type");
  
    size_t pair_size = (sizeof (F) + sizeof (S));
    const size_t *psize =
      (reinterpret_cast<const size_t*> (&(data[0])));
    size_t num_maps = *psize;
  
    container.resize (num_maps);
    std::vector<size_t> size_vec (num_maps);

    size_t ii = 0;
    for (ii = 0, ++psize; ii < num_maps; ++ii, ++psize)
      size_vec[ii] = (*psize);

    auto dests = reinterpret_cast<const S*> (psize);
    for (size_t kk = 0; kk < num_maps; ++kk)
      {
        auto destf = reinterpret_cast<const F*> (dests);
        for (auto ii = 0; ii < size_vec[kk]; ++ii, ++destf)
          container[kk][*destf];

        dests = reinterpret_cast<const S*> (destf);
        for (auto ii = container[kk].begin ();
             ii != container[kk].end (); ++ii, ++dests)
          ii->second = *dests;
      }
  }
}
