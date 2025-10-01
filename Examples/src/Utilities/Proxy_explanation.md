# Proxy.hpp - Automatic Factory Registration Proxy

This header file implements a **Proxy pattern** that provides automatic registration of concrete products into a generic factory system. The proxy acts as an intermediary that simplifies the process of registering new types with a factory, eliminating the need for manual registration code and providing a clean, declarative approach to factory population.

## Design Pattern Overview

The Proxy pattern typically provides a **surrogate or placeholder** for another object to control access to it. In this specific implementation, the proxy serves as:

- **Registration automator**: Automatically registers concrete types with the factory
- **Builder provider**: Supplies standardized object creation logic
- **Initialization manager**: Handles factory registration at static initialization time
- **Type safety enforcer**: Ensures compatibility between concrete types and factory expectations

## Core Problem Solved

### Manual Registration Pain Points

Without the proxy, factory registration requires manual setup:

```cpp
// Manual approach - error-prone and scattered
auto& factory = ShapeFactory::Instance();
factory.add("circle", []() { return std::make_unique<Circle>(); });
factory.add("rectangle", []() { return std::make_unique<Rectangle>(); });
factory.add("triangle", []() { return std::make_unique<Triangle>(); });
```

**Problems with manual registration**:
- **Scattered code**: Registration logic spread across multiple files
- **Easy to forget**: New types might not get registered
- **Boilerplate**: Repetitive builder lambda creation
- **Maintenance burden**: Changes require updating multiple locations

### Proxy Solution Benefits

The proxy enables **declarative registration**:

```cpp
// Proxy approach - clean and automatic
namespace {
    Proxy<ShapeFactory, Circle> registerCircle("circle");
    Proxy<ShapeFactory, Rectangle> registerRectangle("rectangle");
    Proxy<ShapeFactory, Triangle> registerTriangle("triangle");
}
```

**Advantages**:
- **Self-registering types**: Objects register themselves automatically
- **Centralized per-type**: Registration code lives with the type definition
- **Compile-time safety**: Template system ensures type compatibility
- **Zero runtime overhead**: Registration happens during static initialization

## Template Architecture

```cpp
template <typename Factory, typename ConcreteProduct> 
class Proxy
```

### Template Parameter Requirements

#### 1. Factory Parameter
- **Must be**: A specialization of `GenericFactory::Factory<>`
- **Provides**: Type information about the factory system
- **Example**: `Factory<Shape, std::string, std::function<std::unique_ptr<Shape>()>>`

#### 2. ConcreteProduct Parameter  
- **Must be**: A concrete class that can be instantiated
- **Must inherit**: From the factory's `AbstractProduct_type`
- **Must have**: Default constructor (for the built-in builder)
- **Example**: `Circle` (inherits from `Shape`)

### Type Extraction System

```cpp
using AbstractProduct_type = typename Factory::AbstractProduct_type;
using Identifier_type = typename Factory::Identifier_type;
using Builder_type = typename Factory::Builder_type;
using Result_type = std::invoke_result_t<Builder_type>;
using Factory_type = Factory;
```

**Purpose of type aliases**:
- **AbstractProduct_type**: Base class that all products inherit from
- **Identifier_type**: Key type used to identify products in factory
- **Builder_type**: Function signature for object construction
- **Result_type**: Return type of builder functions (typically `std::unique_ptr<AbstractProduct>`)
- **Factory_type**: The complete factory type for convenience

### Advanced Type Deduction

The `std::invoke_result_t<Builder_type>` usage demonstrates **modern C++ metaprogramming**:
- **C++17 feature**: Replaces older `std::result_of`
- **Works with any callable**: Functions, lambdas, functors
- **Compile-time deduction**: No runtime overhead
- **Type safety**: Ensures builder return type compatibility

## Registration Mechanisms

### 1. Automatic Builder Registration

```cpp
Proxy(Identifier_type const &name);
```

