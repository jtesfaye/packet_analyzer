//
// Created by jeremiah tesfaye on 7/5/25.
//

#ifndef DETAILSREADER_H
#define DETAILSREADER_H
#include <string>
#include "../packet/ProtocolTypes.h"

class DetailsReader {
public:

    std::string operator()(packet::frame::EN10MB& pkt) const;

    std::string operator()(auto&& other) const {return "Not yet defined\n";}

};



#endif //DETAILSREADER_H
