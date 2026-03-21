CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic

TARGET := odr_example
SRCS := main.cpp fun3.cpp
OBJS := $(SRCS:.cpp=.o)

FIXED_TARGET := odr_example_fixed
FIXED_SRCS := main_fixed.cpp fun3_fixed.cpp
FIXED_OBJS := $(FIXED_SRCS:.cpp=.o)

.PHONY: all fixed clean

all: $(TARGET)

fixed: $(FIXED_TARGET)

# Linking step: combine object files into the final executable.
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Linking step for corrected variant.
$(FIXED_TARGET): $(FIXED_OBJS)
	$(CXX) $(FIXED_OBJS) -o $(FIXED_TARGET)

# Compilation step: compile each translation unit separately into .o files.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(FIXED_OBJS) $(TARGET) $(FIXED_TARGET)
