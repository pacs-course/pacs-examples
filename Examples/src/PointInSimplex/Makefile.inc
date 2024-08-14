MODULENAME=PointInSimplex# should be changed
PY_INCLUDES != python3-config --includes
PY_EXT != python3-config --extension-suffix
PY_FLAGS=-fPIC -flto #flto is not strictly needed
PY_MODULE=$(MODULENAME)$(PY_EXT)
CPPFLAGS+=$(PY_INCLUDES) -DHAVE_EIGEN
CXXFLAGS+=$(PY_FLAGS)
LDFLAGS+=-flto
PY_OBJS=pyPointInSimplex.o
pyModule: $(PY_MODULE)

$(PY_MODULE): $(OBJS_NOEXEC) $(PY_OBJS)
	$(CXX) $(CPPFLAGS) -shared $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $(PY_MODULE)
