#add_library(test_PcapFile
#        MockPcapDumper.h
#        test_PcapFile.cpp
#        test_ThreadPool.cpp
#        test_PacketParse.cpp
#        test_protcol.cpp
#)

#target_include_directories(test_PcapFile PUBLIC ${CMAKE_SOURCE_DIR}/include)
#target_link_libraries(test_PcapFile PRIVATE GTest::gtest_main GTest::gmock)
function(add_gtest test_name)
    add_executable(${test_name} ${ARGN})
    target_link_libraries(${test_name} PRIVATE GTest::gtest_main Packet parsing layerx)
    target_include_directories(${test_name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    include(GoogleTest)
    gtest_discover_tests(${test_name})
endfunction()

add_gtest(test_parser ./test_PacketParse.cpp)
add_gtest(test_layer2 ./test_layer2.cpp)
add_gtest(test_layer3 ./test_layer3.cpp)
