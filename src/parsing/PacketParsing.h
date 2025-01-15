
#ifndef PACKETPARSING_H
#define PACKETPARSING_H

#include <cstdint>

namespace Parse {

  class PacketParse {
  public:

    PacketParse() = delete;
    PacketParse(PacketParse&) = delete;
    PacketParse operator= (PacketParse&) = delete;
    PacketParse(PacketParse&&) = delete;

    virtual ~PacketParse();

    virtual void parse(uint8_t* data) = 0;

  };

};
#endif
