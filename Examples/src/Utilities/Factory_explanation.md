# Factory.hpp - Generic Object Factory Implementation

This header file implements a sophisticated **Generic Factory Pattern** using modern C++ features. The factory pattern is a creational design pattern that provides an interface for creating objects without specifying their exact classes, allowing for flexible and extensible object creation systems.

## Design Pattern Overview

The Factory pattern solves the problem of creating objects when the exact type isn't known until runtime. Instead of using direct constructors (`new MyClass()`), clients request objects through an identifier, and the factory handles the instantiation logic. This approach provides:

- **Decoupling**: Client code doesn't depend on concrete classes
- **Extensibility**: New types can be added without modifying existing code  
- **Runtime flexibility**: Object types can be determined from configuration, user input, etc.
- **Centralized creation logic**: All object creation happens in one place

## Singleton Implementation Pattern

### Meyer's Singleton

The factory is implemented as a **Singleton** using Meyer's technique:

```cpp
static Factory &Instance() {
    static Factory theFactory;  // Meyer's trick
    return theFactory;
}
```

#### Why Meyer's Singleton?

- **Thread-safe** (in C++11+): Static local variables are initialized exactly once
- **Lazy initialization**: Factory is created only when first accessed
- **Automatic cleanup**: Destructor called automatically at program exit
- **No heap allocation**: Uses stack allocation for better performance

#### Singleton Enforcement

```cpp
private:
    Factory() = default;                    // Private constructor
    Factory(Factory const &) = delete;     // Deleted copy constructor
    Factory &operator=(Factory const &) = delete;  // Deleted assignment
```

This ensures only one instance can exist by:
- Making constructor private (prevents direct instantiation)
- Deleting copy operations (prevents copying the singleton)
- Forcing access through `Instance()` method

## Template Parameters Architecture

The factory is highly generic through three template parameters:

```cpp
template <typename AbstractProduct, typename Identifier, typename Builder>
class Factory
```

### 1. AbstractProduct
- **Purpose**: Base class or interface type that all created objects inherit from
- **Usage**: `std::unique_ptr<AbstractProduct>` is the return type
- **Example**: `class Shape` (base class for `Circle`, `Rectangle`, etc.)

### 2. Identifier  
- **Purpose**: Key type used to identify which concrete type to create
- **Requirements**: Must be hashable (for `std::unordered_map`) and streamable (for error messages)
- **Examples**: `std::string`, `int`, `enum class ProductType`

### 3. Builder
- **Purpose**: Callable object that actually constructs the concrete objects
- **Default**: `std::function<std::unique_ptr<AbstractProduct>()>`
- **Flexibility**: Can be function pointers, lambdas, functors, or any callable

## Core Operations

### Object Registration: `add()`

```cpp
void add(Identifier const &name, Builder_type const &func);
```

**Purpose**: Associates an identifier with a builder function

**Implementation Details**:
```cpp
auto f = _storage.insert(std::make_pair(name, func));
if(f.second == false) {
    // Throw exception for duplicate registration
    throw std::invalid_argument("Double registration not allowed");
}
```

**Key Features**:
- **Duplicate prevention**: Throws exception if identifier already exists
- **Type safety**: Builder must match the template parameter type
- **Error messages**: Uses stringstream to create descriptive error messages

### Object Creation: `create()`

```cpp
template <typename... Args>
std::unique_ptr<AbstractProduct> create(Identifier const &name, Args &&...args) const;
```

**Purpose**: Creates objects by identifier, with optional constructor arguments

**Implementation Strategy**:
```cpp
static_assert(!std::is_same_v<AbstractProduct, void>, 
              "You should use get() not create() on FunctionFactories");
return this->get(name)(std::forward<Args>(args)...);
```

**Advanced Features**:
- **Perfect forwarding**: `std::forward<Args>(args)...` preserves value categories
- **Variadic templates**: Supports any number of constructor arguments
- **Compile-time safety**: `static_assert` prevents misuse with `void` types
- **Exception safety**: Leverages `get()` for consistent error handling

### Builder Retrieval: `get()`

```cpp
Builder get(Identifier const &name) const;
```

**Purpose**: Retrieves the builder function without calling it

**Error Handling**:
```cpp
auto f = _storage.find(name);
if(f == _storage.end()) {
    std::stringstream idAsString;
    idAsString << name;  // Requires operator<< for Identifier
    throw std::invalid_argument("Identifier " + idAsString.str() + " not found");
}
return f->second;
```

**Design Benefits**:
- **Flexibility**: Allows manual builder invocation with custom logic
- **Debugging**: Enables inspection of registered builders
- **Composition**: Builders can be combined or wrapped

## Storage Implementation

### Hash Map Choice

```cpp
using Container_type = std::unordered_map<Identifier, Builder_type>;
Container_type _storage;
```

