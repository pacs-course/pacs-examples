#include <iostream>
#include <mdspan>
#include <numeric>
#include <vector>

int
main()
{
#if __cplusplus < 202100L
#error "This example requires C++23 or later"
#else
  // Example 1: Basic 2D matrix using mdspan
  std::vector<double> data(
    12); // a vector with 12 elements to hold a 3x4 matrix
  std::iota(data.begin(), data.end(),
            1.0); // fill the vector with values from 1.0 to 12.0

  // Create a 3x4 matrix view
  // The mdspan is created with the data pointer and the extents for a 3x4
  // matrix
  // The following statement is equivalent to
  // std::mdspan<double, std::extents<std::size_t, 3, 4>> matrix(data.data());
  // I am exploiting the class template argument deduction to avoid specifying
  // the type and the extents explicitly The matrix is stored in row-major order
  // by default, but you can specify column-major order if needed:
  // std::mdspan<double, std::extents<std::size_t, 3, 4>, std::layout_left>
  // matrix(data.data());
  // std::layout_left is the tag for column-major order, while std::layout_right
  // is the tag for row-major order (the default).

  auto matrix = std::mdspan{data.data(), std::extents<std::size_t, 3, 4>{}};

  std::cout << "2D Matrix (3x4):\n";
  for(std::size_t i = 0; i < matrix.extent(0); ++i)
    {
      for(std::size_t j = 0; j < matrix.extent(1); ++j)
        {
          std::cout << matrix[i, j] << " ";
        }
      std::cout << "\n";
    }

  // Example 2: Dynamic extents
  std::size_t         rows = 4, cols = 5; // this could be read from a file
  std::vector<double> matrix_data(rows *
                                  cols); // a vector to holde the matrix data
  std::iota(matrix_data.begin(), matrix_data.end(),
            1.0); // fill the vector with values from 1.0 to 20.0
  // I use again the automatic tempalte deduction. The mdspan is created with
  // the data pointer and dynamic extents for a 4x5 matrix The following
  // statement is equivalent to std::mdspan<double, std::extents<std::size_t,
  // std::dynamic_extent, std::dynamic_extent>> dyn_matrix(matrix_data.data(),
  // rows, cols); or (short hand notation for a matrix with all the extents
  // dynamic)
  // std::mdspan<double, std::extents<std::size_t, std::dextents<std::size_t,2>
  // dyn_matrix(matrix_data.data(), rows, cols);
  auto dyn_matrix = std::mdspan{
    matrix_data.data(),
    // The mdspan is created with the data pointer and dynamic extents for a
    // 4x5 matrix
    std::extents<std::size_t, std::dynamic_extent, std::dynamic_extent>(rows,
                                                                        cols)};

  std::cout << "\nDynamic Matrix (" << dyn_matrix.extent(0) << "x"
            << dyn_matrix.extent(1) << "):\n";
  for(std::size_t i = 0; i < dyn_matrix.extent(0); ++i)
    {
      for(std::size_t j = 0; j < dyn_matrix.extent(1); ++j)
        {
          std::cout << dyn_matrix[i, j] << " ";
        }
      std::cout << "\n";
    }
  {
    // Example 3: only one dimension is dynamic
    constexpr std::size_t fixed_rows = 3;

    std::size_t dynamic_cols = 5; // maybe read from a file
    // I create the vector holding the data for the matrix with fixed rows and
    // dynamic columns
    std::vector<double> matrix_data2(fixed_rows * dynamic_cols);
    // Asssociate matrix_with_fixed_rows with data
    // an matrix with 3 rows and dynamic number of columns
    std::mdspan<double,
                std::extents<std::size_t, fixed_rows, std::dynamic_extent> >
      matrix_with_fixed_rows(matrix_data2.data(), fixed_rows, dynamic_cols);
    // You can fill the matrix with data as you like,
    // for example with a for loop
    for(std::size_t i = 0; i < matrix_with_fixed_rows.extent(0); ++i)
      {
        for(std::size_t j = 0; j < matrix_with_fixed_rows.extent(1); ++j)
          {
            matrix_with_fixed_rows[i, j] =
              static_cast<double>(i * dynamic_cols + j + 1);
          }
      }
    std::cout << "\nMatrix with fixed rows and dynamic columns ("
              << matrix_with_fixed_rows.extent(0) << "x"
              << matrix_with_fixed_rows.extent(1) << "):\n";
    for(std::size_t i = 0; i < matrix_with_fixed_rows.extent(0); ++i)
      {
        for(std::size_t j = 0; j < matrix_with_fixed_rows.extent(1); ++j)
          {
            std::cout << matrix_with_fixed_rows[i, j] << " ";
          }
        std::cout << "\n";
      }
  }
  {
    // Example 5: 3D The same using column major order
    constexpr std::size_t fixed_cols = 5;

    std::size_t dynamic_rows = 3; // maybe read from a file
    // I create the vector holding the data for the matrix with fixed columns
    // and dynamic rows
    std::vector<double> matrix_data2(fixed_cols * dynamic_rows);
    // A column major matrix with 3 columns and dynamic number of rows
    std::mdspan<double,
                std::extents<std::size_t, std::dynamic_extent, fixed_cols>,
                std::layout_left>
      matrix_with_fixed_cols(matrix_data2.data(), dynamic_rows, fixed_cols);
    // You can fill the matrix with data as you like,
    // for example with a for loop
    for(std::size_t i = 0; i < matrix_with_fixed_cols.extent(0); ++i)
      {
        for(std::size_t j = 0; j < matrix_with_fixed_cols.extent(1); ++j)
          {
            matrix_with_fixed_cols[i, j] =
              static_cast<double>(i * fixed_cols + j + 1);
          }
      }
    std::cout << "\nMatrix with fixed columns and dynamic rows ("
              << matrix_with_fixed_cols.extent(0) << "x"
              << matrix_with_fixed_cols.extent(1) << "):\n";
    for(std::size_t i = 0; i < matrix_with_fixed_cols.extent(0); ++i)
      {
        for(std::size_t j = 0; j < matrix_with_fixed_cols.extent(1); ++j)
          {
            std::cout << matrix_with_fixed_cols[i, j] << " ";
          }
        std::cout << "\n";
      }
  }
  // Example 4: Vector operations with mdspan
  std::vector<double> vec_data(5); // a vector to hold 5 elements
  std::iota(vec_data.begin(), vec_data.end(), 1.0);

  // Create a 1D vector view using mdspan
  auto vector = std::mdspan(vec_data.data(), std::extents<std::size_t, 5>());

  std::cout << "\nVector operations:\n";
  std::cout << "Vector: ";
  for(std::size_t i = 0; i < vector.extent(0); ++i)
    {
      std::cout << vector[i] << " ";
    }

  // Dot product simulation
  double dot_product = 0.0;
  for(std::size_t i = 0; i < vector.extent(0); ++i)
    {
      dot_product += vector[i] * vector[i];
    }
  std::cout << "\nDot product (v·v): " << dot_product << "\n";

  // you can also have a 3D tensor view, or even higher dimensions, by simply
  // changing the extents and the underlying data layout.

  return 0;
#endif
}