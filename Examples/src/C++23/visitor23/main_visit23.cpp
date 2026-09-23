/* @file main_visit23.cpp */
/**
 * Visitor using std::variant and an overload set of lambdas. C++23 deducing
 * this lets the car handler reuse the complete visitor for its children.
 * Compare main_visit_classic.cpp for virtual double dispatch.
 *
 * From https://www.modernescpp.com/index.php/c23-deducing-this/
 */
#include <iostream>
#include <string>
#include <vector>
#include <variant>

// Some visitor handlers only print a message and do not use their arguments.
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

// Each lambda has a distinct closure class with an operator(). Inherit from
// those classes and expose all their call operators as one overload set.
// Class template argument deduction infers Ts from overloaded{lambda, ...}.
template<class... Ts> struct overloaded : Ts... {
    using Ts::operator()...; 
};

// Plain data types: no common base, virtual functions, or accept members.
class Wheel {
 public:
    Wheel(const std::string& n): name(n) { }
    std::string getName() const {
        return name;
    }
 private:
    std::string name;
};

// Distinct empty types still select distinct visitor overloads.
class Body {};

class Engine {};

// Declare Car before naming it as an alternative. The alias alone does not
// create a variant object; Car is defined before variants are created in main.
class Car;

// A closed list of possible element types. Each variant holds one alternative.
// Adding a type here also requires handlers in every visitor used below.
using CarElement = std::variant<Wheel, Body, Engine, Car>;

// Composite: storing pointers avoids embedding children recursively by value.
class Car {
 public:
    Car(std::initializer_list<CarElement*> carElements ):
      elements{carElements} {}
   
   // Accept either visitor type without a virtual interface. T&& is a
   // forwarding reference; the named visitor is used as an lvalue repeatedly.
   template<typename T>
   void visitCarElements(T&& visitor) const {
       for (auto elem : elements) {
           // Runtime selection of the active alternative, with compile-time
           // checking that visitor can handle every alternative in CarElement.
           std::visit(visitor, *elem);
       }
   }
 private:
    // Non-owning: main keeps the pointed-to variants alive during traversal.
    // Nested cars are supported, but cycles would cause infinite recursion.
    std::vector<CarElement*> elements;
};

// Operation 1. Const-reference parameters avoid copying the visited elements.
overloaded carElementPrintVisitor {
    [](const Body& body)     {  std::cout << "Visiting body" << '\n'; },      
    // The explicit object parameter is supplied automatically when invoked.
    // Since operator() is inherited by overloaded, self deduces as a const
    // reference to the COMPLETE visitor, including all four lambda handlers.
    // Passing only the Car lambda would fail std::visit's checks for the other
    // alternatives. self lets traversal reuse this same visitor recursively.
    // This is necessary for this lambda design, not for Visitor in general:
    // a named function object could instead pass *this from operator().
    [](this auto const& self, const Car& car)  {  car.visitCarElements(self);
                                                  // Parent follows its children.
                                                  std::cout << "Visiting car" << '\n'; },
    [](const Wheel& wheel)   {  std::cout << "Visiting " 
                                          << wheel.getName() << " wheel" << '\n'; },
    [](const Engine& engine) {  std::cout << "Visiting engine" << '\n';}
};

// Operation 2. The element types stay unchanged; only the handlers differ.
overloaded carElementDoVisitor {
    [](const Body& body)     {  std::cout << "Moving my body" << '\n'; },
    // Here self refers to the action visitor, so children get action handlers.
    // The visitor controls traversal, unlike Car::accept in the classic code.
    [](this auto const& self, const Car& car) {  car.visitCarElements(self);
                                                std::cout << "Starting my car" << '\n'; },
    [](const Wheel& wheel)   {  std::cout << "Kicking my " 
                                          << wheel.getName()  << " wheel" << '\n'; },
    [](const Engine& engine) {  std::cout << "Starting my engine" << '\n';}
};
 

int main() {

    std::cout << '\n';

    // Each local variant owns its active value; all share the CarElement type.
    CarElement wheelFrontLeft  = Wheel("front left");
    CarElement wheelFrontRight = Wheel("front right");
    CarElement wheelBackLeft   = Wheel("back left");
    CarElement wheelBackRight  = Wheel("back right");
    CarElement body            = Body{};
    CarElement engine          = Engine{};
 
    // The Car alternative stores pointers to the locals, which outlive visiting.
    CarElement car  = Car{&wheelFrontLeft, &wheelFrontRight,
             &wheelBackLeft, &wheelBackRight,
             &body, &engine};

    // Visit one leaf, then the car and its children. The engine is visited twice.
    // std::visit supplies the held element; it does not pass self explicitly.
    std::visit(carElementPrintVisitor, engine);
    std::visit(carElementPrintVisitor, car);
    std::cout << '\n';

    // Same structure, different operation; output matches the classic example.
    std::visit(carElementDoVisitor, engine);
    std::visit(carElementDoVisitor, car);
    std::cout << '\n';
    
}