**Why `std::unordered_map`?**
- **O(1) average lookup**: Fast access by identifier
- **No ordering requirement**: Unlike `std::map`, doesn't need `operator<`
- **Hash-based**: Leverages efficient hashing for performance

**Alternative Considered**:
The comment mentions `std::map` is also possible, which would provide:
- **Ordered iteration**: Deterministic order for `registered()`
- **O(log n) lookup**: Slightly slower but still acceptable
- **Comparison-based**: Only needs `operator<` instead of hash function

### Utility Operations

#### Registry Inspection: `registered()`
```cpp
std::vector<Identifier> registered() const {
    std::vector<Identifier> tmp;
    tmp.reserve(_storage.size());  // Pre-allocate for efficiency
    for(auto const &i : _storage)
        tmp.emplace_back(i.first);  // Copy identifiers
    return tmp;
}
```

**Optimization Details**:
- **Reserve**: Pre-allocates vector capacity to avoid reallocations
- **Emplace**: Constructs elements in-place for efficiency
- **Range-based loop**: Modern C++ iteration style

#### Factory Management
```cpp
void unregister(Identifier const &name) { _storage.erase(name); }
void clear() { _storage.clear(); }
bool empty() const { return _storage.empty(); }
```

These provide complete lifecycle management of the factory contents.

## Specialized Template Alias

### FunctionFactory

```cpp
template <typename Identifier, typename FunType>
using FunctionFactory = Factory<void, Identifier, FunType>;
```

**Purpose**: Specialization for storing functions rather than object factories

**Use Cases**:
- **Command pattern**: Store command functions by name
- **Plugin systems**: Register callback functions
- **Configuration**: Map string keys to function implementations

**Limitations**:
- **No `create()` method**: Since `AbstractProduct` is `void`
- **Use `get()` instead**: Retrieve and call functions manually

**Example Usage**:
```cpp
using CommandFactory = FunctionFactory<std::string, std::function<void()>>;
auto& factory = CommandFactory::Instance();
factory.add("save", []() { /* save logic */ });
auto saveCommand = factory.get("save");
saveCommand(); // Execute the command
```

## Advanced C++ Features Utilized

### 1. Perfect Forwarding
```cpp
template <typename... Args>
std::unique_ptr<AbstractProduct> create(Identifier const &name, Args &&...args) const {
    return this->get(name)(std::forward<Args>(args)...);
}
```

**Benefits**:
- **Preserves value categories**: Lvalue references stay lvalues, rvalues stay rvalues
- **Avoids unnecessary copies**: Move semantics preserved through the call chain
- **Universal references**: `Args &&...` can bind to any argument type

### 2. Variadic Templates
The `Args...` parameter pack allows the factory to work with constructors taking any number of arguments:
```cpp
// Works with any constructor signature
factory.create("circle", radius);                    // 1 argument
factory.create("rectangle", width, height);          // 2 arguments  
factory.create("complex_shape", x, y, z, color);     // 4 arguments
```

### 3. SFINAE and Type Traits
```cpp
static_assert(!std::is_same_v<AbstractProduct, void>, 
              "You should use get() not create() on FunctionFactories");
```

**Purpose**: Provides clear compile-time errors instead of confusing runtime failures

### 4. Exception Safety
The implementation provides **strong exception safety**:
- **`add()`**: Either succeeds completely or throws without modifying state
- **`get()`**: Either returns valid builder or throws with descriptive message
- **RAII**: `std::unique_ptr` ensures automatic memory management

## Real-World Usage Patterns

### 1. Shape Factory Example
```cpp
class Shape { 
public:
    virtual ~Shape() = default;
    virtual void draw() = 0;
};

class Circle : public Shape { /* implementation */ };
class Rectangle : public Shape { /* implementation */ };

// Factory setup
using ShapeFactory = Factory<Shape, std::string>;
auto& factory = ShapeFactory::Instance();

factory.add("circle", []() { return std::make_unique<Circle>(); });
factory.add("rectangle", []() { return std::make_unique<Rectangle>(); });

// Usage
auto shape = factory.create("circle");
shape->draw();
```

### 2. Plugin System
```cpp
class Plugin {
public:
    virtual ~Plugin() = default;
    virtual void execute() = 0;
};

using PluginFactory = Factory<Plugin, std::string>;

// Plugins register themselves
struct AudioPlugin : Plugin { /* implementation */ };

// Registration (could be in separate modules)
auto& factory = PluginFactory::Instance();
factory.add("audio", []() { return std::make_unique<AudioPlugin>(); });
```

### 3. Command Pattern Implementation
```cpp
using CommandFactory = FunctionFactory<std::string, std::function<void(Document&)>>;

auto& commands = CommandFactory::Instance();
commands.add("save", [](Document& doc) { doc.save(); });
commands.add("print", [](Document& doc) { doc.print(); });

// Execute commands by name
Document myDoc;
auto saveCmd = commands.get("save");
saveCmd(myDoc);
```

