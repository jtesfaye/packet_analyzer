add_library(layer4_protocols
        TCP.cpp
)

target_include_directories(layer4_protocols PUBLIC ${CMAKE_SOURCE_DIR}/include)
