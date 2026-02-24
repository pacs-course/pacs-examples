cmake_minimum_required(VERSION 3.20)
project(Utilities LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsExample.cmake)

pacs_setup_defaults()
set(CMAKE_CXX_STANDARD 20)
set(PACS_USE_TBB ON)

pacs_add_example_dir(LIB_NAME pacs EXPLICIT_MAINS test_Factory.cpp test_JoinVectors.cpp test_absdiff.cpp test_chrono.cpp test_clonable.cpp test_cppversion.cpp test_csvReader.cpp test_hash_combine.cpp test_is_complex.cpp test_is_eigen.cpp test_is_specialization.cpp test_overloaded.cpp test_parallel_for.cpp test_range_to_vector.cpp test_scientific_precision.cpp test_string.cpp test_toString.cpp test_tuple_util.cpp test_type_name.cpp EXPLICIT_OTHERS readCSV.cpp string_utility.cpp)