**Implementation**:
```cpp
Proxy(Identifier_type const &name) {
    Factory_type &factory(Factory_type::Instance());
    factory.add(name, &Proxy<F, C>::Build);
    std::clog << "Added " << name << " to factory" << std::endl;
}
```

**Process Flow**:
1. **Get factory instance**: Uses singleton pattern to access factory
2. **Register builder**: Adds the static `Build` method as the constructor
3. **Log registration**: Provides feedback about successful registration

**Key Features**:
- **Uses built-in builder**: Leverages the proxy's static `Build()` method
- **Type safety**: Compiler ensures `ConcreteProduct` is compatible
- **Exception safety**: Factory handles duplicate registration errors

### 2. Custom Builder Registration

```cpp
Proxy(Identifier_type const &name, Builder_type const &b);
```

**Purpose**: Allows custom builder functions when the default isn't suitable

**Use cases**:
- **Custom constructors**: Products requiring specific constructor arguments
- **Initialization logic**: Objects needing special setup
- **Compatibility**: Working with existing builder functions
- **Factory method pattern**: Using static factory methods instead of constructors

**Example**:
```cpp
// Custom builder for complex initialization
auto customBuilder = []() {
    auto product = std::make_unique<ComplexProduct>();
    product->initialize();
    product->configure();
    return product;
};

Proxy<MyFactory, ComplexProduct> registerComplex("complex", customBuilder);
```

## Built-in Builder Implementation

### Standard Builder Method

```cpp
static Result_type Build() {
    return Result_type(new ConcreteProduct());
}
```

**Design Decisions**:

#### Why `new` instead of `make_unique`?
- **Generic compatibility**: Works with any smart pointer type, not just `std::unique_ptr`
- **Type flexibility**: `Result_type` might be custom smart pointer
- **Historical reasons**: Code may predate widespread `make_unique` adoption

#### Alternative Implementation (Commented)
```cpp
// static std::unique_ptr<AbstractProduct_type> Build(){
//     return std::make_unique<ConcreteProduct>();
// }
```

**Why commented out**:
- **Less generic**: Assumes `std::unique_ptr` specifically
- **Type constraints**: Limits flexibility of `Result_type`
- **Compatibility**: Current version works with more factory configurations

### Builder Requirements and Assumptions

The documentation specifies several **implicit contracts**:

1. **Builder signature**: Must match factory's `Builder_type`
2. **Return type compatibility**: `Result_type` must accept `ConcreteProduct*`
3. **Memory management**: `Result_type` must handle object lifetime
4. **Constructor availability**: `ConcreteProduct` must have accessible default constructor

## Singleton Interaction Pattern

### Factory Access Strategy

```cpp
Factory_type &factory(Factory_type::Instance());
```

**Design benefits**:
- **Lazy initialization**: Factory created only when first needed
- **Thread safety**: Meyer's singleton provides thread-safe creation
- **Global access**: Single factory instance shared across all proxies
- **Lifetime management**: Factory automatically cleaned up at program end

### Registration Timing

Proxy registration occurs during **static initialization**:

```cpp
namespace {
    // These execute during static initialization
    Proxy<Factory, Product1> reg1("product1");
    Proxy<Factory, Product2> reg2("product2");
}
```

**Initialization order considerations**:
- **Factory creation**: Happens on first proxy construction
- **Cross-translation unit**: Order between different `.cpp` files is undefined
- **Within translation unit**: Order follows declaration order
- **Best practice**: Keep proxies in anonymous namespaces near type definitions

## Copy Prevention

```cpp
private:
    Proxy(Proxy const &) = delete;            
    Proxy &operator=(Proxy const &) = delete; 
```

**Rationale for non-copyable design**:
- **Unique registration**: Each proxy represents one-time registration
- **Prevents confusion**: Copying would not re-register the type
- **Resource management**: Avoids issues with multiple registration attempts
- **Clear semantics**: Proxy is a configuration object, not a value type

## Real-World Usage Patterns

### 1. Self-Registering Classes

