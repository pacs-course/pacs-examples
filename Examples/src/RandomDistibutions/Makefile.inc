.DEFAULT_GOAL = all
main_writeDensityFunction: main_writeDensityFunction.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o main_writeDensityFunction main_writeDensityFunction.cpp
