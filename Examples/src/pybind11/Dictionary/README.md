# Processing an heterogeneous python dictionary in C++

This example shows how to process an heterogeneous python dictionary in C++ using pybind11.
    
## Description
    
This example demonstrates how to pass a dictionary from Python to C++ and process it using pybind11.
The main problem here is that python allows for dictionaries with values of different type, that cannot be mapped to a `std::map<std::string, T>` in C++. 

To overcome the problem we need to use the special class `py::dict` provided by pybind11. This class allows to access the dictionary values using the `[]` operator and to check the type of the value.
    
## Usage

To use the provided `process_dict` function, make sure to have `dictionary` module available in your path. You can run the example with the following command:

```bash
python3 test.py
```

## Requirements

- Python 3.x
- pybind11 installed

## Author

Luca Formaggia