```cpp
// In Circle.hpp
class Circle : public Shape {
public:
    Circle(double radius) : radius_(radius) {}
    void draw() override { /* implementation */ }
private:
    double radius_;
    
    // Self-registration at file scope
    static inline Proxy<ShapeFactory, Circle> registrar_{"circle"};
};
```

**Benefits**:
- **Colocated**: Registration code lives with class definition
- **Automatic**: No separate registration step needed
- **Maintainable**: Adding new shapes is self-contained

### 2. Plugin Architecture

```cpp
// In plugin.cpp
class AudioPlugin : public Plugin {
public:
    void execute() override { /* audio processing */ }
};

namespace {
    // Anonymous namespace prevents external linkage
    Proxy<PluginFactory, AudioPlugin> registerAudio("audio");
}
```

### 3. Command Pattern Implementation

```cpp
class SaveCommand : public Command {
public:
    void execute() override { /* save logic */ }
};

// Custom builder for commands with parameters
auto saveBuilder = []() {
    auto cmd = std::make_unique<SaveCommand>();
    cmd->setDefaultPath("/tmp");
    return cmd;
};

namespace {
    Proxy<CommandFactory, SaveCommand> registerSave("save", saveBuilder);
}
```

### 4. Serialization Systems

```cpp
class SerializableData : public Serializable {
public:
    static std::unique_ptr<Serializable> deserialize(const std::string& data) {
        auto obj = std::make_unique<SerializableData>();
        obj->fromString(data);
        return obj;
    }
};

namespace {
    Proxy<SerializationFactory, SerializableData> reg("data", 
        SerializableData::deserialize);
}
```

## Error Handling and Diagnostics

### Registration Logging

```cpp
std::clog << "Added " << name << " to factory" << std::endl;
```

**Purpose of logging**:
- **Debugging aid**: Shows which types are being registered
- **Initialization tracking**: Confirms static initialization is working
- **Dependency debugging**: Helps trace factory population order

**Why `std::clog`?**
- **Separate from output**: Won't interfere with main program output
- **Buffered**: More efficient than `std::cerr`
- **Standard practice**: Conventional stream for logging messages

### Exception Propagation

The proxy **doesn't catch exceptions** from `factory.add()`:
- **Fail-fast behavior**: Registration errors immediately terminate program
- **Clear error reporting**: Factory exceptions contain detailed error information
- **Development feedback**: Duplicate registrations caught during development

## Advanced Usage Scenarios

### 1. Conditional Registration

```cpp
#ifdef ENABLE_ADVANCED_SHAPES
namespace {
    Proxy<ShapeFactory, AdvancedCircle> regAdvanced("advanced_circle");
}
#endif
```

### 2. Dynamic Configuration-Based Registration

```cpp
// Registration based on configuration
class ConfigurableProxy {
public:
    ConfigurableProxy() {
        if (Config::isFeatureEnabled("circles")) {
            static Proxy<ShapeFactory, Circle> circleReg("circle");
        }
    }
};
```

### 3. Version-Specific Registration

```cpp
namespace {
#if API_VERSION >= 2
    Proxy<ShapeFactory, Circle> regCircle("circle");
#else
    Proxy<ShapeFactory, LegacyCircle> regCircle("circle");
#endif
}
```

## Performance Characteristics

### Registration Overhead
- **Compile-time**: Template instantiation cost during compilation
- **Static initialization**: One-time cost during program startup
- **Runtime**: Zero overhead after initialization completes

### Memory Footprint
- **Per proxy instance**: Minimal (typically empty after construction)
- **Factory storage**: One entry per registered type
- **Static data**: Proxy objects persist for program lifetime

### Scalability Considerations
- **Large type count**: Linear scaling with number of registered types
- **Factory lookup**: O(1) average with hash-based factory implementation
- **Registration time**: O(1) per type during static initialization

## Design Trade-offs

### Advantages
1. **Automatic registration**: No manual factory setup required
2. **Type safety**: Compile-time verification of compatibility
3. **Maintainability**: Registration code stays with type definitions
4. **Extensibility**: Easy to add new types without modifying existing code
5. **Performance**: Zero runtime overhead after initialization

