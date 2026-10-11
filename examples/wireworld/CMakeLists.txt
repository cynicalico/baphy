add_executable(wireworld main.cpp wireworld.cpp wireworld_app.cpp)
target_compile_features(wireworld PRIVATE cxx_std_23)
target_link_libraries(wireworld PRIVATE baphy::baphy)
