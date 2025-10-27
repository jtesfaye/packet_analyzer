add_library(util
        PacketRead.cpp
        RowFactory.cpp
        ${CMAKE_SOURCE_DIR}/include/util/PacketBuffer.h
        PacketObserver.cpp
        ${CMAKE_SOURCE_DIR}/include/util/PacketObserver.h
        ${CMAKE_SOURCE_DIR}/include/util/LRUCache.h
        ${CMAKE_SOURCE_DIR}/include/util/IContainerType.h
        TreeModelFactory.cpp

)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(util PUBLIC Qt6::Widgets Qt6::Core)