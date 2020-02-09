#include<optional>
#include<vector>
#include<iostream>
#include<algorithm>
template<class C>
void countValid(C const & v)
{
  int count{0};
  // A possible use of for_each
  std::for_each(v.begin(),v.end(),[&count](typename C::value_type const & x){if (x) ++count;});
  std::cout<<"The container has "<< count<<" valid elements:\n";
  for (auto & x: v)
    if(x) // valid
      {
        // Dereferencing an optional returns underlying value
        std::cout<<*x<<" ";
      }
  std::cout<<std::endl;
}
int main()
{
  // A vector of 10 optionals
  // The optional are default contructed and thus are empty
  std::vector<std::optional<double>> v(10);
  
  v[0]=10.0;
  v[3]=30.0;
  countValid(v);
  // another way of thesting if a value is set
  // and to get the value: value() will throw an exception
  // if the value is not set, so it is safer than *
  if(v[2].has_value())
    std::cout<<"v[2] stores "<<v[2].value()<<std::endl;
  else
    std::cout<<"v[2] stores no value"<<std::endl;
  v[2]=20.0;
  countValid(v);  
  if(v[2].has_value())
    std::cout<<"v[2] stores "<<v[2].value()<<std::endl;
  else
    std::cout<<"v[2] stores no value"<<std::endl;
  // You can reset a value to "unset"
  v[2].reset();
  if(v[2].has_value())
    std::cout<<"v[2] stores "<<v[2].value()<<std::endl;
  else
    std::cout<<"v[2] stores no value"<<std::endl;  
}
