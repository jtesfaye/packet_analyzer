add_library(model
        DisplayModel.cpp
        ${CMAKE_SOURCE_DIR}/include/model/DisplayModel.h
)

target_include_directories(model PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(model Qt6::Widgets Qt6::Core Qt6::Gui)
