add_library(util
        DetailsReader.cpp
        PacketBuffer.cpp
        ${CMAKE_SOURCE_DIR}/include/util/PacketBuffer.h
        PacketGetter.cpp
        ${CMAKE_SOURCE_DIR}/include/util/PacketGetter.h
        PacketRefBuffer.cpp
        PacketObserver.cpp
        ${CMAKE_SOURCE_DIR}/include/util/ThreadPool.h
        PacketPresenter.cpp

)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(util PUBLIC Qt6::Widgets Qt6::Core)