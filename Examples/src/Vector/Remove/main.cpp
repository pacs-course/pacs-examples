#include <vector>
#include <iostream>
#include <algorithm>
template<class V >
void printVector(const V & v)
{
  std::cout<<"Vector size = "<<v.size()<<std::endl;
  std::cout<<"Vector capacity = "<<v.capacity()<<std::endl;
  std::cout<<"Vector entries:\n";
  for (auto x:v) std::cout<<x<<", ";
  std::cout<<std::endl;
}
int main()
{
  {
    std::cout<<"VERSION WITH ONLY REMOVE**\n";
    std::vector<int> v{1,2,3,4,5,6,7,8,9,1,2,3};
    printVector(v);
    std::remove(v.begin(),v.end(),9);
    std::cout<<"After *removing* 9\n";
    printVector(v);
    // Passing a predicate 
    std::remove_if(v.begin(),v.end(),[](int i){return i>7;}); 
    std::cout<<"After *removing* all >7\n";
    printVector(v);
  }
  {
    std::cout<<"VERSION WITH REMOVE AND ERASE**\n";
    std::vector<int> v{1,2,3,4,5,6,7,8,9,1,2,3};
    printVector(v);
    auto it= std::remove(v.begin(),v.end(),9);
    v.erase(it, v.end());
    std::cout<<"After erasing 9\n";
    printVector(v);
    // Passing a predicate 
    it= std::remove_if(v.begin(),v.end(),[](int i){return i>7;}); 
    v.erase(it, v.end());
    std::cout<<"After erasing all >7\n";
    printVector(v);
  }
  {
    std::cout<<"VERSION WITH REMOVE +ERASE+SHRINK**\n";
    std::vector<int> v{1,2,3,4,5,6,7,8,9,1,2,3};
    printVector(v);
    auto it= std::remove(v.begin(),v.end(),9);
    v.erase(it, v.end());
    v.shrink_to_fit();
    std::cout<<"After erasing 9 and shrinking\n";
    printVector(v);
    // Passing a predicate 
    it= std::remove_if(v.begin(),v.end(),[](int i){return i>7;}); 
    v.erase(it, v.end());
    v.shrink_to_fit();
    std::cout<<"After erasing all >7 and shrinking\n";
    printVector(v);
  }

}
