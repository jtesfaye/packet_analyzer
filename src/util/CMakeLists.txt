add_library(util
        ${CMAKE_SOURCE_DIR}/include/util/SparsePacketBuffer.h
        PacketObserver.cpp
        ${CMAKE_SOURCE_DIR}/include/util/PacketObserver.h
        ${CMAKE_SOURCE_DIR}/include/util/IContainerType.h
        ${CMAKE_SOURCE_DIR}/include/util/LRUCache.h
        PacketUtil.cpp

)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(util PUBLIC Boost::container)