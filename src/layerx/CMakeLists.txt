add_subdirectory(layer2)
add_subdirectory(layer3)
add_subdirectory(layer4)

add_library(layerx)

target_include_directories(layerx PUBLIC ${CMAKE_SOURCE_DIR}/include)

