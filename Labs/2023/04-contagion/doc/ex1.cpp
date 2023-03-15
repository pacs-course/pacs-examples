// naive
for(size_t i = 0; i < v.size(); ++i)
  // at each iteration i we bring into the cache K different blocks of v
  // since we are accessing to the whole range
  for(size_t j = 0; j < v.size(); ++j)
    // do something with v[i] and v[j]

// partially cache optimized
for(size_t k = 0; k < K; ++k)
  for(size_t i = 0; i < v.size(); ++i)
    // at each iteration i we bring into the cache just one block of v
    // since we access only to the values from L1CS * k to L1CS * (k + 1)
    for(size_t j = L1CS * k; j < L1CS * (k + 1); ++j)
      // do something with v[i] and v[j]