//
// Created by jeremiah tesfaye on 7/31/25.
//

#ifndef PCAPDUMPERWRAPPER_H
#define PCAPDUMPERWRAPPER_H

#include <cstdio>

class IPcapDumper {
public:

    virtual ~IPcapDumper() = default;
    virtual void* get_raw_dumper() = 0;
    virtual FILE* get_file() = 0;


};

#endif //PCAPDUMPERWRAPPER_H
