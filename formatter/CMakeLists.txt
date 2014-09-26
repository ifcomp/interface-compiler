add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/stream")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/cpp")

set(SRC_LIST
   ${SRC_LIST}
   ${CMAKE_CURRENT_SOURCE_DIR}/generator.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/formatter.cpp
   PARENT_SCOPE
)

set(HDR_LIST
   ${HDR_LIST}
   ${CMAKE_CURRENT_SOURCE_DIR}/generator.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/formatter.hpp
   PARENT_SCOPE
)
