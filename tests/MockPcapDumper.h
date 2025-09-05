//
// Created by jeremiah tesfaye on 7/31/25.
//

#ifndef MOCKPCAPDUMPER_H
#define MOCKPCAPDUMPER_H
#include <include/packet/PcapDumperWrapper.h>
#include <gmock/gmock.h>

class MockPcapDumper : public IPcapDumper {

public:

    MOCK_METHOD(void*, get_raw_dumper, (), (override));
    MOCK_METHOD(FILE*, get_file, (), (override));

};

#endif //MOCKPCAPDUMPER_H
