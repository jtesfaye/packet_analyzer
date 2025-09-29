add_library(view
        StartupWindow.cpp
        ${CMAKE_SOURCE_DIR}/include/view/StartupWindow.h
        SessionForm.cpp
        ${CMAKE_SOURCE_DIR}/include/view/SessionForm.h
        Sidebar.cpp
        ${CMAKE_SOURCE_DIR}/include/view/Sidebar.h
)

target_include_directories(view PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(view PUBLIC Qt6::Widgets Qt6::Core Qt6::Gui)