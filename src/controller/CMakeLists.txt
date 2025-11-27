add_library(controller
        CaptureController.cpp
        ${CMAKE_SOURCE_DIR}/include/controller/CaptureController.h
        StartupWindowController.cpp
        ${CMAKE_SOURCE_DIR}/include/controller/StartupWindowController.h
)

target_include_directories(controller PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(controller Qt6::Widgets Qt6::Core Qt6::Gui)

