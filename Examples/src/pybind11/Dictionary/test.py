#!/usr/bin/env python3

# Add your code below this line
# indicate to look the working directory for modules
import sys
sys.path.append('.')
# import the module
import dictionary
data = {
    "name": "Alice",
    "age": 30,
    "height": 5.5,
    "is_student": False,
    "courses": ["Math", "Science"]
}
dictionary.process_dict(data)
