add_library(view
        MainWindow.cpp
        ${CMAKE_SOURCE_DIR}/include/view/MainWindow.h
        SessionForm.cpp
        ${CMAKE_SOURCE_DIR}/include/view/SessionForm.h
        Sidebar.cpp
        ${CMAKE_SOURCE_DIR}/include/view/Sidebar.h
)

target_include_directories(view PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(view PUBLIC Qt6::Widgets Qt6::Core Qt6::Gui)