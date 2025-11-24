add_library(model
        RowModel.cpp
        ${CMAKE_SOURCE_DIR}/include/model/RowModel.h
        DetailModel.cpp
)

target_include_directories(model PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(model Qt6::Widgets Qt6::Core Qt6::Gui)
