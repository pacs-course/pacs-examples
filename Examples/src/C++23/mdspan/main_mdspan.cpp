// mdspan.cpp

#include <mdspan>   // Provides std::mdspan for multidimensional views.
#include <iostream> // Provides console output.
#include <vector>   // Provides the contiguous storage for the elements.
#include <tuple>    // Provides tuple-related standard library facilities.

int main() {

    // Store eight integers in contiguous memory.
    std::vector myVec{1, 2, 3, 4, 5, 6, 7, 8};

    // Create a 2-by-4 view over the vector; mdspan does not own the data.
    // the dafault layout policy is std::layout-right, which means that the matrix
    // is stored in row-major order. i j -> i * 4 + j
    std::mdspan m{myVec.data(), 2, 4};
    std::cout << "m.rank(): " << m.rank() << '\n';

    // Visit each element using its row and column indices.
    for (std::size_t i = 0; i < m.extent(0); ++i) {
        for (std::size_t j = 0; j < m.extent(1); ++j) {
            std::cout << m[i, j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << '\n';

    // Create another view of the same data, this time with four rows and two columns.
    // end left layout policy is std::layout-left, which means that the matrix
    // is stored in column-major order. i j -> j * 4 + i
    // Here I need to specify the extents of the mdspan explicitly, in the template argument
    std::mdspan<int, std::extents<std::size_t, 4, 2>, std::layout_left> m2{myVec.data()};
    std::cout << "m2.rank(): " << m2.rank() << '\n';

    // Print the second view to show how the same storage can have a different shape.
    for (std::size_t i = 0; i < m2.extent(0); ++i) {
        for (std::size_t j = 0; j < m2.extent(1); ++j) {
            std::cout << m2[i, j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
//If you want static extent, that is you know the size of the array at compile time, 
//you can also use std::extents to specify the extents of the mdspan. 
//The advantage is gained efficiency, but you have to know the size of the array at compile time.
// We had static extent also above, thanks to the constructor of mdspan, 
//but here we are using std::extents explicitely to specify the extents of the mdspan.
    std::mdspan<int, std::extents<std::size_t, 2, 4>> m3{myVec.data()};
    std::cout << "m3.rank(): " << m3.rank() << '\n';

    // Visit each element using its row and column indices.
    for (std::size_t i = 0; i < m3.extent(0); ++i) {
        for (std::size_t j = 0; j < m3.extent(1); ++j) {
            std::cout << m3[i, j] << ' ';
        }
        std::cout << '\n';
    }
    // SOmetime you do know the extent of the array at run time. In this case you have to use dynamic extent. 
    std::size_t rows = 2;// this are nomelly read from afile or user input.
    std::size_t cols = 4;
    std::mdspan<int, std::extents<std::size_t, std::dynamic_extent, std::dynamic_extent>> m4{myVec.data(), rows, cols};
    std::cout << "m4.rank(): " << m4.rank() << '\n';
    for (std::size_t i = 0; i < m4.extent(0); ++i) {
        for (std::size_t j = 0; j < m4.extent(1); ++j) {
            std::cout << m4[i, j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';    
// you can also have a 3 dimensional array, for example a 2-by-3-by-4 array. 
    std::vector myVec3D{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
// Here we show that you can mix static and dynamic extents. Some extents can be static while others are dynamic.
// Here the first two extents are static and the third is dynamic.
    std::size_t third=4;
    std::mdspan<int, std::extents<std::size_t, 2, 3, std::dynamic_extent>>  m5{myVec3D.data(),2,3,third};
    std::cout << "m5.rank(): " << m5.rank() << '\n';
    for (std::size_t i = 0; i < m5.extent(0); ++i) {
        for (std::size_t j = 0; j < m5.extent(1); ++j) {
            for (std::size_t k = 0; k < m5.extent(2); ++k) {
                std::cout << m5[i,j,k] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}
