add_llvm_executable(mta mta.cpp)
target_link_libraries(mta PUBLIC ${llvm_libs} ${NEO4J_CLIENT_LIBRARIES} SvfLLVM)
