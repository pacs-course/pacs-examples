import example

A = [1.,2.,3.,4.]

B = example.modify(A)

print(B)

# In C++ a std::map is sorted by ascending key.
map_int_double = example.create_map(
    # Python sets are sorted and will only contain unique elements.
    {111, 1, 11, 111},
    [0.1, 0.2, 0.3]
)
print(map_int_double)

# Python dictionaries are not sorted by ascending key.
map_string_int = example.create_map(
    {"Dave", "Bob", "Charlie"},
    [1989, 1974, 1991])
map_string_int["Alice"] = 1980
print(map_string_int)
