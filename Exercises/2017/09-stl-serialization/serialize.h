#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

template <class T>
std::vector<unsigned char> 
write (const std::vector<T>& container)
{
  size_t numel = container.size ();
  size_t size = sizeof (T) * numel;
  std::vector<unsigned char> res (size, 0);  

  std::copy (container.begin (), container.end (),
             reinterpret_cast<T*> (&(res[0])));
  
  return res;
}

template <class T>
std::vector<T> 
read (const std::vector<unsigned char>& data)
{
  size_t size = sizeof (T);
  size_t numel = data.size () * sizeof (unsigned char) / size;
  // std::cout << "data size = " << data.size () << std::endl;
  // std::cout << "numel = " << numel << std::endl;
  std::vector<T> container (numel);  
    
  std::copy (data.begin (), data.end (),
             reinterpret_cast<unsigned char*> (&(container[0])));
  
  return container;
}

template <class F, class S>
std::vector<unsigned char> 
write_map (const std::map<F,S>& container)
{
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

template <class F, class S>
std::map<F,S> 
read_map (const std::vector<unsigned char>& data)
{
  size_t size = (sizeof (F) + sizeof (S));
  size_t numel = data.size () * sizeof (unsigned char) / size;

  std::map<F,S> container;

  auto destf = reinterpret_cast<const F*> (&(data[0]));
  for (auto ii = 0; ii < numel; ++ii, ++destf)
    container[*destf];

  auto dests = reinterpret_cast<const S*> (destf);
  for (auto ii = 0; ii < numel; ++ii, ++dests)
    container[*destf] = *dests;
        
  return container;
}
