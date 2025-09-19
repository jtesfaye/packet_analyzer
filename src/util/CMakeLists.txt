add_library(util
        PacketRead.cpp
        RowFactory.cpp
        PacketRefBuffer.cpp
        PacketObserver.cpp

)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(util PUBLIC Qt6::Widgets Qt6::Core)