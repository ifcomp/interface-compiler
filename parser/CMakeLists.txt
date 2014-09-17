set(SRC_LIST
   ${SRC_LIST}
   ${CMAKE_CURRENT_SOURCE_DIR}/yamlParser.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/namespaceParser.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/langConfigParser.cpp
   PARENT_SCOPE
)

set(HDR_LIST
   ${HDR_LIST}
   ${CMAKE_CURRENT_SOURCE_DIR}/yamlParser.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/namespaceParser.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/langConfigParser.hpp
  PARENT_SCOPE
)
