add_library(session
        ${CMAKE_SOURCE_DIR}/include/session/CaptureSession.h
        CaptureSession.cpp
        CaptureEngine.cpp
)

target_include_directories(session PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(session PRIVATE capture parsing util Packet)