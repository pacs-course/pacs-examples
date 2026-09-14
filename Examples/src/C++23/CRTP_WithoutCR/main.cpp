// deducingThisCRTP.cpp

#include <iostream>
#include <array>

struct Base{                                            // (1)
  // C++23 deducing-this: the receiver type is deduced from the call site,
  // so this one method behaves like a generic member over the actual object.
  template <typename Self>
  void interface(this Self&& self){
    self.implementation();
  }

  // The same idea works for overloaded accessors: const/non-const forms
  // can share a single member definition while preserving the correct object.
  int& operator[](this auto& self, std::size_t index){
    return self.data[index];
  }
  int operator[](this auto const & self, std::size_t index) {
    return self.data[index];
  }

  void implementation(){
    std::cout << "Implementation Base" << '\n';
  }
  protected:
  std::array<int, 10> data{};
};

struct Derived1: Base{
  Derived1(){
    data.fill(1);
  }
  
  int operator[](std::size_t index)const {
    return data[index];
  }
  int& operator[](std::size_t index){
    return data[index];
  }
  void implementation(){
    std::cout << "Implementation Derived1" << '\n';
  }
};

struct Derived2: Base{
  Derived2(){
    data.fill(2);
  }
  int operator[](std::size_t index)const {
    return data[index];
  }
  int& operator[](std::size_t index){
    return data[index];
  }
  void implementation(){
    std::cout << "Implementation Derived2" << '\n';
  }
};

struct Derived3: Base{
  Derived3(){
    data.fill(3);
  }   
};

template <typename T>
void execute(T& base){
    // This is ordinary overloaded dispatch; using the actual object type
    // makes both const and non-const indexing work naturally.
    std::cout << base.operator[](0);
}

template <typename T>
void execute2(T& base){
    // Unlike CRTP's static_cast, deducing-this lets the base member recover
    // the receiver type without templating the whole class hierarchy.
    base.interface();
}

int main(){
  std::cout << '\n';
  Base b;                                             // (2)
  execute(b);                                         // base indexing, base implementation

  Derived1 d1;                                        // (3)
  execute(d1);                                        // derived indexing, derived implementation

  Derived2 d2;                                        // (4)
  execute(d2);                                        // derived indexing, derived implementation

  Derived3 d3;                                        // (5)
  execute(d3);                                        // base indexing, base implementation

  std::cout << '\n';

  // The key demo: a base member can call back into the concrete receiver
  // without virtual dispatch or CRTP-style casts.
  execute2(b);
  execute2(d1);
  execute2(d2);
  execute2(d3);
  std::cout << '\n';
}