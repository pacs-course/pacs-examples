#include <pybind11/pybind11.h>

#include <iostream>
#include <string>

// ------------------
// regular C++ header
// ------------------

struct Type
{
  enum class Value : int
  {
    Cat,
    Dog,
  };
};

// ----------------
// regular C++ code
// ----------------

void
which_animal(Type::Value animal)
{
  if (animal == Type::Value::Cat)
    std::cout << "Cat" << std::endl;
  else if (animal == Type::Value::Dog)
    std::cout << "Dog" << std::endl;
  else
    std::cout << "Unknown" << std::endl;
}

// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(example, m)
{
  py::module sm = m.def_submodule("Type", "Type enumerator");

  py::enum_<Type::Value>(sm, "Type")
    .value("Cat", Type::Value::Cat)
    .value("Dog", Type::Value::Dog)
    .export_values();

  m.def("which_animal", &which_animal, py::arg("animal"));
}
