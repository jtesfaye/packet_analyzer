add_library(layer4_protocols
        TCP.cpp
        ICMP.cpp
        UDP.cpp
        Layer4Registry.cpp
)

target_include_directories(layer4_protocols PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(layer4_protocols PUBLIC util)
target_link_libraries(layer4_protocols PUBLIC
        util
        Boost::unordered
        Boost::container_hash
        Boost::container
        Boost::json)
