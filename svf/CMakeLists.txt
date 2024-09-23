# Define the core library
add_library(SvfCore)

# Add the public headers as an include directory
target_include_directories(SvfCore
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${SVF_INSTALL_INCLUDE_DIR}>
)

# Set output directories for the library
set_target_properties(SvfCore PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Set library search path and find the lgraph library
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/local/lib64)
find_library(LGRAPH_LIB lgraph PATHS /usr/local/lib64)

# Check if the library was found
if(NOT LGRAPH_LIB)
    message(FATAL_ERROR "lgraph library not found in /usr/local/lib64")
endif()

# Get the source files (i.e. all *.c/*.cpp files) for SVF's subprojects and add them to SvfCore
file(GLOB_RECURSE SVF_CORE_SOURCES lib/*.cpp)
target_sources(SvfCore PRIVATE ${SVF_CORE_SOURCES})

# Link the found library to the target
target_link_libraries(SvfCore PRIVATE ${LGRAPH_LIB})

# Get all of the public header files (i.e. all *.h/*.hpp files) for SVF's subprojects and add them
file(GLOB_RECURSE SVF_CORE_HEADERS include/*.h include/*.hpp)

target_sources(SvfCore
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES ${SVF_CORE_HEADERS}
)

# Install the headers and compiled libraries (associate SvfCore with the SVF package's targets)
install(
  TARGETS SvfCore
  EXPORT SVFTargets
  RUNTIME DESTINATION ${SVF_INSTALL_BIN_DIR}
  LIBRARY DESTINATION ${SVF_INSTALL_LIB_DIR}
  ARCHIVE DESTINATION ${SVF_INSTALL_LIB_DIR}
  FILE_SET HEADERS DESTINATION ${SVF_INSTALL_INCLUDE_DIR}
)
