#include <iostream>
#include <vector>
#include "heapView.hpp"
int main() {
    std::vector<int> data = {9, 5, 7, 3, 2, 6};
    apsc::HeapView<int> heapView(data);

    std::cout << "The top element is: " << heapView.top() << std::endl;
    // addinga na element
    heapView.add(-3);
    std::cout<<" after adding a new value (-3) ";
    std::cout << "The top element is: " << heapView.top() << std::endl;
    heapView.pop();
    std::cout<<" after popPing the top element ";
    std::cout << "The top element is: " << heapView.top() << std::endl;
    heapView.update(1, -10);
    std::cout<<" after changing a value (-10) ";
    std::cout << "The top element is: " << heapView.top() << std::endl;
 
    auto const & data2 = heapView.data();
    std::cout << "The origine data is: \n";
    for (auto const & d : data2)
        std::cout << d << " ";
    std::cout << std::endl;
    std::cout << "The heap is: \n";
    while (heapView.empty() == false) {
        std::cout << heapView.popPair().second << " ";
    }
    std::cout << std::endl;
    return 0;
}
