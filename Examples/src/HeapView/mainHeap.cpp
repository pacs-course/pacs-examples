//
// Created by forma on 18/06/23.
//
#include "heapView.hpp"
#include <iostream>
#include <iomanip>
// A small function to print a heap stored in a vector
void printHeap(auto const & heap, std::size_t idx = 0, std::size_t indent = 0) {
    if (idx < heap.size()) {
        printHeap(heap, 2*idx + 2, indent + 4);
        std::cout << std::setw(indent) << ' ';
        std::cout << heap[idx] << "\n";
        printHeap(heap, 2*idx + 1, indent + 4);
    }
}
void printHeapView(auto const & heapView) {
  std::vector<double> heap;
  for(auto i = 0u; i < heapView.size(); ++i)
    {
      heap.push_back(heapView.heapValue(i));
    }
  printHeap(heap);
}

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
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    auto [index, value] = heapView.popPair();
    std::cout << "heapView pop = " << index << " " << value << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "\nadding a small element" << std::endl;
    heapView.add(-90.);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "removing element 2" << std::endl;
    heapView.remove(2);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "updating element 7 with a big value" << std::endl;
    heapView.update(7, 100.);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
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
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    auto [index, value] = heapView.popPair();
    std::cout << "heapView pop = " << index << " " << value << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "\nadding a small element" << std::endl;
    heapView.add(-90.);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "removing element 2" << std::endl;
    heapView.remove(2);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout << "updating element 7 with a big value" << std::endl;
    heapView.update(7, 100.);
    std::cout << "heapView top = " << heapView.top() << std::endl;
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout<< "Emptying heap\n";
    auto currentSize=heapView.size();
    std::vector<std::size_t> currentHeapElements;
    for (auto i=0ul;i<currentSize;++i)
    {
     auto where= heapView.popPair();
     currentHeapElements.emplace_back(where.first);
    }
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    std::cout<< "Putting elements back with update\n";
    for (auto i=0ul;i<currentSize;++i)
    {
      auto k = currentHeapElements[i];
      heapView.update(k,heapView[k]);
    }
    std::cout << "heapView size = " << heapView.size() << std::endl;
    std::cout << "current values in the heap = " << std::endl;
    printHeapView(heapView);
    std::cout << std::endl;
    std::cout << "Heap is sane? " << std::boolalpha << heapView.check()
              << std::endl;
    
  }
}
