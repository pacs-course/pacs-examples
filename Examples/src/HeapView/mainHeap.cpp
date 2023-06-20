//
// Created by forma on 18/06/23.
//
#include "heapView.hpp"
#include <iostream>

int
main()
{
  using namespace apsc;
  std::vector<double> data {1.0, 2.0, 3.0, 4.0, 5.0, 0.,
                           -1., -2., -3., -4., -5.};

  {
    // min heap
    HeapView<double> heapView(data);

    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    auto [index, value] = heapView.pop();
    std::cout << "heapView pop = " << index << " " << value << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "\nadding a small element" << std::endl;
    heapView.add(-90.);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "removing element 2" << std::endl;
    heapView.remove(2);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "updating element 7 with a big value" << std::endl;
    heapView.update(7, 100.);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
  }
  std::cout << "\n*****************************************\n";
  {
    // max heap
    HeapView<double, std::greater<>> heapView(data);

    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    auto [index, value] = heapView.pop();
    std::cout << "heapView pop = " << index << " " << value << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "\nadding a small element" << std::endl;
    heapView.add(-90.);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "removing element 2" << std::endl;
    heapView.remove(2);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "updating element 7 with a big value" << std::endl;
    heapView.update(7, 100.);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    for(auto i = 0u; i < heapView.size(); ++i)
      {
        std::cout << heapView.heapValue(i) << " ";
      }
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
  }
}
