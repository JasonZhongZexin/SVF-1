add_llvm_executable(cfl cfl.cpp)
target_link_libraries(cfl PUBLIC ${llvm_libs} ${NEO4J_CLIENT_LIBRARIES} SvfLLVM )