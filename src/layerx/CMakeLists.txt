add_subdirectory(layer2)
add_subdirectory(layer3)
add_subdirectory(layer4)

add_library(layerx
        Layer.cpp)
target_link_libraries(layerx PUBLIC layer4_protocols layer3_protocols layer2_protocols)
target_include_directories(layerx PUBLIC ${CMAKE_SOURCE_DIR}/include)

