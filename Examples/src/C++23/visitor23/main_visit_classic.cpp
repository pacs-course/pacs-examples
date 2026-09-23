/*!
@file main_visit_classic.cpp

Classic Visitor: separate operations from the objects on which they act.
Two class hierarchies cooperate through virtual functions: car elements and
visitors. Compare main_visit23.cpp for a variant-and-lambda implementation.

From https://www.modernescpp.com/index.php/c23-deducing-this/

*/

#include <iostream>
#include <string>
#include <vector>

// Some visitor handlers only print a message and do not use their arguments.
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

// Forward declarations break the circular dependency: elements accept visitors,
// and visitors declare operations taking concrete elements.
class CarElementVisitor;

// Common interface lets a car store different element types through pointers.
class CarElement {
 public:
    // Pure virtual: every concrete element must supply its own entry point.
    virtual void accept(CarElementVisitor& visitor) const = 0;
    // Allows safe destruction through a base pointer, although main uses locals.
    virtual ~CarElement() = default;
};

class Body;
class Car;
class Engine;
class Wheel;

// One overload per element type. A new operation implements this interface;
// a new element type requires extending it and updating concrete visitors.
class CarElementVisitor {
 public:
    // These parameters are passed by value, so visiting copies the element.
    // A production interface could use const references to avoid these copies.
    virtual void visit(Body body) const = 0;
    virtual void visit(Car car) const = 0;
    virtual void visit(Engine engine) const = 0;
    virtual void visit(Wheel wheel) const = 0;
    virtual ~CarElementVisitor() = default;
};

// A leaf element: it has data but delegates visitor-specific operations.
class Wheel: public CarElement {
 public:
    Wheel(const std::string& n): name(n) { }

    void accept(CarElementVisitor& visitor) const override {
        // Double dispatch:
        // 1. A virtual accept call selects Wheel::accept from the element type.
        // 2. Here *this is const Wheel, selecting visit(Wheel) at compile time.
        // 3. That virtual visit call selects the concrete visitor's operation.
        visitor.visit(*this);
    }

    std::string getName() const {
        return name;
    }
 private:
    std::string name;
};

// Even stateless leaves need accept overrides: their concrete static type is
// what selects the matching visit overload. A base-only accept would lose it.
class Body: public CarElement {
 public:
    void accept(CarElementVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

// Same dispatch mechanism as Wheel and Body, selecting visit(Engine).
class Engine: public CarElement {
 public:
    void accept(CarElementVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

// Composite element: a car contains elements and can itself be visited.
class Car: public CarElement {
 public:
    Car(std::initializer_list<CarElement*> carElements ):
      elements{carElements} {}
   
    void accept(CarElementVisitor& visitor) const override {
        // Traversal belongs to the element in this version. Visit children in
        // insertion order, using their virtual accept functions.
        for (auto elem : elements) {
            elem->accept(visitor);
        }
        // Visit the parent after its children (post-order traversal).
        visitor.visit(*this);
    }
 private:
    // Non-owning pointers: pointees must remain alive during traversal.
    // Nested cars are possible; cycles would recurse indefinitely.
    std::vector<CarElement*> elements;
};

// Operation 1: describe actions without adding action methods to each element.
// Overrides are private by default in a class. Calls remain valid through the
// public base interface: access is checked there, then virtual dispatch occurs.
class CarElementDoVisitor: public CarElementVisitor {
   
    void visit(Body body) const override {
        std::cout << "Moving my body" << '\n';
    }

     void visit(Car car) const override {
        std::cout << "Starting my car" << '\n';
    }

    void visit(Wheel wheel) const override {
        std::cout << "Kicking my " << wheel.getName() 
          << " wheel" << '\n';
    }

    void visit(Engine engine) const override {
        std::cout << "Starting my engine" << '\n';
    }
};

// Operation 2: inspect the same structure using a different visitor.
class CarElementPrintVisitor: public CarElementVisitor {
   
    void visit(Body body) const override {
        std::cout << "Visiting body" << '\n';
    }

     void visit(Car car) const override {
        std::cout << "Visiting car" << '\n';
    }

    void visit(Wheel wheel) const override {
        std::cout << "Visiting " << wheel.getName() 
          << " wheel" << '\n';
    }

    void visit(Engine engine) const override {
        std::cout << "Visiting engine" << '\n';
    }
};

int main() {

    std::cout << '\n';

    // main owns the elements. They outlive the car and all visitor calls.
    Wheel wheelFrontLeft("front left");
    Wheel wheelFrontRight("front right");
    Wheel wheelBackLeft("back left");
    Wheel wheelBackRight("back right");
    Body body;
    Engine engine;
    Car car {&wheelFrontLeft, &wheelFrontRight, 
             &wheelBackLeft, &wheelBackRight,
             &body, &engine};

    CarElementPrintVisitor carElementPrintVisitor; 

    // Visit one leaf, then the whole structure. The engine prints twice because
    // it is also a child of the car; the car message follows all child messages.
    engine.accept(carElementPrintVisitor);
    car.accept(carElementPrintVisitor);

    std::cout << '\n';

    CarElementDoVisitor carElementDoVisitor;

    // Reuse the same objects with a different operation.
    engine.accept(carElementDoVisitor);
    car.accept(carElementDoVisitor);

    std::cout << '\n';

}
