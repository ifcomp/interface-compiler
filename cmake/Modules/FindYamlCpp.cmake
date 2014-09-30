# CMake module to search for the yaml-cpp library
# If it's found it sets YamlCpp_FOUND to TRUE
# and following variables are set:
#    YamlCpp_INCLUDE_DIRS
#    YamlCpp_LIBRARIES

FIND_PATH(YamlCpp_INCLUDE_DIRS NAMES yaml-cpp/yaml.h)
FIND_LIBRARY(YamlCpp_LIBRARIES NAMES yaml-cpp libyaml-cpp libyaml-cppmdd)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(YamlCpp DEFAULT_MSG YamlCpp_LIBRARIES YamlCpp_INCLUDE_DIRS)
MARK_AS_ADVANCED(YamlCpp_INCLUDE_DIRS YamlCpp_LIBRARIES)