## Design Patterns Integration

### Factory + Singleton
The combination provides:
- **Global access point**: Factory available anywhere in the application
- **Consistent state**: Single registry shared across all components
- **Thread safety**: Meyer's singleton handles concurrent access

### Factory + Registry Pattern
The factory essentially implements a **Registry Pattern**:
- **Service location**: Find services/objects by identifier
- **Dynamic registration**: Add new types at runtime
- **Decoupled access**: Clients don't need to know concrete types

### Potential Extensions

#### 1. Abstract Factory Support
Could be extended to support families of related objects:
```cpp
template <typename AbstractFactory, typename Identifier>
class MetaFactory;
```

#### 2. Parameterized Factories
Could support factories with configuration:
```cpp
template <typename Product, typename Identifier, typename Config>
class ConfigurableFactory;
```

#### 3. Hierarchical Factories
Could support factory chains or hierarchies:
```cpp
class HierarchicalFactory {
    std::vector<std::unique_ptr<Factory>> factoryChain;
};
```

## Performance Considerations

### Time Complexity
- **Registration (`add`)**: O(1) average, O(n) worst case (hash collision)
- **Creation (`create`)**: O(1) average lookup + constructor time
- **Lookup (`get`)**: O(1) average, O(n) worst case
- **Listing (`registered`)**: O(n) where n = number of registered types

### Space Complexity
- **Storage overhead**: O(n) where n = number of registered builders
- **Memory per entry**: sizeof(Identifier) + sizeof(Builder) + hash table overhead

### Optimization Opportunities
1. **Custom hash functions**: For better distribution with custom Identifier types
2. **Memory pools**: For frequently created objects
3. **Caching**: Store recently created objects for reuse

## Thread Safety Considerations

### Singleton Thread Safety
- **Meyer's singleton**: Guaranteed thread-safe initialization in C++11+
- **Static local variables**: Compiler ensures single initialization

### Factory Operations Thread Safety
The current implementation is **not thread-safe** for modifications:
- **Concurrent reads**: Safe (const methods don't modify state)
- **Concurrent writes**: **Unsafe** - could cause data races
- **Mixed read/write**: **Unsafe** - could cause data races

### Thread-Safe Extensions
For multi-threaded environments:
```cpp
class ThreadSafeFactory {
private:
    mutable std::shared_mutex mutex_;  // Reader-writer lock
    
public:
    void add(/*...*/) {
        std::unique_lock lock(mutex_);  // Exclusive lock for writes
        // ... implementation
    }
    
    Builder get(/*...*/) const {
        std::shared_lock lock(mutex_);  // Shared lock for reads
        // ... implementation  
    }
};
```

## Error Handling Strategy

### Exception Types
The factory uses `std::invalid_argument` for all error conditions:
- **Consistent error handling**: Single exception type simplifies catch blocks
- **Descriptive messages**: Includes problematic identifier in error text
- **Strong exception safety**: Operations either succeed or leave state unchanged

### Error Scenarios
1. **Duplicate registration**: Attempting to register same identifier twice
2. **Unknown identifier**: Requesting non-existent identifier
3. **Type mismatches**: Compile-time detection via `static_assert`

### Error Message Construction
```cpp
std::stringstream idAsString;
idAsString << name;  // Requires Identifier to have operator<<
std::string out = "Identifier " + idAsString.str() + " is not stored in the factory";
```

**Requirements**: Identifier type must support streaming (`operator<<`)

## Conclusion

The `Factory.hpp` implementation demonstrates **sophisticated C++ design** that combines multiple patterns and advanced language features:

### Key Strengths
1. **Type safety**: Compile-time guarantees through templates and SFINAE
2. **Performance**: Hash-based lookup with minimal overhead
3. **Flexibility**: Works with any callable builder and identifier type
4. **Extensibility**: Easy to add new types without modifying existing code
5. **Memory safety**: RAII through `std::unique_ptr`
6. **Modern C++**: Leverages C++11/14/17 features effectively

### Appropriate Use Cases
- **Plugin architectures**: Dynamic loading of components
- **Configuration-driven object creation**: Types determined by config files
- **Polymorphic object hierarchies**: Abstract base classes with multiple implementations  
- **Command pattern implementations**: Dynamic command registration and execution
- **Serialization systems**: Create objects from type identifiers

### Design Trade-offs
- **Memory overhead**: Hash table storage vs. direct instantiation
- **Runtime lookup**: Identifier resolution vs. compile-time binding
- **Complexity**: Generic design vs. simple concrete factories

This factory implementation provides a **production-ready foundation** for flexible object creation systems in modern C++ applications, demonstrating how advanced language features can create elegant and efficient design pattern implementations.