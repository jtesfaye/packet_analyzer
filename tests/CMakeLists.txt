
function(add_gtest test_name)
    add_executable(${test_name} ${ARGN})
    target_link_libraries(${test_name} PRIVATE GTest::gtest_main Packet parsing layerx)
    target_include_directories(${test_name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    include(GoogleTest)
    gtest_discover_tests(${test_name})
endfunction()

add_gtest(test_PcapFile ./test_PcapFile.cpp)
add_gtest(test_parser ./test_PacketParse.cpp)
add_gtest(test_layer2 ./test_layer2.cpp)
add_gtest(test_layer3 ./test_layer3.cpp)
add_gtest(test_layer4 ./test_layer4.cpp)