add_library(util
        ${CMAKE_SOURCE_DIR}/include/util/LRUCache.h
        PacketUtil.cpp
        PktRingBuffer.cpp
)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(util PUBLIC Boost::container Boost::json)