add_subdirectory(Cpp)
add_subdirectory(JavaScript)

set(SRC ${SRC}
	Components/NamespaceReader.cpp
	Components/StandardParser.cpp

	Components/NamespaceReader.hpp
	Components/StandardParser.hpp
PARENT_SCOPE)
