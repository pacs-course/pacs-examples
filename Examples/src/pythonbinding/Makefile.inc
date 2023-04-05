PYTHON_COMMAND ?= python3
PY_INCLUDES != python3-config --includes
PY_EXT != python3-config --extension-suffix

# Here the name of the module
#PY_MODULE=pyEikonal$(DIM)$(PY_EXT)
