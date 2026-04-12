find_package(pybind11 CONFIG REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_DIRS})

pybind11_add_module(sniffer_py bindings.cpp ../src/sniffer_api.cpp)

target_link_libraries(sniffer_py PRIVATE sniffer_main)
