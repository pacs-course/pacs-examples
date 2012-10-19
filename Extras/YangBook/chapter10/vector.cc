#include <vector>
#include <iostream>
#include <algorithm>

int main() {

 std::vector<int> vi(10);
 for (int i = 0; i < 10; i++) vi[i] = (i-5)*i;
 std::sort(vi.begin(),vi.end());

 for (int i = 0; i < 10; i++) {
   std::cout << vi[i] << "    ";
 } 
 std::cout << std::endl;

 std::unique(vi.begin(),vi.end());
 for (int i = 0; i < 10; i++) {
  std::cout << vi[i] << "    ";
 } 
 std::cout << std::endl;

}
