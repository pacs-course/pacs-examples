#A class for matrices.# 

It stores the elements as `unique_ptr` and access them via the call () operator rowwise or columnwise according to a policy that can be determined run time. The policy is implemented as a pointer to member function.

Since I want a deep copy, I had to define copy and move operators explicitly.

Alternatively, I could have used the `Wrapper` class in the file `CloningUtilities.hpp` in the `Utility` directory, which implements a deep copy implementation for a single owning pointer, provided the pointed object is clonable (i.e. implements virtual construction).

