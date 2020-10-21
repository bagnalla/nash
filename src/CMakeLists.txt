include_directories(${nash_SOURCE_DIR}/include)

add_executable(nash main.cc mwu.cc solver.cc)

target_link_libraries(nash)
