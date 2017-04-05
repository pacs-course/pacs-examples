bin_PROGRAMS=integration

lib_LTLIBRARIES = libintegration.la
libintegration_la_SOURCES = domain.cpp mesh.cpp numerical_integration.cpp numerical_rule.cpp
libintegration_la_LDFLAGS = -version-info 1:0:0

include_HEADERS = domain.hpp mesh.hpp numerical_integration.hpp numerical_rule.hpp

integration_SOURCES = main_integration.cpp 
integration_LDADD = libintegration.la

