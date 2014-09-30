add_subdirectory(Cpp)

set(SRC ${SRC}
	Components/YamlReader.cpp
	Components/LangConfigReader.cpp
	Components/NamespaceReader.cpp
	Components/StandardParser.cpp
	Components/TestFormatter.cpp

	Components/YamlReader.hpp
	Components/LangConfigReader.hpp
	Components/NamespaceReader.hpp
	Components/StandardParser.hpp
	Components/TestFormatter.hpp
PARENT_SCOPE)
