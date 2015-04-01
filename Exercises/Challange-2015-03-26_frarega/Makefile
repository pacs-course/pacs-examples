DEPS = mesh.hpp
OBJ = mesh2D.o mesh3D.o
CXXFLAGS = -std=c++11
SOURCE = main.cpp

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
main: $(OBJ) $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $@  $^

.PHONY: clear

clear:
	$(RM) $(OBJ) main
