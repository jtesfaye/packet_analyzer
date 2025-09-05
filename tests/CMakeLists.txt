add_library(test_PcapFile
        MockPcapDumper.h
        test_PcapFile.cpp
        test_ThreadPool.cpp
        test_PacketParse.cpp
        test_protcol.cpp
)

target_include_directories(test_PcapFile PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_PcapFile PRIVATE GTest::gtest_main GTest::gmock)