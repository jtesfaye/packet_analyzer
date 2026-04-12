add_library(stream
        Stream.cpp
        StreamTable.cpp
        StreamStatistics.cpp
        TCPStream.cpp
)
target_include_directories(stream PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(stream PUBLIC
        util
        Boost::unordered
        Boost::container_hash
        Boost::container
        Boost::json)