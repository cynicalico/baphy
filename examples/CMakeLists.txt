add_executable(example example.cpp)
target_compile_features(example PRIVATE cxx_std_23)
target_link_libraries(example PRIVATE baphy::baphy)