### Disadvantages
1. **Static initialization**: Can complicate program startup
2. **Hidden dependencies**: Registration happens implicitly
3. **Debugging complexity**: Static initialization order issues can be hard to trace
4. **Memory usage**: All proxies remain in memory for program lifetime
5. **Compile-time coupling**: Changes to factory interface affect all proxies

## Integration with Other Patterns

### Factory + Proxy + Registry
```cpp
// Complete system integration
class ShapeRegistry {
public:
    static ShapeFactory& getFactory() {
        return ShapeFactory::Instance();
    }
    
    static std::vector<std::string> availableShapes() {
        return getFactory().registered();
    }
};

// Usage
auto shapes = ShapeRegistry::availableShapes();
for (const auto& shapeName : shapes) {
    auto shape = ShapeRegistry::getFactory().create(shapeName);
    shape->draw();
}
```

### Plugin System Integration
```cpp
class PluginManager {
public:
    void loadPlugins() {
        // Proxies have already registered during static initialization
        auto& factory = PluginFactory::Instance();
        auto available = factory.registered();
        
        std::clog << "Available plugins: ";
        for (const auto& name : available) {
            std::clog << name << " ";
        }
        std::clog << std::endl;
    }
};
```

## Thread Safety Considerations

### Static Initialization
- **Thread-safe**: Static initialization is guaranteed thread-safe in C++11+
- **Single execution**: Each static proxy constructed exactly once
- **Factory access**: Meyer's singleton provides thread-safe factory access

### Runtime Safety
- **Read-only after init**: Proxies don't modify state after construction
- **Factory thread safety**: Depends on factory implementation
- **Registration immutable**: Once registered, entries don't change

## Best Practices

### 1. Placement Strategy
```cpp
// Good: In anonymous namespace near class definition
namespace {
    Proxy<MyFactory, MyClass> registrar("myclass");
}
```

### 2. Naming Conventions
```cpp
// Clear naming for debugging
namespace {
    Proxy<ShapeFactory, Circle> circleRegistrar("circle");
    Proxy<ShapeFactory, Rectangle> rectangleRegistrar("rectangle");
}
```

### 3. Error Prevention
```cpp
// Use string constants to avoid typos
namespace ShapeNames {
    constexpr const char* CIRCLE = "circle";
    constexpr const char* RECTANGLE = "rectangle";
}

namespace {
    Proxy<ShapeFactory, Circle> reg(ShapeNames::CIRCLE);
}
```

### 4. Documentation
```cpp
/*!
 * @brief Automatic registration for Circle shape
 * 
 * This proxy automatically registers Circle with the ShapeFactory
 * during static initialization, making it available for creation
 * via factory.create("circle").
 */
namespace {
    Proxy<ShapeFactory, Circle> circleProxy("circle");
}
```

## Conclusion

The `Proxy.hpp` implementation provides an **elegant solution for automatic factory registration** that demonstrates several advanced C++ concepts:

### Key Innovations
1. **Template metaprogramming**: Sophisticated type deduction and compatibility checking
2. **Static initialization patterns**: Leveraging C++ initialization order for registration
3. **Design pattern composition**: Combining Proxy, Factory, and Singleton patterns
4. **Modern C++ features**: Uses C++17 `std::invoke_result_t` and other contemporary features

### Production Readiness
The implementation is **suitable for production use** with:
- **Exception safety**: Proper error handling and propagation
- **Type safety**: Compile-time verification of correctness
- **Performance**: Zero runtime overhead after initialization
- **Maintainability**: Clean separation of concerns and clear interfaces

### Appropriate Applications
- **Plugin architectures**: Self-registering plugin systems
- **Extensible applications**: Applications that need runtime type discovery
- **Factory-based designs**: Systems using factory patterns for object creation
- **Configuration-driven systems**: Applications where available types are determined dynamically

This proxy implementation represents a **sophisticated approach to automatic registration** that eliminates boilerplate code while maintaining type safety and performance, making it an excellent choice for modern C++ applications requiring flexible object creation systems.